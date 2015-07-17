#ifndef __AUDIO_DATA__
#define __AUDIO_DATA__

struct audio_data {
    int size;
    int sampling_rate;
    void* data;
};

struct audio_data get_audio_data(char *file);

#endif
