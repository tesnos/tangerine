#include <string.h>

#include "gui.h"
#include "player.h"
#include "filemanage.h"

void cleanexit()
{
	//Clean up in the player
	exitplayer();
	
	//Properly exit all previously initialized libraries/services
	gui_exit();
	hidExit();
}

void errexit(int errcode)
{
	u32 kDown;
	
	while (1)
	{
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_START){
			break;
		}
	
		gui_prepare_frame(GFX_TOP, GFX_LEFT);
		
		gui_printi(20, 20, col_black, errcode);
		
		gui_finish_frame();
	}
	
	cleanexit();
}

int main()
{
	//Initialize the hid (human interface device (controls)), player (handles audio), and pp2d (graphics)
	hidInit();
	playerInit();
	files_init();
	
	//Holds which keys are down/up
	u32 kDown = 0;
	u32 kUp = 0;
	u32 kprevDown = 0;
	u32 kprevUp = 0;
	
	int buffersfilled = 0;
	
	int dirsize = getdirsize();
	char* direntries[dirsize];
	int dirpos = 0;
	gui_init(direntries, &dirpos);
	buildentries(direntries, dirsize);
	
	int appstate = 0;
	
	//Main loop of program
	while (aptMainLoop())
	{
		//Scan input and put all keys down/up into kDown/kUp
		hidScanInput();
		kprevDown = kDown;
		kprevUp = kUp;
		kDown = hidKeysDown();
		kUp = hidKeysUp();
		
		//If the start key is pressed, exit to the hbmenu
		if (kDown & KEY_START){
			break;
		}
		
		//Start drawing this frame's graphics
		gui_prepare_frame(GFX_TOP, GFX_LEFT);
		
		//gui_printc(0, 0, col_black, direntries[dirpos]);
		//gui_printi(0, 40, col_black, dirsize);
		//gui_printi(0, 80, col_black, dirpos);
		
		gui_draw_frame(appstate);
		
		gui_prepare_frame(GFX_BOTTOM, GFX_LEFT);
		
		gui_draw_frame(appstate);
		
		buffersfilled = play_audio();
		
		gui_printi(0, 120, col_black, buffersfilled);
		
		//Finish drawing graphics
		gui_finish_frame();
		
		if (appstate == 0)
		{
			//Selector
			if (kDown & KEY_A)
			{
				char fileloc[256] = "";
				strcat(fileloc, getcurdir());
				strcat(fileloc, "/");
				strcat(fileloc, direntries[dirpos]);
				int beginplay = playfile(fileloc);
				appstate = 1;
				if (beginplay > 0)
				{
					errexit(beginplay);
				}
			}
			
			if (kDown & KEY_DDOWN)
			{
				if (dirpos < dirsize - 1)
				{
					dirpos++;
				}
			}
			else if (kDown & KEY_DUP)
			{
				if (dirpos > 0)
				{
					dirpos--;
				}
			}
		}
		else if (appstate == 1)
		{
			//Playback controls
			if (kDown & KEY_A){
				toggle_playback();
			}
		}
	}
	
	cleanexit();

	return 0;
}