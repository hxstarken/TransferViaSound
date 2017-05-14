/*
 * FFT.cc
 *
 *  Created on: May 12, 2017
 *      Author: ken
 */

#include "FFT.h"
#include <math.h>


namespace TransferViaSound
{

enum {
	REAL = 0,
	IMAG = 1,
};

FFT::FFT()
{
	// TODO Auto-generated constructor stub
	fft_in_ = nullptr;
	fft_out_ = nullptr;
	data_ = nullptr;
	size_ = 0;
	plan_ = nullptr;
	model_ = nullptr;
}

FFT::~FFT()
{
	// TODO Auto-generated destructor stub
	delete plan_;
	if (fft_in_ != nullptr)
	{
		fftw_free(fft_in_);
	}
	if (fft_out_ != nullptr)
	{
		fftw_free(fft_out_);
	}
	if (plan_ == nullptr)
	{
		fftw_destroy_plan(*plan_);
		delete plan_;
	}
	delete model_;
}

int FFT::Prepare(int16_t *data, int size)
{
	if (data==nullptr)
		return -1;

	if (fft_in_ != nullptr)
	{
		fftw_free(fft_in_);
		fft_in_ = nullptr;
	}
	if (fft_out_ != nullptr)
	{
		fftw_free(fft_out_);
		fft_out_ = nullptr;
	}

	if (plan_ != nullptr)
	{
		fftw_destroy_plan(*plan_);
		delete plan_;
		plan_ = nullptr;
	}

	delete model_;

	model_ = new double[size];
	if (model_ == nullptr)
	{
		return -1;
	}

	fft_in_ = fftw_alloc_complex(size);
	if (fft_in_ == nullptr)
	{
		goto __malloc_in_err;
	}
	fft_out_ = fftw_alloc_complex(size);
	if (fft_out_ == nullptr)
	{
		goto __malloc_out_err;
	}

	size_ = size;
	data_ = data;

	plan_ = new fftw_plan;
	if (plan_ == nullptr)
	{
		goto __malloc_paln_err;
	}

    *plan_ = fftw_plan_dft_1d(size_,
							  fft_in_,
							  fft_out_,
							  FFTW_FORWARD,
							  FFTW_ESTIMATE);

    return 0;

__malloc_paln_err:
	fftw_free(fft_out_);
	fft_out_ = nullptr;
__malloc_out_err:
	fftw_free(fft_in_);
	fft_in_ = nullptr;
__malloc_in_err:
	delete model_;
	model_ = nullptr;
	return -1;
}

int FFT::DoFFT(void)
{
	int i = 0;
	if (plan_ == nullptr)
	{
		fprintf(stderr, "Should use FFT::Prepare before DoFFT\n");
		return -1;
	}

	ToComplex();
	DoWindow();
    fftw_execute(*plan_);
    for (i=0; i<size_; i++)
    {
    	model_[i] = sqrt(fft_out_[i][REAL]*fft_out_[i][REAL] + fft_out_[i][IMAG]*fft_out_[i][IMAG]);
    }
//    //TEST
//    int i = 0;
//    for (i=0; i<size_; i++)
//    {
//    	std::cout << "REAL: " << fft_out_[i][0] <<"    IMAGE: "<< fft_out_[i][1] << std::endl;
//    }
    return 0;

}

double FFT::GetModel(int num)
{
	return model_[num];
}

void FFT::ToComplex()
{
	int i = 0;

	for (; i<size_; i++)
	{
		fft_in_[i][REAL] = (double)data_[i];
		fft_in_[i][IMAG] = 0;
	}

}

void FFT::DoWindow(void)
{

}


} /* namespace TransferViaSound */


