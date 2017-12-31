#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void files_init(void);

void startread(char* dirpath);

struct dirent* streamentry(void);

char* getcurdir(void);

char* getentry(void);

void endread(void);

int getdirsize(void);

void resetdir(void);

int isdirectory(char* filepath);

void buildentries(char** entryarray, int length);

void endread(void);