#include "gui.h"

//Buffer for printing a string to the screen
char str[256];

const uint32_t clear_color_top = col_white;
const uint32_t clear_color_bot = col_white;

gfxScreen_t targetscreen;

char** entrytable;
int* dirposptr;
bool drawing = false;

void gui_init(char** entrytableptr, int* dirposptrinput)
{
	//Initialize graphics library
	pp2d_init();
	
	entrytable = entrytableptr;
	dirposptr = dirposptrinput;
	
	//Configure default colors for top/bottom screens and disable 3D
	pp2d_set_screen_color(GFX_TOP, clear_color_top);
	pp2d_set_screen_color(GFX_BOTTOM, clear_color_bot);
	
	pp2d_load_texture_png(0, "orangetest.png");
	pp2d_load_texture_png(1, "play.png");
	
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
	pp2d_draw_text_wrap(x, y, 1, 1, color, 320, str);
}

void gui_printc(float x, float y, u32 color, char* value)
{
	pp2d_draw_text_wrap(x, y, 1, 1, color, 320, value);
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
			pp2d_draw_rectangle(0, 0, 400, 24, col_orange);
		}
		
		if (state == 0)
		{
			//Upper bar
			pp2d_draw_rectangle(0, 0, 400, 24, col_orange);
			gui_printc(0, 0, col_black, entrytable[*dirposptr]);
		}
	}
	else if (targetscreen == GFX_BOTTOM)
	{
		if (state == 1)
		{
			//Lower bar
			pp2d_draw_rectangle(0, 192, 320, 48, col_orange);
		}
		
		if (state == 0)
		{
			//Fast Forward
			pp2d_draw_texture_scale(1, 145, 195, 0.4f, 0.4f);
			
			//Lower bar
			pp2d_draw_rectangle(0, 192, 320, 48, col_orange);
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