#include "gui.h"

u32 screenwidth, screenheight;
struct Graphics_Image* testimg;
struct Graphics_Image* noartimg;
struct Graphics_Image* exitimg;
struct Graphics_Image* settingsimg;

void gui_init()
{
	graphics_init();
	testimg = graphics_loadpng("/bluegrad.png");
	noartimg = graphics_loadpng("/noart.png");
	exitimg = graphics_loadpng("/exit.png");
	//settingsimg = graphics_loadpng("/settings.png");
}

void gui_fillscreen(u32 fillcolor)
{
	graphics_draw_rect(0, 0, screenwidth, screenheight, fillcolor);
}

void gui_clearscreen()
{
	gui_fillscreen(CLEAR_COL);
}

void gui_draw_header()
{
	graphics_draw_rect(0, 0, screenwidth, screenheight / 16, COL_ORANGE);
	graphics_draw_image_scaled(5, 4, exitimg, 0.0625);
}

void gui_beginframe()
{
	graphics_beginframe();
	screenwidth = graphics_get_width();
	screenheight = graphics_get_height();
	gui_clearscreen();
	gui_draw_header();
}

void gui_drawframe(int appstate)
{
	if (appstate == 0)
	{
		graphics_draw_rect(100, 100, 100, 50, 0xFF00FFFF);
		graphics_draw_text(50, 25, 0xFFFFFFFF, "+");
	}
}

void gui_endframe()
{
	graphics_endframe();
}

void gui_blankframe()
{
	gui_beginframe();
	gui_endframe();
}

void gui_exit()
{
	graphics_freeimage(testimg);
	graphics_freeimage(noartimg);
	graphics_freeimage(exitimg);
	//graphics_freeimage(settingsimg);
	graphics_exit();
}