#include "player.h"

//What format the audio is in
int format = FORMAT_NONE;

//Buffers to hold audio data
int16_t* audioBuffer0 = 0;
int16_t* audioBuffer1 = 0;

//Audio file pointer
FILE* afile;

//Information about the audio file
int16_t buffersize;
int samplerate;
int16_t numchannels;

//Buffers specifically for use with the DSP
ndspWaveBuf waveBuf[2];

//Is the audio playing?
bool playing = false;

void fill_buffer(void* audioBuffer, size_t size)
{
	if (format == FORMAT_WAV)
	{
		read_sampleswav(audioBuffer);
	}
	if (format == FORMAT_FLAC)
	{
		read_samplesflac(audioBuffer);
	}
	if (format == FORMAT_MP3)
	{
		read_samplesmp3(audioBuffer);
	}
	
	DSP_FlushDataCache(audioBuffer, size);
}

void free_buffers()
{
	if (audioBuffer0 == 0 && audioBuffer1 == 0)
	{
		linearFree(audioBuffer0);
		linearFree(audioBuffer1);
		audioBuffer0 = 0;
		audioBuffer1 = 0;
	}
}

void exitplayer()
{
	//Free up the preiously allocated buffers
	free_buffers();
	
	//Close the file object
	fclose(afile);
	
	//Stop playing
	ceaseplayback();
	
	//Stop using the DSP
	ndspExit();
}

int recognize(FILE* unknownfile)
{
	int8_t header[4];
	int wav_magic = 0x52494646;
	int flac_magic = 0x664c6143;
	int mp3_magic0 = 0x494433;
	int mp3_magic1 = 0xFFFB;
	
	fseek(unknownfile, 0, SEEK_SET);
	fread(header, 1, 5, unknownfile);
	fseek(unknownfile, 0, SEEK_SET);
	
	if ((header[0] << 24) + (header[1] << 16) + (header[2] << 8) + (header[3]) == wav_magic)
	{
		return FORMAT_WAV;
	}
	else if ((header[0] << 24) + (header[1] << 16) + (header[2] << 8) + (header[3]) == flac_magic)
	{
		return FORMAT_FLAC;
	}
	else if (((header[0] << 16) + (header[1] << 8) + (header[2]) == mp3_magic0) || ((header[0] << 8) + (header[1]) == mp3_magic1))
	{
		return FORMAT_MP3;
	}
	
	return FORMAT_NONE;
}

bool* playerInit()
{
	//Initialize the DSP (audio) chip for use
	ndspInit();
	
	//Play stereo audio, regardless of whether input is mono
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	
	//Reset,
	ndspChnReset(0);
	//clear,
	ndspChnWaveBufClear(0);
	//set interpolation of,
	ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);
	
	//Set the front right and left volumes of our channel to 100% but silence everything else
	float mix[12];
	memset(mix, 0, sizeof(mix));
	mix[0] = 1.0;
	mix[1] = 1.0;
	ndspChnSetMix(0, mix);
	
	return &playing;
}

void ceaseplayback()
{
	if (format == FORMAT_NONE) { return; }
	
	if (format == FORMAT_WAV)
	{
		exitwav();
	}
	else if (format == FORMAT_FLAC)
	{
		exitflac();
	}
	else if (format == FORMAT_MP3)
	{
		exitmp3();
	}
	
	playing = false;
}

void seekaudio(int percentage)
{
	if (format == FORMAT_NONE) { return; }
	
	if (playing)
	{
		toggle_playback();
	}
	
	if (format == FORMAT_WAV)
	{
		seekwav(percentage);
	}
	else if (format == FORMAT_FLAC)
	{
		seekflac(percentage);
	}
	else if (format == FORMAT_MP3)
	{
		seekmp3(percentage);
	}
	
	if (!playing)
	{
		toggle_playback();
	}
}

int playfile(const char* filename)
{
	//Reset and
	ndspChnReset(0);
	//clear
	ndspChnWaveBufClear(0);
	
	int success;
	
	//Open the audio file
	afile = fopen(filename, "rb");
	
	//Figure out what format the audio is
	format = recognize(afile);
	
	fclose(afile);
	//Try and open the file, get some information if it succeeds
	if (format == FORMAT_WAV)
	{
		svcOutputDebugString("wav\0", 4);
		success = init_audiowav(filename);
		if (success > 0)
		{
			return success;
		}
		
		buffersize = get_bufsizewav();
		samplerate = get_sampleratewav();
		numchannels = get_channelswav();
	}
	if (format == FORMAT_FLAC)
	{
		svcOutputDebugString("flac\0", 5);
		success = init_audioflac(filename);
		if (success > 0)
		{
			return success;
		}
		
		buffersize = get_bufsizeflac();
		samplerate = get_samplerateflac();
		numchannels = get_channelsflac();
	}
	if (format == FORMAT_MP3)
	{
		svcOutputDebugString("mp3\0", 4);
		success = init_audiomp3(filename);
		if (success > 0)
		{
			return success;
		}
		
		buffersize = get_bufsizemp3();
		samplerate = get_sampleratemp3();
		numchannels = get_channelsmp3();
	}
	if (format == FORMAT_NONE)
	{
		return 0x10;
	}
	
	if (numchannels == 2)
	{
		//Dual channel audio
		ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
	}
	else
	{
		//Single channel audio
		ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM16);
	}
	
	//and set the samplerate of the NDSP
	ndspChnSetRate(0, samplerate);
	
	//Allocate space in memory for our 2 wavebuffers the size of the buffer * # of channels
	free_buffers();
	audioBuffer0 = linearAlloc(buffersize * numchannels);
	audioBuffer1 = linearAlloc(buffersize * numchannels);
	
	//Clear wavebuffers and configure their addresses and how many samples they contain
	memset(waveBuf, 0, sizeof(waveBuf));
	waveBuf[0].data_vaddr = &audioBuffer0[0];
	waveBuf[0].nsamples = (buffersize / numchannels);
	waveBuf[1].data_vaddr = &audioBuffer1[0];
	waveBuf[1].nsamples = (buffersize / numchannels);
	
	//Put the first bits of audio in them
	fill_buffer(audioBuffer0, buffersize * numchannels);
	fill_buffer(audioBuffer1, buffersize * numchannels);
	
	//Then queue them
	ndspChnWaveBufAdd(0, &waveBuf[0]);
	ndspChnWaveBufAdd(0, &waveBuf[1]);
	
	playing = true;
	
	return 0;
}

int get_progress()
{
	if (format == FORMAT_WAV)
	{
		return get_progresswav();
	}
	if (format == FORMAT_FLAC)
	{
		return get_progressflac();
	}
	if (format == FORMAT_MP3)
	{
		return get_progressmp3();
	}
	return 0;
}

int play_audio()
{
	//Checks if the audio data inside the buffers is done playing and refills + requeues them if so
	if (waveBuf[0].status == NDSP_WBUF_DONE) {
			fill_buffer(waveBuf[0].data_pcm16, buffersize * numchannels);
			
			ndspChnWaveBufAdd(0, &waveBuf[0]);
	}
	
	if (waveBuf[1].status == NDSP_WBUF_DONE) {
			fill_buffer(waveBuf[1].data_pcm16, buffersize * numchannels);
			
			ndspChnWaveBufAdd(0, &waveBuf[1]);
	}
	
	return get_progress();
}

void toggle_playback()
{
	playing = !playing;
	ndspChnSetPaused(0, !playing);
}