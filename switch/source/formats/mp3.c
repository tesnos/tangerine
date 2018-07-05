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

void exitmp3()
{
	mpg123_close(mpg);
	mpg123_delete(mpg);
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

TrackMetadata* get_tmdmp3(const char* filename)
{
	TrackMetadata* meta = malloc(sizeof(TrackMetadata));
	
	int initerror = MPG123_OK;
	mpg123_init();
	mpg = mpg123_new(NULL, &initerror);
	//enable picture processing
	initerror = mpg123_param(mpg, MPG123_ADD_FLAGS, 0x10000, 0);
	if (initerror != MPG123_OK) { return initerror; }
	initerror = mpg123_open(mpg, filename);
	if (initerror != MPG123_OK) { return initerror; }
	
	
	mpg123_id3v1* id3v1;// = malloc(sizeof(mpg123_id3v1));
	mpg123_id3v2* id3v2;// = malloc(sizeof(mpg123_id3v2));
	
	if (MPG123_ID3 & mpg123_meta_check(mpg))
	{
		
	}
	
	initerror = mpg123_id3(mpg, &id3v1, &id3v2);
	if (initerror != MPG123_OK) { return initerror; }
	
	
	//Prefer id3v2 over id3v1
	// if (id3v2 != NULL)
	// {
		// mpg123_picture* pic;
		// for (int i = 0; i < id3v2->pictures; i++)
		// {
			// pic = &(id3v2->picture[i]);
			// if (pic->type != 3) //&& pic->type != 1)
			// {
				// continue;
			// }
			
			// char* blep = pic->mime_type.p;
			// printf(blep);
		// }
		clear_tmd(meta);
		meta->picdata[0] = (long) NULL;
		//strcpy(meta->name, "BAL\0");
		snprintf(meta->name, 30, "%i", mpg123_length(mpg));
		snprintf(meta->artist, 30, "%s", id3v2->artist->p);
		snprintf(meta->album, 30, "%s", id3v2->album->p);
		//if (id3v2->title != NULL && id3v2->title->p != NULL && id3v2->title->fill != 0) { snprintf(meta->name, 0x2, "%s", "B\0"); }
		//if (id3v2->title->fill > 255) { strncpy(meta->name, id3v2->title->p, 10); }
		//strncpy(meta->name, id3v2->title->p, id3v2->title->fill);
		//strcpy(meta->name, id3v2->title->p);
		//strcpy(meta->artist, id3v2->artist->p);
		//strcpy(meta->album, id3v2->album->p);
		//strncpy(
		//strncpy(meta->artist, id3v2->artist->p, id3v2->artist->fill);
		//strncpy(meta->album, id3v2->album->p, id3v2->album->fill);
	// }
	// else if (id3v1 != NULL)
	// {
		// meta->picdata[0] = (long) NULL;
		// strncpy(meta->name, id3v1->title, 30);
		// strncpy(meta->artist, id3v1->artist, 30);
		// strncpy(meta->album, id3v1->album, 30);
	// }
	//else
	//{
		//clear_tmd(meta);
	//}
	
	mpg123_close(mpg);
	mpg123_delete(mpg);
	mpg = NULL;
	
	//free(id3v1);
	//free(id3v2);
	
	return meta;
}