#include "flac.h"

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_BUFFER_SIZE FLACBUFSIZE

#include "dr_flac.h"

drflac* pFlac;

//"fLaC"
int flac_magic0 = 0x664c6143;

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
	drflac_read_s16(pFlac, FLACBUFSIZE, audiobuf);
}

int get_fposflac()
{
	//return ftell(audiofile);
	return 0;
}

void exitflac()
{
	drflac_close(pFlac);
	pFlac = NULL;
}

int get_bufsizeflac()
{
	return FLACBUFSIZE;
}

int process_headerflac()
{
	int verificationerrs = 0;
	int errnum;
	
	int numchannels = get_channelsflac();
	
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
		return FLACERR_NONE;
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