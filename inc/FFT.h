/*
 * FFT.h
 *
 *  Created on: May 12, 2017
 *      Author: ken
 */

#ifndef SRC_FFT_H_
#define SRC_FFT_H_

#include <stdint.h>
#include "fftw3.h"


namespace TransferViaSound
{

class FFT
{
public:
	FFT();
	virtual ~FFT();

public:
	int Prepare(int16_t *data, int size);
	int DoFFT(void);
	double GetModel(int num);

private:
	fftw_complex *fft_in_;
	fftw_complex *fft_out_;
	int16_t *data_;
	int size_;
	fftw_plan *plan_;
	double *model_;

private:
	void ToComplex();
	void DoWindow(void);
};

} /* namespace TransferViaSound */

#endif /* SRC_FFT_H_ */
