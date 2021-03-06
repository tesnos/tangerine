#include <dirent.h>
#include <malloc.h>
#include <math.h>

#include "md5.h"
#include "common.h"
#include "formats/wav.h"
#include "formats/flac.h"
#include "formats/mp3.h"

void file_init(void);

void file_reset_dir(void);

char* file_concat_path_curdir(char* filepath);

char* file_extract_path_fname(char* filepath);

int file_get_size(FILE* unknownfile);

char* file_get_next_line(FILE* textfile);

TrackMetadata* file_check_data(char* unknownfile_path);

int file_get_dir_size(void);

char* file_change_dir_up(void);

void file_change_dir(char* dir_path);

void file_list_dir(char** listings);

void file_list_dir_tracks(char** track_listings);

void file_free_dir_list(char** list, int length);

FileFormat file_determine_format_ptr(FILE* file_ptr);
FileFormat file_determine_format_path(char* file_path);

int file_get_dir_size_tracks(void);

void file_exit(void);