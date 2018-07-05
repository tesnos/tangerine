#include <stdbool.h>
#include <malloc.h>

#include "common.h"
#include "util.h"

void player_init(void);

void player_loadfile(char* filename);

void player_updatebuffer_thread(void* arg);

void player_updateaudio(void);

void player_exit(void);