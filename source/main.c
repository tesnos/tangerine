#include <string.h>
#include <stdbool.h>

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
	bool* playing = playerInit();
	files_init();
	
	//Holds which keys are down/up
	u32 kDown = 0;
	u32 kUp = 0;
	u32 kprevDown = 0;
	u32 kprevUp = 0;
	
	touchPosition touch;
	int touchx;
	int touchy;
	int touchdelay = 0;
	
	int audioprogress;
	
	int dirsize = getdirsize();
	char* direntries[dirsize];
	int dirpos = dirsize;
	gui_init(direntries, &dirpos);
	dirpos = 0;
	buildentries(direntries, getalldirsize());
	
	int appstate = 0;
	
	//Main loop of program
	while (aptMainLoop())
	{
		//Scan input and put all keys down/up into kDown/kUp
		hidScanInput();
		
		//Read the touch screen coordinates
		hidTouchRead(&touch);
		touchx = touch.px;
		touchy = touch.py;
		
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
		
		gui_draw_frame(appstate);
		
		gui_prepare_frame(GFX_BOTTOM, GFX_LEFT);
		
		gui_draw_frame(appstate);
		
		if (appstate == 1)
		{
			audioprogress = play_audio();
			if (audioprogress == 100)
			{
				ceaseplayback();
				if (*playing)
				{
					toggle_playback();
				}
				appstate = 0;
			}
			gui_printi(0, 0, col_black, audioprogress);
			gui_draw_progress(audioprogress);
		}
		
		gui_draw_play(*playing);
		
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
			
			if ((kDown & KEY_TOUCH) && (touchdelay == 0))
			{
				if ((touchx > 140 && touchx < 180) && (touchy > 195 && touchy < 240))
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
					touchdelay = 20;
				}
			}
		}
		else if (appstate == 1)
		{
			//Playback controls
			if (kDown & KEY_A)
			{
				toggle_playback();
			}
			
			if (kDown & KEY_B)
			{
				appstate = 0;
				ceaseplayback();
				if (*playing)
				{
					toggle_playback();
				}
			}
			
			if ((kDown & KEY_TOUCH) && (touchdelay == 0))
			{
				if ((touchx > 140 && touchx < 180) && (touchy > 195 && touchy < 240))
				{
					toggle_playback();
					touchdelay = 20;
				}
				
				if ((touchx > 40 && touchx < 280) && (touchy > 155 && touchy < 170))
				{
					seekaudio(((touchx - 41) / 238.0) * 100);
					touchdelay = 15;
				}
			}
		}
		
		if (touchdelay > 0)
		{
			touchdelay--;
		}
	}
	
	cleanexit();

	return 0;
}