#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <stdbool.h>

#include "formats/wav.h"
#include "formats/flac.h"
#include "formats/mp3.h"

//The different audio formats
typedef enum
{
	FORMAT_NONE = 0,
	FORMAT_WAV = 1,
	FORMAT_FLAC = 2,
	FORMAT_MP3 = 3
} Formats;

/**
 * @brief Fills audioBuffer with size of audio data and flushes it to the DSP
 */
void fill_buffer(void* audioBuffer, size_t size);

/**
 * @brief Frees the memory held in the audiobuffers if there is any
 */
void free_buffers(void);

/**
 * @brief Called when audio is done playing are there are not plans to play anymore, closes everything
 */
void exitplayer(void);

/**
 * @brief Figures out what format the audio inside unknownfile is in based on the first magic
 * 
 * @return One of Formats
 */
int recognize(FILE* unknownfile);

/**
 * @brief Initializes all the necessary things to prepare for audio playback
 */
bool* playerInit(void);

/**
 * @brief Starts to play audio.wav
 * 
 * @return Success or failure code
 */
int playfile(const char* filename);

/**
 * @brief Call this in the main loop of the program, checks if buffers are done and returns which ones are refilled
 * 
 * @return The buffers that are refilled, coded as follows: 0 = none, 1 = buffer 1, 2 = buffer 0, 3 = both 
 */
int play_audio(void);


/**
 * @brief If audio is playing, pause it. If audio is pause, resume it.
 */
void toggle_playback(void);