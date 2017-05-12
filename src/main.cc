/*
 * main.cc
 *
 *  Created on: May 10, 2017
 *      Author: ken
 */

#include "SoundDevice.h"

int main(int argc, char *argv[])
{
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

	return 0;
}

