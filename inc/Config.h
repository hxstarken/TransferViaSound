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
	static const int BASE_FREQ = 8000;
	static const int FREQ_STEP = 10;
	static const int SAMPLE_RATE = 48000;
	static const int TIME_BAND = 8192;
	static const int MAX_SIGNAL_STRENGTH = 32767/*65535*/;
	static const int START_COMMAND = 130;
	static const int STOP_COMMAND = 128;

	static const int RING_BUFFER_SIZE = 1024;

	static const int VOICE_TEMP_BUF_SIZE = TIME_BAND;

};

} /* namespace TransferViaSound */

#endif /* CONFIG_H_ */
