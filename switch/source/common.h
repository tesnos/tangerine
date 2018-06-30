#include <switch.h>
#include <stdio.h>
#include <string.h>

#ifndef COMMON_H_
#define COMMON_H_

static char* const nodata = "?\0";
static char* const haspic = "haspic\0";
static char* const newline = "\n\0";
static char* const nodatanl = "?\n\0";

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
	char name[256];
	char artist[256];
	char album[256];
} TrackMetadata;

void clear_tmd(TrackMetadata* meta);

#endif