#include "graphics.h"
#include "../common.h"

//Format in hex is  0xAABBGGRR
#define CLEAR_COL       0xFFFFE0E0
#define COL_ORANGE      0xFF00A5FF
#define COL_BLUE        0xFFEE7129
#define COL_YELLOW      0xFF00FFFF
#define COL_MAGENTA     0xFFFF00FF
#define COL_CYAN        0xFFFFFF00
#define COL_WHITE       0xFFFFFFFF
#define COL_BLACK       0xFF000000
#define COL_LIGHTGREY   0xFFC7C7C7

void gui_init(void);

void gui_fillscreen(u32 fillcolor);

void gui_clearscreen(void);

void gui_draw_header(void);

void gui_draw_file_boxes(int dirsize);

void gui_draw_file_box_data(int position, TrackMetadata* meta);

void gui_beginframe(char* asdf);

void gui_drawframe(int appstate);

void gui_endframe(void);

void gui_blankframe(void);

void gui_exit(void);