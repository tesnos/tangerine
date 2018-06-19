#include "gui.h"

//Buffer for printing a string to the screen
char str[256];

const uint32_t clear_color_top = col_white;
const uint32_t clear_color_bot = col_white;

C3D_RenderTarget* topscreen;
C3D_RenderTarget* botscreen;

C3D_RenderTarget* targetscreen;

char** entrytable;
int* dirposptr;
size_t entrytablelen;
bool drawing = false;

void gui_init(char** entrytableptr, int* dirposptrinput)
{
	entrytable = entrytableptr;
	dirposptr = dirposptrinput;
	
	//Initialize graphics library
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	
	topscreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	botscreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	
	//Configure default colors for top/bottom screens and disable 3D
	pp2d_set_screen_color(GFX_TOP, clear_color_top);
	pp2d_set_screen_color(GFX_BOTTOM, clear_color_bot);
	
	pp2d_load_texture_png(0, "romfs:/orangetest.png");
	pp2d_load_texture_png(1, "romfs:/play.png");
	
	pp2d_set_3D(1);
}

void gui_set_table_length(size_t length)
{
	entrytablelen = length;
}

void gui_draw_play(bool playing)
{
	if (!playing)
	{
		pp2d_draw_texture_scale(1, 145, 195, 0.4f, 0.4f);
	}
	else
	{
		pp2d_draw_rectangle(145, 195, 10, 40, col_white);
		pp2d_draw_rectangle(160, 195, 10, 40, col_white);
	}
}

void gui_draw_progress(float percent)
{
	pp2d_draw_rectangle(41, 161, (int) (238 * (percent / 100)), 5, col_base);
}

void gui_prepare_frame(DrawScreen target)
{
	//Begin drawing to whatever place
	if (!drawing)
	{
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(topscreen, clear_color_top);
		C2D_TargetClear(botscreen, clear_color_bot);
		drawing = true;
	}
	
	if (target == SCREEN_TOP)
	{
		C2D_SceneBegin(topscreen);
		targetscreen = topscreen;
	}
	else if (target == SCREEN_BOT)
	{
		C2D_SceneBegin(botscreen);
		targetscreen = botscreen;
	}
	
	//And then clear it (top and bottom screen are different sizes, 400 and 320 respectively)
	if (targetscreen == GFX_TOP)
	{
		pp2d_draw_rectangle(0, 0, 400, 240, clear_color_top);
	}
	if (targetscreen == GFX_BOTTOM)
	{
		pp2d_draw_rectangle(0, 0, 320, 240, clear_color_bot);
	}
}

void gui_draw_selection(int selectiontype)
{
	if (selectiontype == 0) //SELTYPE_AUDIO
	{
		gui_printc(235, 0, col_grey, "R: Playlists");
	}
	else if (selectiontype == 1) //SELTYPE_PLAYLIST
	{
		gui_printc(0, 0, col_grey, "L: Audio Files");
	}
}

void gui_printi(float x, float y, u32 color, int value)
{
	//Convert whatever value given into a char array
	snprintf(str, sizeof(str), "%zu", value);
	//and draw it to the screen
	pp2d_draw_text_wrap(x, y, 0.6f, 0.6f, color, 320, str);
}

void gui_printc(float x, float y, u32 color, char* value)
{
	if (strlen(value) > 39)
	{
		void* shortentry = malloc(40);
		strncpy(shortentry, value, 36);
		strncpy(shortentry + 36, "...", 3);
		strncpy(shortentry + 39, "\0", 1);
		
		pp2d_draw_text_wrap(x, y, 0.6f, 0.6f, color, 320, shortentry);
	}
	else
	{
		pp2d_draw_text_wrap(x, y, 0.6f, 0.6f, color, 320, value);
	}
}

void gui_draw_frame(int state)
{
	if (!drawing)
	{
		return;
	}
	if (targetscreen == GFX_TOP)
	{
		if (state == 1)
		{
			//Logo
			pp2d_draw_texture(0, 112, 32);
			
			//Upper bar
			pp2d_draw_rectangle(0, 0, 400, 24, col_base);
		}
		
		if (state == 0)
		{
			//Logo
			pp2d_draw_texture(0, 112, 32);
			
			//Upper bar
			pp2d_draw_rectangle(0, 0, 400, 24, col_base);
		}
	}
	else if (targetscreen == GFX_BOTTOM)
	{
		if (state == 1)
		{
			//Lower bar
			pp2d_draw_rectangle(0, 192, 320, 48, col_base);
			
			//Progress bar
			pp2d_draw_rectangle(40, 160, 240, 7, col_lightgrey);
			
			//Next
			pp2d_draw_texture_scale(1, 232, 195, 0.4f, 0.4f);
			pp2d_draw_rectangle(266, 195, 10, 40, col_white);
			
			//Previous
			pp2d_texture_select(1, 54, 195);
			pp2d_texture_flip(HORIZONTAL);
			pp2d_texture_scale(0.4f, 0.4f);
			pp2d_texture_draw();
			pp2d_draw_rectangle(44, 195, 10, 40, col_white);
		}
		
		if (state == 0)
		{
			//Lower bar
			pp2d_draw_rectangle(0, 192, 320, 48, col_base);
			
			//File selection indicator
			pp2d_draw_rectangle(0, 20, 320, 20, col_lightgrey);
			
			if (entrytablelen == 0) {
				gui_printc(0, 20, col_black, "No music found.");
			}
			else
			{
				gui_printc(0, 20, col_black, entrytable[*dirposptr]);
				
				if (*dirposptr < entrytablelen - 1 && entrytablelen > 1)
				{
					gui_printc(20, 40, col_black, entrytable[*dirposptr + 1]);
				}
				if (*dirposptr < entrytablelen - 2 && entrytablelen > 2)
				{
					gui_printc(40, 60, col_black, entrytable[*dirposptr + 2]);
				}
			}
		}
	}
}

void gui_finish_frame()
{
	drawing = false;
	pp2d_end_draw();
}

void gui_exit()
{
	C3D_Fini();
	C2D_Fini();
	gfxExit();
}