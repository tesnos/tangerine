#include "filemanage.h"

DIR* dirstream;

char defaultdir[7] = "/music\0";
char* curdir = defaultdir;
int tolddir = 0;

void files_init()
{
	mkdir("/music", 777);
	startread(curdir);
}

void startread(char* dirpath)
{
	curdir = dirpath;
	dirstream = opendir(curdir);
}

struct dirent* streamentry()
{
	return readdir(dirstream);
}

char* getcurdir()
{
	return curdir;
}

void directory_go_into(char* dest)
{
	char deepdir[strlen(curdir) + 1 + strlen(dest) + 1];
	strcpy(deepdir, getcurdir());
	deepdir[strlen(curdir)] = '/';
	strcpy(deepdir + strlen(curdir) + 1, dest);
	deepdir[strlen(curdir) + strlen(dest) + 1] = '\0';
	
	curdir = deepdir;
	
	resetdir();
}

void directory_go_up()
{
	if (strcmp(getcurdir(), "/") == 0) { return; }
	
	for (int i = strlen(getcurdir()); i >= 0; i--)
	{
		if (getcurdir()[i] == '/')
		{
			char updir[i + 1];
			strncpy(updir, getcurdir(), i);
			updir[i] = '\0';
			
			curdir = updir;
		}
	}
	
	resetdir();
}

char* getentry()
{
	char* entry = readdir(dirstream)->d_name;
	void* entrydest = malloc(strlen(entry) + 1);
	strncpy(entrydest, entry, strlen(entry) + 1);
	return entrydest;
}

void endread()
{
	closedir(dirstream);
}

int recognizefiletype(FILE* unknownfile)
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
		return 0;
	}
	else if ((header[0] << 24) + (header[1] << 16) + (header[2] << 8) + (header[3]) == flac_magic)
	{
		return 1;
	}
	else if (((header[0] << 16) + (header[1] << 8) + (header[2]) == mp3_magic0) || ((header[0] << 8) + (header[1]) == mp3_magic1))
	{
		return 2;
	}
	
	return 3;
}

int getdirsize(int type)
{
	resetdir();
	int i = 0;
	struct dirent* direntry;
	direntry = streamentry();
	while (direntry != NULL)
	{
		if (type == 0)
		{
			if (detectfiletype(direntry->d_name) != 3) { i++; }
		}
		
		else if (type == 1)
		{
			if (strstr(direntry->d_name, ".m3u") != NULL) { i++; }
		}
		
		direntry = streamentry();
	}
	resetdir();
	return i;
}

int getalldirsize()
{
	int i = 0;
	struct dirent* direntry;
	direntry = streamentry();
	while (direntry != NULL)
	{
		i++;
		direntry = streamentry();
	}
	resetdir();
	return i;
}

int detectfiletype(char* filename)
{
	char path[256] = "/music";
	//strcat(path, getcurdir());
	strcat(path, "/");
	strcat(path, filename);
	FILE* paf = fopen(path, "rb");
	int ftype = recognizefiletype(paf);
	fclose(paf);
	return ftype;
}

void resetdir()
{
	endread();
	startread(curdir);
}

int isdirectory(char* path)
{
	struct stat path_stat;
    stat(path, &path_stat);
	if (S_ISDIR(path_stat.st_mode) == 0)
	{
		return 1;
	}
	
	return 0;
}

void buildentries(char** entryarray, int length, int entrytypes)
{
	resetdir();
	char* entry;
	int entered = 0;
	for(int i = 0; i < length; i++)
	{
		entry = readdir(dirstream)->d_name;
		if (entrytypes == 0 && detectfiletype(entry) != 3)
		{
			void* entrydest = malloc(strlen(entry) + 1);
			strncpy(entrydest, entry, strlen(entry) + 1);
			entryarray[entered] = entrydest;
			entered++;
		}
		else if (entrytypes == 1 && strstr(entry, ".m3u") != NULL)
		{
			void* entrydest = malloc(strlen(entry) + 1);
			strncpy(entrydest, entry, strlen(entry) + 1);
			entryarray[entered] = entrydest;
			entered++;
		}
	}
}