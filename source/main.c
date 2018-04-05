#include <string.h>
#include <stdbool.h>

#include "filemanage.h"
#include "gui.h"
//#include "player.h"
#include "playlist.h"

//The possible application states
typedef enum
{
	STATE_SELECTION = 0,
	STATE_PLAYING = 1
} AppState;

//Types of files to select
typedef enum
{
	SELTYPE_AUDIO = 0,
	SELTYPE_PLAYLIST = 1
} SelectionType;


//Exit normally and clean everything up
void cleanexit()
{
	//Clean up internally
	playlist_exit();
	gui_exit();
	endread();
	
	//Properly exit previously initialized services
	hidExit();
	romfsExit();
}


//Exit for when an error is encountered
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
		
		gui_printc(20, 20, col_err, "Error: ");
		gui_printi(60, 20, col_err, errcode);
		
		gui_finish_frame();
	}
	
	cleanexit();
}


//Skips to the next audio file
void skip(char** direntries, int* dirpos, int dirsize)
{
	ceaseplayback();
	
	*dirpos += 1;
	
	if (*dirpos >= dirsize)
	{
		*dirpos = 0;
	}
	
	char fileloc[256] = "";
	strcat(fileloc, getcurdir());
	strcat(fileloc, "/");
	strcat(fileloc, direntries[*dirpos]);
	int beginplay = playlist_play_file(fileloc);
	
	if (beginplay > 0)
	{
		errexit(beginplay);
	}
}


//Skips to the previous audio file
void prev(char** direntries, int* dirpos, int dirsize)
{
	ceaseplayback();
	
	*dirpos -= 1;
	
	if (*dirpos < 0)
	{
		*dirpos = dirsize - 1;
	}
	
	char fileloc[256] = "";
	strcat(fileloc, getcurdir());
	strcat(fileloc, "/");
	strcat(fileloc, direntries[*dirpos]);
	int beginplay = playlist_play_file(fileloc);
	
	if (beginplay > 0)
	{
		errexit(beginplay);
	}
}


int main()
{
	//External initializations (libctru services)
	hidInit();
	romfsInit();
	
	//Internal initializations
	files_init();
	player_init();
	playlist_init();
	
	//Don't stop when the 3DS is closed
	aptSetSleepAllowed(false);
	
	
	//These keep track of the state of application
	int appstate = STATE_SELECTION;
	int selectiontype = SELTYPE_AUDIO;
	bool* playing = get_playing_handle();
	
	//Holds which keys are down
	u32 kDown = 0;
	//For use with touch screen input
	touchPosition touch;
	int touchx;
	int touchy;
	int touchdelay = 0;
	
	
	//Get number of music files in directory
	int dirsize = getdirsize(selectiontype);
	char* direntries[dirsize];
	//Create an int to hold where we are in the directory and give it and the pointer to our table
	//to the gui + tell the gui how long the table is.
	int dirpos = 0;
	gui_init(direntries, &dirpos);
	gui_set_table_length(dirsize);
	//Fill up the entries table with the filenames
	buildentries(direntries, getalldirsize(), selectiontype);
	
	
	//Main loop of program
	while (aptMainLoop())
	{
		//-----------------------
		//       GRAPHICS
		//-----------------------
		
		//Draw the static elements of this frame's graphics
		gui_prepare_frame(GFX_TOP, GFX_LEFT);
		gui_draw_frame(appstate);
		
		gui_prepare_frame(GFX_BOTTOM, GFX_LEFT);
		gui_draw_frame(appstate);
		
		//Keeps track of audio progress
		if (appstate == STATE_PLAYING)
		{
			int audioprogress = play_audio();
			if (audioprogress == 100)
			{
				ceaseplayback();
				if (*playing)
				{
					toggle_playback();
				}
				appstate = 0;
			}
			gui_draw_progress(audioprogress);
		}
		
		//Differences between audio and playlist selection
		if (appstate == STATE_SELECTION)
		{
			gui_draw_selection(selectiontype);
		}
		
		//Draw play/pause button
		gui_draw_play(*playing);
		
		//Stop drawing graphics for this frame
		gui_finish_frame();
		
		
		
		//-----------------------
		//    INPUT HANDLING
		//-----------------------
		
		//Scan input
		hidScanInput();
		
		//Read the touch screen coordinates
		hidTouchRead(&touch);
		touchx = touch.px;
		touchy = touch.py;
		
		//Get what buttons are being pressed
		kDown = hidKeysDown();
		
		//If the start key is pressed, exit to the hbmenu
		if (kDown & KEY_START){
			break;
		}
		
		//Input handler if statements
		//Handle audio file selection
		if (appstate == STATE_SELECTION)
		{
			//Check to see if touch position is on the play button or the a button is pressed
			bool touchvalid = (touchx > 140 && touchx < 180) && (touchy > 195 && touchy < 240) && (touchdelay == 0);
			bool apressed = (kDown & KEY_A);
			
			if ((apressed || touchvalid) && (dirsize > 0))
			{
				//Combine the current directory, a forward slash, and the selected file's name to get
				//the path to try to begin playing from, and switch the application state
				char fileloc[256] = "";
				strcat(fileloc, getcurdir());
				strcat(fileloc, "/");
				strcat(fileloc, direntries[dirpos]);
				int beginplay = 0;
				
				if (selectiontype == SELTYPE_AUDIO) { beginplay = playlist_play_file(fileloc); appstate = STATE_PLAYING; }
				if (selectiontype == SELTYPE_PLAYLIST) { beginplay = playlist_play_list(fileloc); }
				
				if (beginplay > 0) { errexit(beginplay); }
			}
			
			if(touchvalid) { touchdelay = 15; }
			
			
			//Move the selector up and down the list
			if (kDown & KEY_DDOWN)
			{
				if (dirpos < dirsize - 1) { dirpos++; }
			}
			else if (kDown & KEY_DUP)
			{
				if (dirpos > 0)	{ dirpos--; }
			}
			
			if (kDown & KEY_L && selectiontype == SELTYPE_PLAYLIST)
			{
				selectiontype = SELTYPE_AUDIO;
				dirpos = 0;
				dirsize = getdirsize(selectiontype);
				gui_set_table_length(dirsize);
				buildentries(direntries, getalldirsize(), selectiontype);
			}
			if (kDown & KEY_R && selectiontype == SELTYPE_AUDIO)
			{
				selectiontype = SELTYPE_PLAYLIST;
				dirpos = 0;
				dirsize = getdirsize(selectiontype);
				gui_set_table_length(dirsize);
				buildentries(direntries, getalldirsize(), selectiontype);
			}
		}
		
		//Handle audio playback
		else if (appstate == STATE_PLAYING)
		{
			//Playback controls : Pause/Play
			if (kDown & KEY_A)
			{
				toggle_playback();
			}
			
			//Playback controls : Stop and return to selection
			if (kDown & KEY_B)
			{
				appstate = 0;
				ceaseplayback();
				if (*playing)
				{
					toggle_playback();
				}
			}
			
			//Playback controls : Previous
			if (kDown & KEY_L)
			{
				prev(direntries, &dirpos, dirsize);
				if (!*playing)
				{
					toggle_playback();
				}
			}
			
			//Playback controls : Next
			if (kDown & KEY_R)
			{
				skip(direntries, &dirpos, dirsize);
				if (!*playing)
				{
					toggle_playback();
				}
			}
			
			
			//Touch input
			if ((kDown & KEY_TOUCH) && (touchdelay == 0))
			{
				//Playback controls : Pause/Play (touch)
				if ((touchx > 140 && touchx < 180) && (touchy > 195 && touchy < 240))
				{
					toggle_playback();
					touchdelay = 15;
				}
				
				//Playback controls : Seek
				if ((touchx > 40 && touchx < 280) && (touchy > 155 && touchy < 170))
				{
					seekaudio(((touchx - 41) / 238.0) * 100);
					touchdelay = 15;
				}
			}
		}
		
		//Constantly decrement touch input delay counter
		if (touchdelay > 0)	{ touchdelay--; }
	}
	
	cleanexit();

	return 0;
}