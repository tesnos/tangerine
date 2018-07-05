#include "player.h"

AudioOutBuffer audiobuf;
AudioOutBuffer* audiobuf_finished;

u32 audiobuf_size = (60000 + 0xfff) & ~0xfff;
u8* audiobuf_data;

FILE* wavfile;

Thread* buffer_thread;
bool buffer_isfinished, buffer_isfirst;

void player_init()
{
	audoutInitialize();
	audoutStartAudioOut();
	
	audiobuf_data = memalign(0x1000, audiobuf_size);
	memset(audiobuf_data, 0, audiobuf_size);
	
	//buffer_thread = malloc(sizeof(Thread));
	//buffer_thread = &test_thread;
}

void player_loadfile(char* filename)
{
	wavfile = fopen(filename, "rb");
	buffer_isfinished = true;
	buffer_isfirst = true;
}

void player_updatebuffer_thread(void* arg)
{
	fread(audiobuf_data, audiobuf_size, 1, wavfile);
	
	audiobuf.next = NULL;
	audiobuf.buffer = audiobuf_data;
	audiobuf.buffer_size = audiobuf_size;
	audiobuf.data_size = audiobuf_size;
	audiobuf.data_offset = 0;
	
	audiobuf_finished = NULL;
	
	audoutPlayBuffer(&audiobuf, &audiobuf_finished);
	
	buffer_isfinished = true;
}

void player_updateaudio()
{
	if (buffer_isfinished)
	{
		if (buffer_isfirst)
		{
			buffer_isfirst = false;
		}
		else
		{
			threadClose(buffer_thread);
		}
		buffer_isfinished = false;
		
		buffer_thread = util_thread_func(player_updatebuffer_thread);
		//threadCreate(buffer_thread, player_updatebuffer_thread, NULL, 0x10000, 0x2C, -2);
		//threadStart(buffer_thread);
	}
}

void player_exit()
{
	free(audiobuf_data);
	fclose(wavfile);
	audoutStopAudioOut();
	audoutExit();
}