#include "gui.h"

//Buffer for printing a string to the screen
char str[256];

const uint32_t clear_color_top = col_white;
const uint32_t clear_color_bot = col_white;

gfxScreen_t targetscreen;

char** entrytable;
int* dirposptr;
int entrytablelen;
bool drawing = false;

void gui_init(char** entrytableptr, int* dirposptrinput)
{
	entrytablelen = *dirposptrinput;
	entrytable = entrytableptr;
	dirposptr = dirposptrinput;
	
	//Initialize graphics library
	pp2d_init();
	
	//Configure default colors for top/bottom screens and disable 3D
	pp2d_set_screen_color(GFX_TOP, clear_color_top);
	pp2d_set_screen_color(GFX_BOTTOM, clear_color_bot);
	
	pp2d_load_texture_png(0, "romfs:/orangetest.png");
	pp2d_load_texture_png(1, "romfs:/play.png");
	
	pp2d_set_3D(1);
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

void gui_draw_progress(int percent)
{
	pp2d_draw_rectangle(41, 161, (int) (238 * (percent / 100.0)), 5, col_base);
}

void gui_prepare_frame(gfxScreen_t target, gfx3dSide_t side)
{
	targetscreen = target;
	//Begin drawing to whatever place
	if (!drawing)
	{
		pp2d_begin_draw(targetscreen, side);
		drawing = true;
	}
	else
	{
		pp2d_draw_on(targetscreen, side);
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
			
			gui_printc(0, 20, col_black, entrytable[*dirposptr]);
			if (*dirposptr < entrytablelen - 1)
			{
				gui_printc(20, 40, col_black, entrytable[*dirposptr + 1]);
			}
			if (*dirposptr < entrytablelen - 2)
			{
				gui_printc(40, 60, col_black, entrytable[*dirposptr + 2]);
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
	pp2d_exit();
}