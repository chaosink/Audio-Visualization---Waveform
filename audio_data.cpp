#include <stdio.h>
#include <stdlib.h>
#include "audio_data.hpp"

struct WAV_HEADER
{
    char rld[4]; //riff 标志符号
    int rLen; 
    char wld[4]; //格式类型（wave）
    char fld[4]; //"fmt"
    int fLen; //sizeof(wave format matex)
    short wFormatTag; //编码格式
    short wChannels; //声道数
    int nSamplesPersec ; //采样频率
    int nAvgBitsPerSample;//WAVE文件采样大小
    short wBlockAlign; //块对齐
    short wBitsPerSample; //WAVE文件采样大小
    char dld[4]; //”data“
    int wSampleLength; //音频数据的大小
} wav_header;

struct audio_data get_audio_data(char *file) {
    int nread;
    FILE *fp = fopen(file, "rb");
    nread = fread(&wav_header, 1, sizeof(wav_header), fp);
    struct audio_data data;
    data.size = wav_header.rLen - 36;
    data.sampling_rate = wav_header.nSamplesPersec;
    data.data = malloc(data.size);
    fseek(fp, 44, SEEK_SET);
    nread = fread(data.data, data.size, 1, fp);
    fclose(fp);
    return data;
}
