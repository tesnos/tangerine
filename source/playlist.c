#include "playlist.h"

char* list[128];
int listpos = 0;
int listsize = 0;

void playlist_clear()
{
	listpos = 0;
	listsize = 0;
}

void playlist_append(char* path)
{
	list[listsize] = path;
	listsize++;
}

int playlist_begin()
{
	listpos = 0;
	return playfile(list[listpos]);
}

int playlist_play_file(char* path)
{
	playlist_clear();
	playlist_append(path);
	return playlist_begin();
}

int playlist_play_list(char* path)
{
	FILE* m3ufile = fopen(path, "r");
	int validity = playlist_validate(m3ufile);
	if (validity != PLERR_NONE) { return validity; } else { svcOutputDebugString("yo", strlen("yo")); }
	//playlist_populate();
	//return playlist_begin();
	return 0;
}

int count_lines(FILE* targetfile)
{
	fseek(targetfile, 0, SEEK_SET);
	
	int linecount = 0;
	char charbuf[1];
	
	while(1) {
		size_t n = fread(charbuf, 1, 1, targetfile);

		if (charbuf[0] == '\n')
		{
			linecount++;
		}
		if (n != 1) {
			linecount++;
			break;
		}
	}
	
	return linecount;
}

void get_lines(FILE* targetfile, char** linestorage, int linecount)
{
	fseek(targetfile, 0, SEEK_SET);
	
	char charbuf[1];
	char linebuf[256];
	for (int j = 0; j < linecount; j++)
	{
		size_t n = fread(linebuf, 1, 256, targetfile);
		
		if (j == linecount - 1)
		{
			char lastline[n + 1];
			strncpy(lastline, linebuf, n);
			lastline[n] = '\0';
			svcOutputDebugString(lastline, n);
			linestorage[j] = lastline;
			break;
		}
		
		for (int i = 0; i < n; i++)
		{
			charbuf[0] = linebuf[i];
			
			if (charbuf[0] == '\n')
			{
				char* line = malloc(i + 1);
				strncpy(line, linebuf, i);
				line[i] = '\0';
				linestorage[j] = line;
				int offset = -(n - i) + 1;
				svcOutputDebugString(line, i);
				fseek(targetfile, offset, SEEK_CUR);
				break;
			}
		}
	}
}

int playlist_validate(FILE* unknownpl)
{
	int pl_length = count_lines(unknownpl);
	char* pl_lines[pl_length];
	get_lines(unknownpl, pl_lines, pl_length);
	//char str[256];
	//snprintf(str, sizeof(str), "%zi", pl_length);
	//svcOutputDebugString(str, strlen(str));
	for (int i = 0; i < pl_length; i++)
	{
		//svcOutputDebugString(pl_lines[i], strlen(pl_lines[i]));
		//Check if playlist tries to load from internet
		if (strstr(pl_lines[i], "http://") != NULL || strstr(pl_lines[i], "https://") != NULL)
		{
			return PLERR_HTTP_NOT_SUPPORTED;
		}
		//Check to make sure playlist destinations are all files
		if (strstr(pl_lines[i], ".") == NULL)
		{
			return PLERR_DIR_NOT_SUPPORTED;
		}
		//Check to make sure this playlist is not extm3u
		if (strstr(pl_lines[i], "#EXTM3U") != NULL)
		{
			return PLERR_EXTM3U_NOT_SUPPORTED;
		}
		//Check to make sure files exist
		if (access(pl_lines[i], F_OK) == -1)
		{
			return PLERR_INVALID_PATHS;
		}
	}
	return PLERR_NONE;
}

void playlist_init()
{
	playlist_clear();
}

void playlist_exit()
{
	player_exit();
}