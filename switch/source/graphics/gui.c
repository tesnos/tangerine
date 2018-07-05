#include "gui.h"

u32 screenwidth, screenheight;
struct Graphics_Image* testimg;
struct Graphics_Image* noartimg;
struct Graphics_Image* exitimg;
struct Graphics_Image* settingsimg;

int pulsepos; bool pulsesub;
u32 gui_selection_pulse_color(void)
{
	if (pulsepos >= 8)
	{
		pulsesub = true;
	}
	else if (pulsepos <= -8)
	{
		pulsesub = false;
	}
	
	int mapped_pulse_value = pulsepos * 0x800;
	
	if (pulsesub) { pulsepos--; }
	else { pulsepos++; }
	
	return (COL_PULSE_BASE + mapped_pulse_value);
}

void gui_init()
{
	graphics_init();
	testimg = graphics_loadpng("/bluegrad.png");
	noartimg = graphics_loadpng("/noart.png");
	exitimg = graphics_loadpng("/exit.png");
	//settingsimg = graphics_loadpng("/settings.png");
	
	pulsepos = -8;
	pulsesub = false;
}

void gui_fillscreen(u32 fillcolor)
{
	graphics_draw_rect(0, 0, screenwidth, screenheight, fillcolor);
}

void gui_clearscreen()
{
	gui_fillscreen(graphics_get_theme_color());
}

void gui_draw_header()
{
	graphics_draw_rect(0, 0, screenwidth, screenheight / 16, COL_ORANGE);
	graphics_draw_image_scaled(5, 4, exitimg, 0.0625);
}

void gui_draw_file_boxes(int dirsize, TrackMetadata** metadb, int selectedbox, int dboffset)
{
	int numboxes = ((dirsize > 6) ? 6 : dirsize);
	for (int i = 0; i < numboxes; i++)
	{
		if (i != selectedbox)
		{
			graphics_draw_rect_outlined((screenwidth/4) * 3, screenheight/16 + ((screenheight/6) * i), screenwidth/4, screenheight/6, graphics_get_theme_color(), COL_LIGHTGREY, 2);
		}
		else
		{
			graphics_draw_rect_outlined((screenwidth/4) * 3, screenheight/16 + ((screenheight/6) * i), screenwidth/4, screenheight/6, graphics_get_theme_color(), gui_selection_pulse_color(), 4);
		}
		//gui_draw_file_box_data(i, metadb[dboffset + 1]);
	}
}

void gui_draw_file_box_data(int position, TrackMetadata* meta)
{
	struct Graphics_Image* metapic = NULL;
	
	if (meta->picdata[0] != (long) NULL)
	{
		metapic->width = 500;
		metapic->height = 500;
		metapic->ptr = meta->picdata;
	}
	else
	{
		metapic = noartimg;
	}
	
	graphics_draw_image_scaled((screenwidth/4) * 3.0625, screenheight/16 + ((screenheight/6) * (position + 0.2)), metapic, 0.0625);
	graphics_draw_text((screenwidth/4) * 3.3125, screenheight/16 + ((screenheight/6) * (position + 0.4)), graphics_get_theme_color_font(), meta->name);
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
		graphics_draw_text(50, 18, 0xFFFFFFFF, "+");
		
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