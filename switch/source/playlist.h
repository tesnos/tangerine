#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#include "player.h"

//Errors that may be returned by playlist_validate
typedef enum
{
	PLERR_NONE = 0x0,
	PLERR_INVALID_PATHS = 0x101,
	PLERR_EXTM3U_NOT_SUPPORTED = 0x102,
	PLERR_DIR_NOT_SUPPORTED = 0x103,
	PLERR_HTTP_NOT_SUPPORTED = 0x104,
	PLERR_TOO_LONG = 0x105
} PlaylistError;

//playlist things
void playlist_clear();

void playlist_append(char* path);

int playlist_begin();

int playlist_play_file(char* path);

int playlist_play_list(char* path);

void playlist_play_previous();

void playlist_play_next();

int count_lines(FILE* targetfile);

void get_lines(FILE* targetfile, char** linestorage, int linecount);

int playlist_validate(FILE* unknownpl);

void playlist_populate(FILE* validpl);

void playlist_init();

void playlist_exit();