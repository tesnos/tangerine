#include "player.h"

AudioOutBuffer audiobuf;
AudioOutBuffer* audiobuf_finished;

u32 audiobuf_size = (60000 + 0xfff) & ~0xfff;
u8* audiobuf_data;

FILE* wavfile;

void player_init()
{
	audoutInitialize();
	audoutStartAudioOut();
	
	audiobuf_data = memalign(0x1000, audiobuf_size);
	memset(audiobuf_data, 0, audiobuf_size);
}

void player_loadfile(char* filename)
{
	wavfile = fopen(filename, "rb");
}

void player_updateaudio()
{
	fread(audiobuf_data, audiobuf_size, 1, wavfile);
	
	audiobuf.next = NULL;
	audiobuf.buffer = audiobuf_data;
	audiobuf.buffer_size = audiobuf_size;
	audiobuf.data_size = audiobuf_size;
	audiobuf.data_offset = 0;
	
	audiobuf_finished = NULL;
	
	audoutPlayBuffer(&audiobuf, &audiobuf_finished);
}

void player_exit()
{
	free(audiobuf_data);
	fclose(wavfile);
	audoutStopAudioOut();
	audoutExit();
}