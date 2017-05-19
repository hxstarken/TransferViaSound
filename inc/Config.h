/*
 * Config.h
 *
 *  Created on: May 16, 2017
 *      Author: ken
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

namespace TransferViaSound
{

class Config
{
public:
	Config(){};
	virtual ~Config(){};

public:
	static const int16_t BASE_FREQ = 3000;
	static const int16_t FREQ_STEP = 10;
	static const int32_t SAMPLE_RATE = 44100;
	static const int32_t TIME_BAND = 8192;
	static const int32_t MAX_SIGNAL_STRENGTH = 65535;
	static const int16_t START_COMMAND = 130;
	static const int16_t STOP_COMMAND = 128;

	static const int16_t RING_BUFFER_SIZE = 1024;

	static const int32_t VOICE_TEMP_BUF_SIZE = SAMPLE_RATE;

};

} /* namespace TransferViaSound */

#endif /* CONFIG_H_ */
