#include <stdio.h>
#include "audio_data.hpp"

int main() {
	audio_data data = get_audio_data((char *)"audio/30.wav");
	for(int i = 0; i < data.size / 2; i++) printf("%p ", ((short*)data.data)[i]);
	return 0;
}
