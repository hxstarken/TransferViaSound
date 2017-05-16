/*
 * Soundify.cc
 *
 *  Created on: May 15, 2017
 *      Author: ken
 */

#include <Soundify.h>
#include "SoundifyUtils.h"

#define RECEIVE_SIZE 8192

using namespace TransferViaSound;



/** @brief
 *
 */
static void ParaseFunc(void *arg)
{
	int err = 0, i = 0;
	double current_model = 0, max_model = 0;
	int max_model_num = 0;
	uint16_t freq = 0;
	char data = 0;
	FFT fft;
	SoundifyUtils soundify_utils;

	uint16_t rec_buf[RECEIVE_SIZE];

	ThreadArg *thread_arg = (ThreadArg *)arg;
	fft.Prepare((int16_t *)rec_buf, RECEIVE_SIZE);

	while (1)
	{
		//get voice data
		err = thread_arg->sound_dev->Receive(rec_buf, RECEIVE_SIZE);
		if (err)
		{
			continue;
		}
		// fft
		fft.DoFFT();
		//calculate max fft model
		max_model = 0;
		for (i=0; i<(RECEIVE_SIZE>>2); i++)
		{
			current_model =  fft.GetModel(i);
			if (current_model > max_model)
			{
				max_model = current_model;
				max_model_num = i;
			}
		}

		freq = soundify_utils.CalcFreq(max_model_num - 1);
		//CalcData
		data = soundify_utils.CalcData(freq);
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






} /* namespace TransferViaSound */




