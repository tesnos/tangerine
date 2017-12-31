#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_WIN32_IO

#include "dr_flac.h"

#include "flac.h"

FILE* audiofile;
int samplerate;
int numchannels;
int8_t headerbuf[45];

drflac* pFlac;

//"fLaC"
int flac_magic0 = 0x52494646;

int get_samplerateflac()
{
	return pFlac->sampleRate;
}

int get_channelsflac()
{
	return pFlac->channels;
}

void read_samplesflac(void* audiobuf)
{
	drflac_read_s16(pFlac, FLACBUFSIZE * numchannels, audiobuf);
}

int get_fposflac()
{
	return ftell(audiofile);
}

int get_bufsizeflac()
{
	return FLACBUFSIZE;
}

int process_headerflac()
{
	int verificationerrs = 0;
	int errnum;
	
	// if ((headerbuf[0] << 24) + (headerbuf[1] << 16) + (headerbuf[2] << 8) + (headerbuf[3]) != flac_magic0)
	// {
		// verificationerrs++;
		// errnum = FLACERR_WRONG_MAGIC;
	// }
	
	samplerate = get_samplerateflac();
	numchannels = get_channelsflac();
	
	if (numchannels > 2)
	{
		verificationerrs++;
		errnum = FLACERR_EXTRA_CHANNELS;
	}
	
	if (verificationerrs > 0)
	{
		if (verificationerrs > 1)
		{
			return FLACERR_MULTIPLE;
		}
		else
		{
			return errnum;
		}
	}
	else
	{
		return numchannels;
	}
}

int init_audioflac(const char* filename)
{
	pFlac = drflac_open_file(filename);
	if (pFlac == NULL) {
		return FLACERR_DF_FAIL;
	}
	
	return process_headerflac();
}