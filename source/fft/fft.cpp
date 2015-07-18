#include "fft.h"
#include "wavedata.h"


fft::fft(){
    rawdata=NULL;
    fftdata=NULL;
}

int fft::getFFTSize(){
    return fftsize;
}

int fft::getSampleRate(){
    return samplerate;
}

double *fft::getFFTData(){
    return fftdata+1;
}

void fft::calculateFFT(short int *wave){
    filldata(wave);
    fftprocess();
    scale();
    fftsize=samplerate/2-1;
}

void fft::filldata(short int *wave){
    

    samplerate=44100;
    datasize=735;
    

    Fdatasize=(int)pow(2.0,ceil(log((double)samplerate)/log(2.0)));

    if(rawdata==NULL){
        rawdata=new complex[Fdatasize];
    }
    if(fftdata==NULL){
        fftdata=new double[Fdatasize];
    }
    for(int i=0;i<Fdatasize;i++){
        if(i<samplerate){
            int index=i%datasize;
            rawdata[i].real=wave[index*2];
            rawdata[i].img=0;
        }
        else{
            rawdata[i].real=0;
            rawdata[i].img=0;
        }
    }
}

void fft::fftprocess(){
    int i,j=0,k;
    complex t;
    for(i=0;i<Fdatasize-1;i++){
        if(i<j){
            t=rawdata[j];
            rawdata[j]=rawdata[i];
            rawdata[i]=t;
        }
        k=Fdatasize/2;
        while(k<=j){
            j-=k;
            k/=2;
        }
        j+=k;
    }
    int stage,le,lei,ip;
    complex u,w;
    j=Fdatasize;
    for(stage=1;(j=j/2)!=1;stage++); //caculate stage,which represents  butterfly stages
    for(k=1;k<=stage;k++){
        le=2<<(k-1);
        lei=le/2;
        u.real=1.0;// u,butterfly factor initial value
        u.img=0.0;
        w.real=cos(PI/lei);
        w.img=sin(PI/lei);
        for(j=0;j<=lei-1;j++){
            for(i=j;i<=Fdatasize-1;i+=le){
                ip=i+lei;
                t= EE(rawdata[ip],u);
                rawdata[ip].real=rawdata[i].real- t.real;
                rawdata[ip].img=rawdata[i].img-t.img;
                rawdata[i].real=rawdata[i].real+t.real;
                rawdata[i].img=rawdata[i].img+t.img;
            }
            u=EE(u, w);
        }
    }
}

complex fft::EE(complex a,complex b){
    complex c;
    c.real=a.real*b.real-a.img*b.img;
    c.img=a.real*b.img+a.img*b.real;
    return c;
}

void fft::scale(){
    for(int i=0;i<Fdatasize;i++){
        fftdata[i]=sqrt(rawdata[i].real*rawdata[i].real+rawdata[i].img*rawdata[i].img)*2/Fdatasize;
    }
}



