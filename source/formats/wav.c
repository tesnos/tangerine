#include "wav.h"
#include "dr_wav.h"

drwav* pWav;
long int wavprogress;
bool wavdone = false;

int get_sampleratewav()
{
	return pWav->sampleRate;
}

int get_progresswav()
{
	return ((wavprogress * 100) / pWav->totalSampleCount);
}

int get_channelswav()
{
	return pWav->channels;
}

void read_sampleswav(void* audiobuf)
{
	if (wavdone) { return; }
	
	wavprogress += drwav_read_s16(pWav, WAVBUFSIZE, audiobuf);
	
	if (wavprogress >= pWav->totalSampleCount)
	{
		wavdone = true;
	}
}

void exitwav()
{
	drwav_close(pWav);
	pWav = NULL;
}

void seekwav(int percentage)
{
	drwav_seek_to_sample(pWav, (int) (pWav->totalSampleCount * (percentage / 100.0)));
	wavprogress = (int) (pWav->totalSampleCount * (percentage / 100.0));
}

int get_bufsizewav()
{
	return WAVBUFSIZE;
}

int process_headerwav()
{
	int verificationerrs = 0;
	int errnum;
	
	int numchannels = get_channelswav();
	
	if (numchannels > 2)
	{
		verificationerrs++;
		errnum = WAVERR_EXTRA_CHANNELS;
	}
	
	if (verificationerrs > 0)
	{
		if (verificationerrs > 1)
		{
			return WAVERR_MULTIPLE;
		}
		else
		{
			return errnum;
		}
	}
	else
	{
		return WAVERR_NONE;
	}
}

int init_audiowav(const char* filename)
{
	wavprogress = 0;
	wavdone = false;
	
	pWav = drwav_open_file(filename);
	if (pWav == NULL) {
		return WAVERR_DW_FAIL;
	}
	
	return process_headerwav();
}