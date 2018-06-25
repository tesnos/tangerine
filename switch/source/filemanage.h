#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include "md5.h"
#include "common.h"

void file_init(void);

void file_reset_dir(void);

char* file_extract_path_fname(char* filepath);

TrackMetadata file_check_data(char* unknownfile_path);

int file_get_dir_size(void);

char* file_change_dir_up(void);

void file_change_dir(char* dir_path);

char** file_list_dir(void);

void file_free_dir_list(char** list, int length);

FileFormat file_determine_format_path(char* file_path);
FileFormat file_determine_format_ptr(FILE* file_ptr);

void file_exit(void);