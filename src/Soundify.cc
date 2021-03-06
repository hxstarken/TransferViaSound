/*
 * Soundify.cc
 *
 *  Created on: May 15, 2017
 *      Author: ken
 */

#include <Soundify.h>
#include "SoundifyUtils.h"
#include "Config.h"
#include <math.h>
#include <stdlib.h>

using namespace TransferViaSound;



/** @brief
 *
 */
static void ParaseFunc(void *arg)
{
	int err = 0, i = 0;
	double current_model = 0, max_model = 0;
	uint16_t max_model_num = 0;
	int freq = 0;
	char data = 0;
	FFT fft;
	SoundifyUtils soundify_utils;

	uint16_t rec_buf[Config::TIME_BAND];

	ThreadArg *thread_arg = (ThreadArg *)arg;
	err = fft.Prepare((int16_t *)rec_buf, Config::TIME_BAND);
	if (err)
	{
		return;
	}

	while (1)
	{
		//get voice data
		err = thread_arg->sound_dev->Receive(rec_buf, Config::TIME_BAND);
		if (err)
		{
			continue;
		}
		// fft
		fft.DoFFT();
		//calculate max fft model
		max_model = 0;
		max_model_num = 0;

		//TODO
		for (i=0/*Config::BASE_FREQ*Config::TIME_BAND/Config::SAMPLE_RATE*/; i<4096; i++)
		{
			current_model =  fft.GetModel(i);
			if (max_model < current_model)
			{
				max_model = current_model;
				max_model_num = i;
			}
		}

		freq = (max_model_num)*Config::SAMPLE_RATE/Config::TIME_BAND;

		if (freq < Config::BASE_FREQ)
		{
			continue;
		}
		printf("freq:%d\n", freq);
		//CalcData
		data = soundify_utils.CalcData(freq);
		printf("the data is:%c\n", data);
		//put to ringbuf
		thread_arg->ring_buf->Put((uint8_t *)&data, 1);
	}
}



namespace TransferViaSound
{

Soundify::Soundify()
{
	// TODO Auto-generated constructor stub
	parase_thread_ = nullptr;
	sound_dev_ = nullptr;
	receive_ring_buf_ = nullptr;
	thread_arg_ = nullptr;
}

Soundify::~Soundify()
{
	// TODO Auto-generated destructor stub
	delete thread_arg_;
	delete receive_ring_buf_;
	delete sound_dev_;
	delete parase_thread_;
}

/**
 * @return 0 if SUCCESS
 */
int Soundify::Init(void)
{
	int err = 0;

	sound_dev_ = new SoundDevice;
	if (sound_dev_ == nullptr)
	{
		return -2;
	}
	err = sound_dev_->Init();
	if (err)
	{
		goto __sound_dev_init_err;
	}

	return err;

__sound_dev_init_err:
	delete sound_dev_;
	sound_dev_ = nullptr;
	return err;
}

/** @brief start receive
 * @return 0 if SUCCESS
 */
int Soundify::StartReceive(void)
{
	int err = 0;

	if (parase_thread_ != nullptr)
	{
		return 0;
	}
	//------------------------------------
	//------------  first start -----------
	receive_ring_buf_ = new RingBuf;
	if (receive_ring_buf_ == nullptr)
	{
		return -2;
	}

	thread_arg_ = new ThreadArg;
	if (thread_arg_ == nullptr)
	{
		goto __new_thread_arg_err;
	}
	//------------------------------------
	//-------- fill thread_arg_ --------
	thread_arg_->ring_buf = receive_ring_buf_;
	thread_arg_->sound_dev = sound_dev_;

	//------------------------------------
	//----- new receive thread -----------
	parase_thread_ = new std::thread(ParaseFunc, thread_arg_);
	if (parase_thread_ == nullptr)
	{
		goto __new_thread_err;
	}

	return 0;

__new_thread_err:
	delete thread_arg_;
	thread_arg_ = nullptr;
__new_thread_arg_err:
	delete receive_ring_buf_;
	receive_ring_buf_ = nullptr;
	return err;
}

int Soundify::Send(uint8_t *msg, int len)
{
	int i = 0, j = 0;
	char *data = (char *)msg;
	int freq;
	int block = 0;

	int16_t *audio = (int16_t *)malloc(sizeof(int16_t) * len * Config::TIME_BAND);

	if (audio == nullptr)
	{
		printf("malloc error\n");
		return -1;
	}

	for (i=0; i<len; i++)
	{
		block = i * Config::TIME_BAND;
		freq = soundify_utils_.CalcFreq(data[i]);
		printf("send freq %d\n", freq);
		for (j = 0; j < Config::TIME_BAND; j++)
		{	// Percorre o tamanho de cada banda de frequencia
			double angle = 2.0 * j * freq * M_PI / Config::SAMPLE_RATE;// Realiza o calculo do angulo da frequencia
//			list.add((short) (Math.sin(angle) * Config.MAX_SIGNAL_STRENGTH));
			audio[block+j] = (int16_t)(sin(angle) * Config::MAX_SIGNAL_STRENGTH);
//			printf("audio:%d\t", audio[block+j]);
		}
	}

	sound_dev_->Send((uint16_t *)audio, len * Config::TIME_BAND);

//	delete audio;
	free(audio);
	return 0;
}

int Soundify::Receive(uint8_t *msg, int len)
{
	// TODO 需要完善
	return receive_ring_buf_->Get(msg, len);
}


} /* namespace TransferViaSound */




