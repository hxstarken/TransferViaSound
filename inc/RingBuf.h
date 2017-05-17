/*
 * RingBuf.h
 *
 *  Created on: May 16, 2017
 *      Author: ken
 */

#ifndef RINGBUF_H_
#define RINGBUF_H_

#include <stdint.h>
#include "Config.h"

namespace TransferViaSound
{

class RingBuf
{

public:
	RingBuf();
	virtual ~RingBuf();

public:
	/** @brief put data to the ring buffer
	 * @data the point of data
	 * @len the length of data
	 * return 0 if SUCCESS
	 */
	int Put(uint8_t *data, int len);

	/** @brief get data to the ring buffer
	 * @data the point of data
	 * @len the length of data
	 * return 0 if SUCCESS
	 */
	int Get(uint8_t *data, int len);

private:
	int GetOneChar(uint8_t *_data);

private:
	uint8_t buf_[Config::TIME_BAND];
	uint16_t head_;//read
	uint16_t tail_;//put
};

} /* namespace TransferViaSound */

#endif /* RINGBUF_H_ */
