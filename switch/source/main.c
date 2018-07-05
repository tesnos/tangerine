#include <stdbool.h>
#include <switch.h>

#include "graphics/gui.h"
#include "player.h"
#include "filemanage.h"
#include "util.h"

int dirsize, boxcount, selectedbox, boxoff;
TrackMetadata** metadb;
int loaded_metas; bool db_done_loading;
Thread* db_loader;

void free_metadb()
{
	for(int i = 0; i < dirsize; i++)
	{
		free(metadb[i]);
	}
	
	free(metadb);
}

void load_metadb()
{
	db_done_loading = false;
	metadb = malloc(dirsize * sizeof(TrackMetadata*));
	char* trackpaths[dirsize];
	file_list_dir_tracks(trackpaths);
	
	for (loaded_metas = 0; loaded_metas < dirsize; loaded_metas++)
	{
		metadb[loaded_metas] = file_check_data(trackpaths[loaded_metas]);
	}
	
	file_free_dir_list(trackpaths, dirsize);
	db_done_loading = true;
}


int main(int argc, char** argv)
{
	file_init();
	gui_init();
	player_init();
	
	//TrackMetadata* test = file_check_data("/test.wav\0");
	player_loadfile("/test.wav");
	
	dirsize = file_get_dir_size_tracks();
	boxcount = ((dirsize > 6) ? 6 : dirsize);
	boxoff = 0;
	selectedbox = 0;
	
	load_metadb();
	//db_loader = util_thread_func(load_metadb);
	
	
	while (appletMainLoop())
	{
		// if (db_done_loading)
		// {
			// threadClose(db_loader);
		// }
		
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		if (kDown & KEY_PLUS) break;
		
		gui_beginframe();
		gui_drawframe(0);
		gui_draw_file_boxes(boxcount, metadb, selectedbox, boxoff);
		gui_endframe();
		
		player_updateaudio();
	}
	
	
	player_exit();
	gui_exit();
	file_exit();
	//free_metadb();
	return 0;
}