/*
 * SoundDevice.cc
 *
 *  Created on: May 10, 2017
 *      Author: ken
 */

#include "SoundDevice.h"


SoundDevice::SoundDevice()
{
	// TODO Auto-generated constructor stub
	pcm_play_handle_ = nullptr;
	pcm_capture_handle_ = nullptr;

	char name[] = "default";
	pcm_name_ = new char[sizeof(name)]();
	memcpy(pcm_name_, name, sizeof(name));

	stream_ = SND_PCM_STREAM_PLAYBACK;
	open_mode_ = 0;
	rate_ = 48000;
	channels_ = 1;
	buffer_size_ = 1024;
}

SoundDevice::~SoundDevice()
{
	// TODO Auto-generated destructor stub
	delete pcm_name_;

	if (pcm_play_handle_ != NULL)
	{
		snd_pcm_drain(pcm_play_handle_);
		snd_pcm_close(pcm_play_handle_);
	}

	if (pcm_capture_handle_ != NULL)
	{
		snd_pcm_drain(pcm_capture_handle_);
		snd_pcm_close(pcm_capture_handle_);
	}
}

int SoundDevice::Init(void)
{
	int err = -1, dir = 0;
	snd_pcm_hw_params_t *hw_params;

	//-----------------------------------------------------
	//-------------- 打开发声设备 ---------------------------
	stream_ = SND_PCM_STREAM_PLAYBACK;
	err = snd_pcm_open(&pcm_play_handle_, pcm_name_, stream_, open_mode_);
	if (err < 0)
	{
		goto __open_play_err;
	}

	err = snd_pcm_hw_params_malloc(&hw_params);
	if (err < 0)
	{
		goto __alloc_paly_param_err;
	}

	err = snd_pcm_hw_params_any(pcm_play_handle_, hw_params);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}

	err = snd_pcm_hw_params_set_access(pcm_play_handle_, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}
	err = snd_pcm_hw_params_set_format(pcm_play_handle_, hw_params, SND_PCM_FORMAT_S16_LE);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}

	err = snd_pcm_hw_params_set_rate_near(pcm_play_handle_, hw_params, &rate_, NULL);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}

	err = snd_pcm_hw_params_set_channels(pcm_play_handle_, hw_params, channels_);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}

	err = snd_pcm_hw_params(pcm_play_handle_, hw_params);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}

	/* Set period size to 32 frames. */
	snd_pcm_hw_params_set_period_size_near(pcm_play_handle_,
			hw_params, &buffer_size_, &dir);

	snd_pcm_hw_params_free(hw_params);

	err = snd_pcm_prepare (pcm_play_handle_);
	if (err<0)
	{
		goto __alloc_paly_param_err;
	}

	//-----------------------------------------------------
	//--------------------打开mic设备-----------------------
	stream_ = SND_PCM_STREAM_CAPTURE;
	err = snd_pcm_open(&pcm_capture_handle_, pcm_name_, stream_, open_mode_);
	if (err < 0)
	{
		goto __alloc_paly_param_err;
	}

	err = snd_pcm_hw_params_malloc(&hw_params);
	if (err < 0)
	{
		goto __alloc_receive_param_err;
	}

	err = snd_pcm_hw_params_any(pcm_capture_handle_, hw_params);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}

	err = snd_pcm_hw_params_set_access(pcm_capture_handle_, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}
	err = snd_pcm_hw_params_set_format(pcm_capture_handle_, hw_params, SND_PCM_FORMAT_S16_LE);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}

	err = snd_pcm_hw_params_set_rate_near(pcm_capture_handle_, hw_params, &rate_, NULL);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}

	err = snd_pcm_hw_params_set_channels(pcm_capture_handle_, hw_params, channels_);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}

	snd_pcm_hw_params_set_period_size_near(pcm_capture_handle_,
			hw_params, &buffer_size_, &dir);

	err = snd_pcm_hw_params(pcm_capture_handle_, hw_params);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}
	snd_pcm_hw_params_free(hw_params);

	err = snd_pcm_prepare (pcm_capture_handle_);
	if (err<0)
	{
		goto __alloc_receive_param_err;
	}

	return 0;
__alloc_receive_param_err:
	snd_pcm_close(pcm_capture_handle_);
	pcm_capture_handle_ = nullptr;
__alloc_paly_param_err:
	snd_pcm_close(pcm_play_handle_);
	pcm_play_handle_ = nullptr;
__open_play_err:
	return err;
}

int SoundDevice::Send(uint16_t *msg, int len)
{
	int err = -1;
	int count = 0;

	do
	{
		err = snd_pcm_writei(pcm_play_handle_, msg+count, len-count);	//写入声卡 （放音）
		if (err == -EPIPE)
		{
			/* EPIPE means underrun */
			fprintf(stderr, "underrun occurred\n");
			snd_pcm_prepare (pcm_play_handle_);
			return -1;
		}
		else if (err < 0)
		{
			fprintf(stderr, "error from writei: %s\n", snd_strerror(err));
			return -2;
		}
//		else if (err != buffer_size_)
//		{
//			fprintf(stderr, "short write, write %d frames\n", err);
//		}
		count += err;
	} while (count < len);

	return 0;
}

int SoundDevice::Receive(uint16_t *msg, int len)
{
	int err = 1;
	int count = 0;
	do
	{
		err = snd_pcm_readi(pcm_capture_handle_, msg+count, len-count);
		if (err == -EPIPE)
		{
			/* EPIPE means overrun */
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(pcm_capture_handle_);
			return -1;
		}
		else if (err < 0)
		{
			fprintf(stderr, "error from read: %s\n", snd_strerror(err));
			return -2;
		}
		count += err;
	} while (count < len);

	return 0;
}
