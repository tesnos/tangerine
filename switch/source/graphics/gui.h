#include "graphics.h"

//Format in hex is 0xAABBGGRR
#define CLEAR_COL 0xFFFFD0D0
#define COL_ORANGE 0xFF00A5FF

void gui_init(void);

void gui_fillscreen(u32 fillcolor);

void gui_clearscreen(void);

void gui_draw_header(void);

void gui_beginframe(void);

void gui_drawframe(int appstate);

void gui_endframe(void);

void gui_blankframe(void);

void gui_exit(void);