#include <3ds.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pp2d/pp2d.h"

//Define colors, any commented out here are no longer in use
#define col_white RGBA8(0xFF, 0xFF, 0xFF, 0xFF)
#define col_black RGBA8(0x00, 0x00, 0x00, 0xFF)
#define col_grey RGBA8(0x7F, 0x7F, 0x7F, 0xFF)
#define col_lightgrey RGBA8(0xC9, 0xCA, 0xDD, 0xFF)

//#define col_red = RGBA8(0xFF, 0x00, 0x00, 0xFF)
//#define col_blue = RGBA8(0x00, 0x00, 0xFF, 0xFF)
//#define col_green = RGBA8(0x00, 0xFF, 0x00, 0xFF)

//#define col_purple = RGBA8(0xFF, 0x00, 0xFF, 0xFF)
//#define col_cyan = RGBA8(0x00, 0xFF, 0xFF, 0xFF)
#define col_base RGBA8(0x29, 0x71, 0xEE, 0xFF)
#define col_orange RGBA8(0xFF, 0xA5, 0x00, 0xFF)
#define col_yellow RGBA8(0xFF, 0xFF, 0x00, 0xFF)

/**
 * @brief Gets everything prepared to use the gui
 */
void gui_init(char** entrytableptr, int* dirposptrinput);

void gui_draw_play(bool playing);

void gui_draw_progress(float percent);

/**
 * @brief Prepares to begin drawing to screen target, on side
 * 
 * @param target Which screen to draw on, GFX_TOP or GFX_BOTTOM
 * @param side Which 3d side to draw on, GFX_LEFT or GFX_RIGHT
 */
void gui_prepare_frame(gfxScreen_t target, gfx3dSide_t side);

/**
 * @brief Convert any value to a char array and draw it to a specified point w/ a specified color
 * 
 * @param x X position for printing
 * @param y Y position for printing
 * @param color RGB color to print text in
 * @param value The value to print
 */
void gui_printi(float x, float y, u32 color, int value);

/**
 * @brief Convert any value to a char array and draw it to a specified point w/ a specified color
 * 
 * @param x X position for printing
 * @param y Y position for printing
 * @param color RGB color to print text in
 * @param value The value to print
 */
void gui_printc(float x, float y, u32 color, char* value);

/**
 * @brief Draw a frame that corresponds to the current application state
 * 
 * @param state One of the application states, tells what frame to draw
 */
void gui_draw_frame(int state);

/**
 * @brief Finish drawing to the screen for the current frame
 */
void gui_finish_frame(void);

/**
 * @brief Clean everything up for when the application needs to exit
 */
void gui_exit(void);