#include "filemanage.h"

const char* datapath = "/switch/nxxp/data/\0";
const char* picdataext = ".png\0";
const char* datdataext = ".dat\0";

char* curdir_path = "/music\0";
DIR* curdir;

void file_init()
{
	mkdir(curdir_path, 777);
	curdir = opendir(curdir_path);
	mkdir(datapath, 777);
}

void file_reset_dir()
{
	closedir(curdir);
	curdir = opendir(curdir_path);
}

char* file_concat_path_curdir(char* filepath)
{
	char* result = malloc(strlen(curdir_path) + 1 + strlen(filepath) + 1);
	
	strcpy(result, curdir_path);
	result[strlen(curdir_path)] = 0x2F;
	strcpy(result + strlen(curdir_path) + 1, filepath);
	result[strlen(curdir_path) + 1 + strlen(filepath)] = 0x00;
	
	return result;
}

char* file_extract_path_fname(char* filepath)
{
	int i = strlen(filepath); if (i > 256) { i = 256; }
	for (; i >= 0; i--)
	{
		if (filepath[i] == 0x2F) { break; }
	}
	
	int fnamelen = strlen(filepath) - i;
	char* fname = malloc(fnamelen);
	strncpy(fname, filepath + i + 1, fnamelen);
	fname[fnamelen] = 0x00;
	
	return fname;
}

int file_get_size(FILE* unknownfile)
{
	int oldpos = ftell(unknownfile);
	
	fseek(unknownfile, 0, SEEK_END);
	int file_len = ftell(unknownfile);
	fseek(unknownfile, oldpos, SEEK_SET);
	
	return file_len;
}

char* file_get_next_line(FILE* textfile)
{
	int file_len = file_get_size(textfile);
	int linebase = ftell(textfile);
	int file_pos = linebase;
	char check_buf;
	
	fread(&check_buf, 1, 1, textfile);
	file_pos = ftell(textfile);
	
	while (file_pos < file_len)
	{
		fread(&check_buf, 1, 1, textfile);
		file_pos = ftell(textfile);
		if (check_buf == 0x0A) { break; }
	}
	
	char* line = malloc(file_pos - linebase);
	fseek(textfile, linebase, SEEK_SET);
	fread(line, file_pos - linebase, 1, textfile);
	
	if (file_pos == file_len)
	{
		line[file_pos - linebase] = 0x00;
	}
	else
	{
		line[file_pos - linebase - 1] = 0x00;
	}
	
	return line;
}

TrackMetadata* file_check_data(char* unknownfile_path)
{
	FILE* unknownfile = fopen(unknownfile_path, "rb");
	
	char digest[16];
	char hexdigest[32];
	MD5_CTX* context = malloc(sizeof(MD5_CTX));
	MD5_Init(context);
	
	int file_len = file_get_size(unknownfile);
	
	
	void* file_buffer = malloc(0x80000);
	for (int i = 0; i < floor(file_len / 0x80000); i++)
	{
		fread(file_buffer, 0x80000, 1, unknownfile);
		MD5_Update(context, file_buffer, 0x80000);
	}
	
	file_len = file_len - ftell(unknownfile);
	fread(file_buffer, file_len, 1, unknownfile);
	MD5_Update(context, file_buffer, file_len);
	MD5_Final((unsigned char*) digest, context);
	free(file_buffer);
	free(context);
	
	for (int i = 0; i < 16; i++)
	{
		snprintf(&hexdigest[i * 2], 32, "%02x", digest[i]);
	}
	
	//TrackMetadata* meta = malloc(sizeof(TrackMetadata));
	//return meta;
	
	// Hex Digest size + size of "/switch/nxxp/data/" + size of ".png"/".dat" + terminator
	char* imgpicpath = malloc(18 + 32 + 4 + 1);
	strcpy(imgpicpath, datapath);
	strcpy(imgpicpath + 18, hexdigest);
	strcpy(imgpicpath + 18 + 32, picdataext);
	imgpicpath[18 + 32 + 4] = 0x00;
	char* imgdatpath = malloc(18 + 32 + 4 + 1);
	strcpy(imgdatpath, datapath);
	strcpy(imgdatpath + 18, hexdigest);
	strcpy(imgdatpath + 18 + 32, datdataext);
	imgdatpath[18 + 32 + 4] = 0x00;
	
	//strcpy((char *) meta->name, imgpicpath);
	
	//return meta;
	
	TrackMetadata* meta = malloc(sizeof(TrackMetadata));
	
	FILE* imgmeta = fopen(imgdatpath, "rb");
	FILE* imgpic = fopen(imgpicpath, "rb");
	if (imgmeta == NULL || imgpic == NULL)
	{
		imgmeta = fopen(imgdatpath, "wb");
		imgpic = fopen(imgpicpath, "wb");
		
		FileFormat ff = file_determine_format_ptr(unknownfile);
		fclose(unknownfile);
		if (ff == Format_None)
		{
			// TODO: Put error here
		}
		else if (ff == Format_Wav)
		{
			meta = get_tmdwav(unknownfile_path);
		}
		else if (ff == Format_Flac)
		{
			meta = get_tmdflac(unknownfile_path);
		}
		else if (ff == Format_Mp3)
		{
			meta = get_tmdmp3(unknownfile_path);
		}
		
		
		if (meta->picdata[0] == (long) NULL)
		{ fwrite(nodata, strlen(nodata), 1, imgmeta); }
		else
		{ fwrite(haspic, strlen(haspic), 1, imgmeta); }
		fwrite(newline, strlen(newline), 1, imgmeta);
		
		fwrite(meta->name, strlen(meta->name), 1, imgmeta);
		fwrite(newline, strlen(newline), 1, imgmeta);
		fwrite(meta->artist, strlen(meta->artist), 1, imgmeta);
		fwrite(newline, strlen(newline), 1, imgmeta);
		fwrite(meta->album, strlen(meta->album), 1, imgmeta);
	}
	else
	{
		char* doeshavepic = file_get_next_line(imgmeta);
		if (strcmp(haspic, doeshavepic) == 0)
		{
			fread(meta->picdata, sizeof(u32) * 500 * 500, 1, imgpic);
		}
		else
		{
			meta->picdata[0] = (long) NULL;
		}
		
		fclose(unknownfile);
		strcpy(meta->name, file_get_next_line(imgmeta));
		strcpy(meta->artist, file_get_next_line(imgmeta));
		strcpy(meta->album, file_get_next_line(imgmeta));
	}
	
	fclose(imgmeta);
	fclose(imgpic);
	
	return meta;
}

int file_get_dir_size()
{
	int count = 0;
	while (true)
	{
		if (readdir(curdir) == NULL) { break; }
		else { count++; }
	}
	
	file_reset_dir();
	return count;
}

char* file_change_dir_up()
{
	char* updir;
	int i;
	for (i = strlen(curdir_path); i >= 0; i--)
	{
		if (curdir_path[i] == 0x2F) { break; }
	}
	updir = malloc(i);
	strncpy(updir, curdir_path, i);
	updir[i - 1] = 0x00;
	
	return updir;
}

void file_change_dir(char* dir_path)
{
	closedir(curdir);
	curdir_path = dir_path;
	curdir = opendir(curdir_path);
}

void file_list_dir(char** listings)
{
	file_reset_dir();
	
	int dirsize = file_get_dir_size();
	struct dirent* listing;
	char* entry;
	
	for (int i = 0; i < dirsize; i++)
	{
		listing = readdir(curdir);
		entry = listing->d_name;
		
		char* entrydest = malloc(strlen(entry) + 1);
		strncpy(entrydest, entry, strlen(entry) + 1);
		listings[i] = entrydest;
	}
	
	file_reset_dir();
}

void file_list_dir_tracks(char** track_listings)
{
	int all_dirsize = file_get_dir_size();
	char* all_listings[all_dirsize];
	file_list_dir(all_listings);
	
	int track_listing_count = 0;
	
	for (int i = 0; i < all_dirsize; i++)
	{
		char* listing_path = file_concat_path_curdir(all_listings[i]);
		FileFormat ff = file_determine_format_path(listing_path);
		if (ff != Format_None)
		{
			track_listings[track_listing_count] = malloc(strlen(listing_path) + 1);
			strcpy(track_listings[track_listing_count], listing_path);
			track_listings[track_listing_count][strlen(listing_path)] = 0x00;
			track_listing_count++;
		}
	}
	
	file_free_dir_list(all_listings, all_dirsize);
}

void file_free_dir_list(char** list, int length)
{
	for (int i = 0; i < length; i++)
	{
		free(list[i]);
	}
}

FileFormat file_determine_format_ptr(FILE* file_ptr)
{
	FILE* target_file = file_ptr;
	long int oldpos = ftell(target_file);
	fseek(target_file, 0, SEEK_SET);
	char file_magic[4];
	fread((char *) file_magic, 4, 1, target_file);
	fseek(target_file, oldpos, SEEK_SET);
	u32 file_magic_int = (file_magic[0] << 24) + (file_magic[1] << 16) + (file_magic[2] << 8) + (file_magic[3]);
	
	if (file_magic_int == 0x52494646)
	{
		return Format_Wav;
	}
	else if (file_magic_int == 0x664C6143)
	{
		return Format_Flac;
	}
	else if ((file_magic_int & 0xFFFFFF00) == 0x49443300 || (file_magic_int & 0xFFFF0000) == 0xFFFB0000)
	{
		return Format_Mp3;
	}
	else
	{
		return Format_None;
	}
}

FileFormat file_determine_format_path(char* file_path)
{
	FILE* target_file = fopen(file_path, "rb");
	FileFormat format = file_determine_format_ptr(target_file);
	fclose(target_file);
	
	return format;
}

int file_get_dir_size_tracks()
{
	int count = 0;
	struct dirent* listing;
	while (true)
	{
		listing = readdir(curdir);
		if (listing == NULL) { break; }
		else
		{
			char listingpath[strlen(curdir_path) + 1 + strlen(listing->d_name) + 1];
			strcpy(listingpath, curdir_path);
			listingpath[strlen(curdir_path)] = 0x2F;
			strcpy(listingpath + strlen(curdir_path) + 1, listing->d_name);
			listingpath[strlen(curdir_path) + 1 + strlen(listing->d_name)] = 0x00;
			
			
			if (file_determine_format_path(listingpath) != Format_None)
			{
				count++;
			}
		}
	}
	
	file_reset_dir();
	return count;
}

void file_exit()
{
	closedir(curdir);
}