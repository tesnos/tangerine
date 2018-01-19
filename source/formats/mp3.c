#include "mp3.h"

mpg123_handle* mpg;
long int samplerate;
int numchannels;
int encoding;
bool mp3done = false;
size_t bytesread;

int get_sampleratemp3()
{
	return samplerate;
}

int get_progressmp3()
{
	return (mpg123_tell(mpg) / (mpg123_length(mpg) / 100));
}

int get_channelsmp3()
{
	return numchannels;
}

void read_samplesmp3(void* audiobuf)
{
	if (mp3done) { return; }
	
	mpg123_read(mpg, audiobuf, MP3BUFSIZE * numchannels, &bytesread);
	if (bytesread < MP3BUFSIZE * numchannels)
	{
		mp3done = true;
	}
}

int get_fposmp3()
{
	return 0;
}

void exitmp3()
{
	mpg123_close(mpg);
	mpg = NULL;
	samplerate = 0;
	numchannels = 0;
	mp3done = false;
	bytesread = 0;
}

void seekmp3(int percentage)
{
	mpg123_seek(mpg, (int) (mpg123_length(mpg) * (percentage / 100.0)), SEEK_SET);
}

int get_bufsizemp3()
{
	return MP3BUFSIZE;
}

int process_headermp3()
{
	int verificationerrs = 0;
	int errnum;
	
	int initerror = mpg123_getformat(mpg, &samplerate, &numchannels, &encoding);
	if (initerror != MPG123_OK) { return initerror; }
	
	if (numchannels > 2)
	{
		verificationerrs++;
		errnum = MP3ERR_EXTRA_CHANNELS;
	}
	
	if (verificationerrs > 0)
	{
		if (verificationerrs > 1)
		{
			return MP3ERR_MULTIPLE;
		}
		else
		{
			return errnum;
		}
	}
	else
	{
		return MP3ERR_NONE;
	}
}

int init_audiomp3(const char* filename)
{
	int initerror = MPG123_OK;
	
	initerror = mpg123_init();
	if (initerror != MPG123_OK) { return initerror; }
	
	mpg = mpg123_new(NULL, &initerror);
	if (initerror != MPG123_OK) { return initerror; }
	
	initerror = mpg123_open(mpg, filename);
	if (initerror != MPG123_OK) { return initerror; }
	
	return process_headermp3();
}