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
#include <string.h>
#include "Soundify.h"
#include <thread>

using namespace TransferViaSound;

#define NUM_POINTS 512


void ParaseFun(Soundify *arg)
{
	int err = 0;
	uint8_t temp;

	err = arg->StartReceive();
	if (err)
	{
		printf("start receive err");
	}
	while (1)
	{
		arg->Receive(&temp, 1);
		printf("-----------------------\n");
		printf("receive is:%c\n", temp);
	}
}


int main(int argc, char *argv[])
{
	char send_buf[100];
	int temp;
	int err = 0;
	Soundify test;
	err = test.Init();
	if (err < 0)
	{
		std::cout<<"device init error"<< std::endl;
	}

	std::thread receive(ParaseFun, &test);
	sprintf(send_buf, "1234567887654321");

	while (1)
	{
		std::cout<<"-----input command----"<<std::endl;
		std::cin>>temp;
		if (temp == 1)
		{
			test.Send((uint8_t *)send_buf, strlen(send_buf));
		}
	}


//	sprintf(send_buf, "345678345678345678345678345678345678");
//	test.Send((uint8_t *)send_buf, strlen(send_buf));
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
#if 0
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
#endif
}

