#ifndef FFT
#define FFT

#include <iostream>
#include <math.h>
#define PI 3.141592653589793238462643383279

class complex{

public:
    double real;
    double img;

};

class fft{

public:
    fft();
    int getFFTSize();
    int getSampleRate();
    double *getFFTData();

    void calculateFFT(short int *wave);

private:
    void filldata(short int *wave);
    void fftprocess();
    void scale();
    complex EE(complex a,complex b);
    complex *rawdata;
    int datasize;
    int Fdatasize;
    int fftsize;
    int samplerate;
    double *fftdata;

};

#endif // FFT

