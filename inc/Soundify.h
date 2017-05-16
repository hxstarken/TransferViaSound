/*
 * Soundify.h
 *
 *  Created on: May 15, 2017
 *      Author: ken
 */

#ifndef SOUNDIFY_H_
#define SOUNDIFY_H_

#include <thread>
#include <stdint.h>
#include "FFT.h"
#include "SoundDevice.h"
#include "RingBuf.h"
#include "SoundifyUtils.h"

namespace TransferViaSound
{

struct ThreadArg
{
	SoundDevice *sound_dev;//音频设备
	RingBuf *ring_buf;
};

class Soundify
{
public:
	Soundify();
	virtual ~Soundify();

public:
	int Init(void);
	int StartReceive(void);//开始接收
	int Send(uint8_t *msg, int len);
	int Receive(uint8_t *msg, int len);

private:


private:

	SoundifyUtils soundify_utils_;
	std::thread *parase_thread_;
	SoundDevice *sound_dev_;
	RingBuf *receive_ring_buf_;
	ThreadArg *thread_arg_;

};

} /* namespace TransferViaSound */

#endif /* SOUNDIFY_H_ */
