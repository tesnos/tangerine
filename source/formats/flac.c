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

int get_samplerate()
{
  return pFlac->sampleRate;
}

int get_channels()
{
  return pFlac->channels;
}

void read_samples(void* audiobuf)
{
  drflac_read_s16(pFlac, FLACBUFSIZE * numchannels, audiobuf);
}

int get_fpos()
{
  return ftell(audiofile);
}

int process_header()
{
  int verificationerrs = 0;
  int errnum;
  
  if ((headerbuf[0] << 24) + (headerbuf[1] << 16) + (headerbuf[2] << 8) + (headerbuf[3]) != flac_magic0)
  {
    verificationerrs++;
	errnum = FLACERR_WRONG_MAGIC;
  }
  
  sample_rate = get_samplerate()
  num_channels = get_channels();
  
  if (get_channels() > 2)
  {
    verificationerrs++;
	errnum = FLACRR_EXTRA_CHANNELS;
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

int init_audio(FILE* flacfile)
{
  audiofile = flacfile;
  
  fseek(audiofile, 0, SEEK_SET);
  fread(headerbuf, 1, 5, audiofile);
  fseek(audiofile, 0, SEEK_SET);
  
  pFlac = drflac_open(fread, fseek, audiofile);
  if (pFlac == NULL) {
    return FLACERR_DF_FAIL;
  }
  
  return process_header();
}