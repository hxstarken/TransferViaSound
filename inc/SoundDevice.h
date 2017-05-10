/*
 * SoundDevice.h
 *
 *  Created on: May 10, 2017
 *      Author: ken
 */

#ifndef SRC_SOUNDDEVICE_H_
#define SRC_SOUNDDEVICE_H_

#include <stdint.h>

class SoundDevice
{
public:
	SoundDevice();
	virtual ~SoundDevice();
public:
	int Init(void);
	int Send(uint16_t *msg, int len);
	int Receive(uint16_t *msg, int len);
};

#endif /* SRC_SOUNDDEVICE_H_ */
