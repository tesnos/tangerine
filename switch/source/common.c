#include "common.h"

void clear_tmd(TrackMetadata* meta)
{
	meta->picdata[0] = (long) NULL;
	strcpy(meta->name, nodata);
	strcpy(meta->artist, nodata);
	strcpy(meta->album, nodata);
}