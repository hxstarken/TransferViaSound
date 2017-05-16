/*
 * RingBuf.cc
 *
 *  Created on: May 16, 2017
 *      Author: ken
 */

#include <RingBuf.h>
#include <string.h>
#include <unistd.h>

namespace TransferViaSound
{

RingBuf::RingBuf()
{
	// TODO Auto-generated constructor stub
	head_ = 0;
	tail_ = 0;
}

RingBuf::~RingBuf()
{
	// TODO Auto-generated destructor stub
}

/** @brief put data to the ring buffer
 * @data the point of data
 * @len the length of data
 * return 0 if SUCCESS
 */
int RingBuf::Put(uint8_t *data, int len)
{
	int capacity = 0, min_len = 0;
//	const unsigned char *data = _data;
	do
	{
		capacity = DATA_SIZE - tail_;
		min_len = (len < capacity ? len: capacity);//min(len, capacity);
		memcpy(buf_+tail_, data, min_len);
		tail_ += capacity;
		if (tail_ >= DATA_SIZE) tail_ = 0;
		data += min_len;
		len -= min_len;

	}while(len > 0);
	return len;
}

/** @brief get data to the ring buffer
 * @data the point of data
 * @len the length of data
 * return 0 if SUCCESS
 */
int RingBuf::Get(uint8_t *data, int len)
{
	int i = 0;
	for (i=0; i<len; i++)
	{
		GetOneChar(data+i);
	}
	return 0;
}

/**
 * @brief 从缓冲池中取出一个数据，同时，缓冲池中的head数据指针下移一位
 * @param _list 缓冲池地址
 * @param _data 存放取得数据的地址
 * @return 取到数据返回 0
 */
int RingBuf::GetOneChar(uint8_t *_data)
{
	unsigned int time_out = 0;
	while (time_out < 1000)
	{
		time_out++;
		if (head_ < tail_)
		{
			*_data = buf_[head_];
			++head_;
			return 0;
		}
		else if (head_ == tail_)
		{
			//调用延时函数 用于进程切换
			sleep(1);
			continue;
		}
		else
		{
			*_data = buf_[head_];
			if (head_ >= DATA_SIZE)
			{
				head_ = 0;
			}
			else
			{
				++head_;
			}
			return 0;
		}
	}
	return -1;//超时
}

} /* namespace TransferViaSound */
