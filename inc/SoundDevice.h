/*
 * SoundDevice.h
 *
 *  Created on: May 10, 2017
 *      Author: ken
 */

#ifndef SRC_SOUNDDEVICE_H_
#define SRC_SOUNDDEVICE_H_

#include <stdint.h>
#include <alsa/asoundlib.h>

class SoundDevice
{
public:
	SoundDevice();
	virtual ~SoundDevice();
public:
	int Init(void);
	int Send(uint16_t *msg, int len);
	int Receive(uint16_t *msg, int len);

private:
	snd_pcm_t *pcm_play_handle_;
	snd_pcm_t *pcm_capture_handle_;
	char *pcm_name_;
	snd_pcm_stream_t stream_;//
	int open_mode_;
	unsigned int rate_; //采样率
	unsigned int channels_; //声音通道数
	unsigned long int buffer_size_; //缓冲大小
};

#endif /* SRC_SOUNDDEVICE_H_ */
