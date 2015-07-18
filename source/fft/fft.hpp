#ifndef __FFT__
#define __FFT__

#include <iostream>
#include <cmath>
#define PI 3.141592653589793238462643383279

class complex {
public:
	float real;
	float img;
};

class FFT {
public:
	FFT();
	void setDataSize(int size);
	int getFFTSize();
	void setSampleRate(int rate);
	int getSampleRate();
	float *getFFTData();
	float *calculateFFT(short *wave);
private:
	void filldata(short *wave);
	void FFTprocess();
	void scale();
	complex EE(complex a,complex b);
	complex *rawdata;
	int datasize;
	int Fdatasize;
	int FFTsize;
	int samplerate;
	float *FFTdata;
};

#endif // FFT
