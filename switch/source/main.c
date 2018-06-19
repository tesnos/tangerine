#include <switch.h>

#include "graphics/gui.h"
#include "player.h"

int main(int argc, char** argv)
{
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
		gui_endframe();
		
		player_updateaudio();
	}
	
	
	player_exit();
	gui_exit();
	return 0;
}