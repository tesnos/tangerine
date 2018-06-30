#include "wav.h"
#include "dr_wav.h"

drwav* pWav;
long int wavprogress;
bool wavdone = false;
TrackMetadata tmd;

int get_sampleratewav()
{
	return pWav->sampleRate;
}

int get_progresswav()
{
	return (wavprogress / (pWav->totalSampleCount / 100));
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

TrackMetadata* get_tmdwav(const char* filename)
{
	TrackMetadata* meta = malloc(sizeof(TrackMetadata));
	meta->picdata[0] = (long) NULL;
	
	int i = strlen(filename); if (i > 256) { i = 256; }
	for (; i >= 0; i--)
	{
		if (filename[i] == 0x2F) { break; }
	}
	
	int fnamelen = strlen(filename) - i;
	char* fname = malloc(fnamelen);
	strncpy(fname, filename + i + 1, fnamelen);
	fname[fnamelen] = 0x00;
	
	strcpy(meta->name, fname);
	strcpy(meta->artist, nodata);
	strcpy(meta->album, nodata);
	return meta;
}