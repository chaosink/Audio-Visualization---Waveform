#include "fft.hpp"

FFT::FFT() {
	rawdata = NULL;
	FFTdata = NULL;
}

void FFT::setDataSize(int size) {
	datasize = size;
}

int FFT::getFFTSize() {
	return FFTsize;
}

void FFT::setSampleRate(int rate) {
	samplerate = rate;
}

int FFT::getSampleRate() {
	return samplerate;
}

float *FFT::getFFTData() {
	return FFTdata + 1;
}

float * FFT::calculateFFT(short *wave) {
	filldata(wave);
	FFTprocess();
	scale();
	FFTsize = samplerate / 2 - 1;
	return FFTdata + 1;
}

void FFT::filldata(short *wave) {
	Fdatasize = (int)pow(2.0, ceil(log((float)samplerate) / log(2.0)));

	if(rawdata == NULL) {
		rawdata = new complex[Fdatasize];
	}
	if(FFTdata == NULL) {
		FFTdata = new float[Fdatasize];
	}
	for(int i = 0; i < Fdatasize; i++) {
		if(i < samplerate) {
			int index = i % datasize;
			rawdata[i].real = wave[index * 2];
			rawdata[i].img = 0;
		} else {
			rawdata[i].real = 0;
			rawdata[i].img = 0;
		}
	}
}

void FFT::FFTprocess() {
	int i, j = 0, k;
	complex t;
	for(i = 0; i < Fdatasize - 1; i++) {
		if(i < j) {
			t = rawdata[j];
			rawdata[j] = rawdata[i];
			rawdata[i] = t;
		}
		k = Fdatasize / 2;
		while(k <= j) {
			j -= k;
			k /= 2;
		}
		j += k;
	}
	int stage, le, lei, ip;
	complex u, w;
	j = Fdatasize;
	for(stage = 1; (j = j / 2) != 1; stage++); //caculate stage,which represents  butterfly stages
	for(k = 1; k <= stage; k++) {
		le = 2 << (k - 1);
		lei = le / 2;
		u.real = 1.0; //u,butterfly factor initial value
		u.img = 0.0;
		w.real = cos(PI / lei);
		w.img = sin(PI / lei);
		for(j = 0; j <= lei - 1; j++) {
			for(i = j; i <= Fdatasize - 1; i += le) {
				ip = i + lei;
				t = EE(rawdata[ip], u);
				rawdata[ip].real = rawdata[i].real - t.real;
				rawdata[ip].img = rawdata[i].img - t.img;
				rawdata[i].real = rawdata[i].real + t.real;
				rawdata[i].img = rawdata[i].img + t.img;
			}
			u = EE(u, w);
		}
	}
}

complex FFT::EE(complex a, complex b) {
	complex c;
	c.real = a.real*b.real - a.img*b.img;
	c.img = a.real*b.img + a.img*b.real;
	return c;
}

void FFT::scale() {
	for(int i = 0; i < Fdatasize; i++){
		FFTdata[i] = sqrt(rawdata[i].real * rawdata[i].real + rawdata[i].img * rawdata[i].img) * 2 / Fdatasize;
	}
}
