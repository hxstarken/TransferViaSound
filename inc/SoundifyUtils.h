/*
 * SoundifyUtils.h
 *
 *  Created on: May 16, 2017
 *      Author: ken
 */

#ifndef SoundifyUtils_H_
#define SoundifyUtils_H_

#include "FFT.h"

namespace TransferViaSound
{

class SoundifyUtils
{
public:
	SoundifyUtils();
	virtual ~SoundifyUtils();

public:
	/** @brief 计算出 num 对应的频率
	 * @return freq
	 */
	int CalcFreq(char value);

	/** @brief 计算出 freq 对应的字符
	 * @return char
	 */
	char CalcData(uint16_t freq);

private:

};

} /* namespace TransferViaSound */

#endif /* SoundifyUtils_H_ */
