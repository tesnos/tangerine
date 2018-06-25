#include <switch.h>

#include "graphics/gui.h"
#include "player.h"
#include "filemanage.h"

int main(int argc, char** argv)
{
	file_init();
	gui_init();
	player_init();
	
	player_loadfile("/test.wav");
	
	
	while (appletMainLoop())
	{
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		if (kDown & KEY_PLUS) break;
		
		gui_beginframe();
		gui_drawframe(0);
		gui_draw_file_boxes(2);
		gui_endframe();
		
		player_updateaudio();
	}
	
	
	player_exit();
	gui_exit();
	file_exit();
	return 0;
}