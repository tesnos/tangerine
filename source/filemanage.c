#include "filemanage.h"

DIR* dirstream;

char defaultdir[7] = "/music\0";
char* curdir = defaultdir;
int tolddir = 0;

void files_init()
{
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

int getdirsize()
{
	int i = 0;
	while (streamentry() != NULL)
	{
		i++;
	}
	resetdir();
	return i;
}


//ctrulib stubs the real rewinddir, so ¯\_(ツ)_/¯
void resetdir()
{
	endread();
	startread(curdir);
}

int isdirectory(char* filepath)
{
	struct stat path_stat;
    stat(filepath, &path_stat);
	if (S_ISDIR(path_stat.st_mode) == 0)
	{
		return 1;
	}
	
	return 0;
}

void buildentries(char** entryarray, int length)
{
	resetdir();
	char* entry;
	for(int i = 0; i < length; i++)
	{
		entry = readdir(dirstream)->d_name;
		void* entrydest = malloc(strlen(entry) + 1);
		strncpy(entrydest, entry, strlen(entry) + 1);
		entryarray[i] = entrydest;
	}
}