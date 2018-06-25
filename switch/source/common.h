#include <switch.h>
#include <stdio.h>

#ifndef COMMON_H_
#define COMMON_H_

const char* nodata = "?\0";
const char* newline = "\n\0";
const char* nodatanl = "?\n\0";

typedef enum
{
	Format_None = 0,
	Format_Wav = 1,
	Format_Flac = 2,
	Format_Mp3 = 3
} FileFormat;

typedef struct
{
	u32 picdata[500 * 500];
	char* name[256];
	char* artist[256];
	char* album[256];
} TrackMetadata;

void clear_tmd(TrackMetadata meta)
{
	meta->picdata = NULL;
	meta->name = nodata;
	meta->artist = nodata;
	meta->album = nodata;
}

#endif