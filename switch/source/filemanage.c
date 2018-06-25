#include "filemanage.h"

const char* nodata = "?\0";
const char* newline = "\n\0";
const char* nodatanl = "?\n\0";
const char* datapath = "/switch/nxxp/data/\0";
const char* picdataext = ".png\0";
const char* datdataext = ".dat\0";

char* curdir_path = "/music\0";
DIR* curdir;

void file_init()
{
	mkdir(curdir, 777);
	curdir = opendir(curdir_path);
	mkdir("/switch/nxxp/data", 777);
}

void file_reset_dir()
{
	rewinddir(curdir);
}

char* file_extract_path_fname(char* filepath)
{
	char* fname;
	int i;
	for (i = strlen(filepath); i >= 0; i--)
	{
		if (filepath == 0x2F) { break; }
	}
	fname = malloc(strlen(filepath) - i);
	strncpy(fname, filepath + (strlen(filepath) - i), i);
	fname[strlen(filepath) - i] = 0x00;
	
	return fname;
}

TrackMetadata file_check_data(char* unknownfile_path)
{
	FILE* unknownfile = fopen(unknownfile_path, "rb");
	
	unsigned char digest[16];
	struct MD5Context context;
	MD5Init(&context);
	
	fseek(unknownfile, 0, SEEK_END);
	int file_len = ftell(unknownfile);
	fseek(unknownfile, 0, SEEK_SET);
	
	
	void* file_buffer = malloc(0x80000);
	for (int i = 0; i < floor(file_len / 0x80000); i++)
	{
		fread(file_buffer, 0x80000, 1, unknownfile);
		MD5Update(&context, file_buffer, 0x80000);
	}
	
	file_len = file_len - ftell(unknownfile);
	fread(file_buffer, file_len, 1, unknownfile);
	MD5Update(&context, file_buffer, file_len);
	MD5Final(digest, &context);
	free(file_buffer);
	
	//Digest size + size of "/switch/nxxp/data/" + size of ".png"/".dat" + terminator
	char* imgpicpath = malloc(18 + 16 + 4 + 1);
	strcpy(imgpicpath, datapath);
	strcpy(imgpicpath + 18, digest);
	strcpy(imgpicpath + 18 + 16, picdataext);
	imgpicpath[18 + 16 + 4] = 0x00;
	char* imgdatpath = malloc(16 + 16 + 4 + 1);
	strcpy(imgdatpath, datapath);
	strcpy(imgdatpath + 18, digest);
	strcpy(imgdatpath + 18 + 16, datdataext);
	imgdatpath[18 + 16 + 4] = 0x00;
	
	TrackMetadata* meta;
	
	FILE* imgmeta = fopen(imgdatpath, "rb");
	FILE* imgpic = fopen(imgpicpath, "rb");
	if (imgmeta == NULL || imgpic == NULL)
	{
		imgmeta = fopen(imgdatpath, "wb");
		imgpic = fopen(imgpicpath, "wb");
		
		FileFormat = file_determine_format_ptr(unknownfile);
		if (FileFormat == Format_None)
		{
			//TODO: Put error here
		}
		else if (FileFormat == Format_Wav)
		{
			fwrite(nodatanl, strlen(nodatanl), 1, imgpic);
			fwrite(nodatanl, strlen(nodatanl), 1, imgpic);
			fwrite(nodatanl, strlen(nodatanl), 1, imgpic);
			fwrite(nodata, strlen(nodata), 1, imgpic);
			meta->picdata = NULL;
			meta->name = file_extract_path_fname(unknownfile_path);
			meta->artist = nodata;
			meta->album = nodata;
		}
		else if (FileFormat == Format_Flac)
		{
			
		}
		else if (FileFormat == Format_Mp3)
		{
			
		}
	}
	else
	{
		
	}
	
	fclose(imgmeta);
	fclose(imgpic);
	fclose(unknownfile);
	
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

char** file_list_dir()
{
	int dirsize = file_get_dir_size();
	char** listings = malloc(dirsize * sizeof(char *));
	struct dirent* listing;
	
	for (int i = 0; i < dirsize; i++)
	{
		listing = readdir(curdir);
		listings[i] = malloc(strlen(listing->d_name) + 1);
		strcpy(listings[i], listing->d_name);
		listings[i][strlen(listing->d_name)] = 0x00;
	}
	
	file_reset_dir();
	return listings;
}

void file_free_dir_list(char** list, int length)
{
	for (int i = 0; i < length; i++)
	{
		free(list[i]);
	}
	free(list);
}

FileFormat file_determine_format_path(char* file_path)
{
	FILE* target_file = fopen(file_path, "rb");
	u32 file_magic;
	fread(&file_magic, 4, 1, target_file);
	fclose(target_file);
	
	if (file_magic == 0x52494646)
	{
		return Format_Wav;
	}
	else if (file_magic == 0x664C6143)
	{
		return Format_Flac;
	}
	else if (file_magic == 0x494433 || file_magic == 0xFFFB)
	{
		return Format_Mp3;
	}
	else
	{
		return Format_None;
	}
}

FileFormat file_determine_format_ptr(FILE* file_ptr)
{
	FILE* target_file = file_ptr;
	long int oldpos = ftell(target_file);
	fseek(target_file, 0, SEEK_SET);
	u32 file_magic;
	fread(&file_magic, 4, 1, target_file);
	fseek(target_file, oldpos, SEEK_SET);
	
	if (file_magic == 0x52494646)
	{
		return Format_Wav;
	}
	else if (file_magic == 0x664C6143)
	{
		return Format_Flac;
	}
	else if (file_magic == 0x494433 || file_magic == 0xFFFB)
	{
		return Format_Mp3;
	}
	else
	{
		return Format_None;
	}
}

void file_exit()
{
	closedir(curdir);
}