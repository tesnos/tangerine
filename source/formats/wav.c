#include "wav.h"

FILE* audiofile;
int samplerate;
int numchannels;
int8_t headerbuf[45];

//"RIFF"
int wav_magic0 = 0x52494646;
//"WAVE"
int wav_magic1 = 0x57415645;
//"fmt "
int wav_magic2 = 0x666d7420;
//UNUSED, as position can vary and searching for it is uneccessary-ish.
//If additional information is put in the header besides the default, the first few samples
//may be wrong. However, this period is almost guaranteed to be too short to be noticable.
//"data" 
//int wavmagic3 = 0x64617461;

int get_sampleratewav()
{
	return 48000;
}

int get_channelswav()
{
	return numchannels;
}

int get_bufsizewav()
{
	return WAVBUFSIZE;
}

void read_sampleswav(void* audiobuf)
{
	fread(audiobuf, 1, WAVBUFSIZE * numchannels, audiofile);
}

int get_fposwav()
{
	return ftell(audiofile);
}

int process_headerwav()
{
	int verificationerrs = 0;
	int errnum;
	
	int magic0 = (headerbuf[0] << 24) + (headerbuf[1] << 16) + (headerbuf[2] << 8) + (headerbuf[3]);
	int magic1 = (headerbuf[8] << 24) + (headerbuf[9] << 16) + (headerbuf[10] << 8) + (headerbuf[11]);
	int magic2 = (headerbuf[12] << 24) + (headerbuf[13] << 16) + (headerbuf[14] << 8) + (headerbuf[15]);
	
	if (magic0 != wav_magic0)
	{
		verificationerrs++;
		errnum = WAVERR_WRONG_MAGIC;
	}
	
	if (magic1 != wav_magic1)
	{
		verificationerrs++;
		errnum = WAVERR_WRONG_MAGIC;
	}
	
	if (magic2 != wav_magic2)
	{
		verificationerrs++;
		errnum = WAVERR_WRONG_MAGIC;
	}
	
	samplerate = (headerbuf[24]) + (headerbuf[25] << 8) + (headerbuf[26] << 16) + (headerbuf[27] << 24) + 0x100;
	numchannels = (headerbuf[22]) + (headerbuf[23] << 8);
	
	if (numchannels > 2)
	{
		verificationerrs++;
		errnum = WAVERR_EXTRA_CHANNELS;
	}
	
	if (((headerbuf[16]) + (headerbuf[17] << 8) + (headerbuf[18] << 16) + (headerbuf[19] << 24) != 16) || ((headerbuf[20]) + (headerbuf[21] << 8) != 1))
	{
		verificationerrs++;
		errnum = WAVERR_NOT_PCM;
	}
	
	if (verificationerrs > 0)
	{
		if (verificationerrs > 1)
	{
			return verificationerrs + 10;
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

int init_audiowav(FILE* wavfile)
{
	audiofile = wavfile;
	fseek(audiofile, 0, SEEK_SET);
	fread(headerbuf, 1, 45, audiofile);
	fseek(audiofile, 44, SEEK_SET);
	
	return process_headerwav();
}