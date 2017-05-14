/*
 * main.cc
 *
 *  Created on: May 10, 2017
 *      Author: ken
 */

#include "SoundDevice.h"
#include "FFT.h"
#include <math.h>
#include <iostream>

using namespace TransferViaSound;

#define NUM_POINTS 512

int main(int argc, char *argv[])
{
#if 0
	int size = 1024*20*5;
	int err = -1;
	uint16_t buf[size];

	SoundDevice a;
	err = a.Init();
	if (err < 0)
	{
		return err;
	}
	a.Receive(buf, size);
	sleep(1);

	a.Send(buf, size);
#endif
	FFT a;
	int16_t fft_in[NUM_POINTS];
	int i = 0;
	for (i=0; i<NUM_POINTS; i++)
	{
		double theta = 2 * (double)i / (double)NUM_POINTS * M_PI;
		fft_in[i] = 1.0 * cos(10.0 * theta) +
                0.5 * cos(5* theta);
	}


	a.Prepare(fft_in, NUM_POINTS);
	a.DoFFT();
	for (i=0; i<NUM_POINTS; i++)
	{
		std::cout<<a.GetModel(i)<<std::endl;
	}

	return 0;
}

