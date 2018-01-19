#include "flac.h"

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_BUFFER_SIZE FLACBUFSIZE

#include "dr_flac.h"

drflac* pFlac;

//"fLaC"
int flac_magic0 = 0x664c6143;
long int flacprogress;
bool flacdone = false;

int get_samplerateflac()
{
	return pFlac->sampleRate;
}

int get_progressflac()
{
	return (flacprogress / (pFlac->totalSampleCount / 100));
}

int get_channelsflac()
{
	return pFlac->channels;
}

void read_samplesflac(void* audiobuf)
{
	if (flacdone) { return; }
	
	flacprogress += drflac_read_s16(pFlac, FLACBUFSIZE, audiobuf);
	
	if (flacprogress >= pFlac->totalSampleCount)
	{
		flacdone = true;
	}
}

void exitflac()
{
	drflac_close(pFlac);
	pFlac = NULL;
}

void seekflac(int percentage)
{
	drflac_seek_to_sample(pFlac, (int) (pFlac->totalSampleCount * (percentage / 100.0)));
	flacprogress = (int) (pFlac->totalSampleCount * (percentage / 100.0));
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
	flacprogress = 0;
	flacdone = false;
	
	pFlac = drflac_open_file(filename);
	if (pFlac == NULL) {
		return FLACERR_DF_FAIL;
	}
	
	return process_headerflac();
}