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
} Format;

/**
 * @brief Fills audioBuffer with size bytes of audio data and flushes it to the DSP
 * 
 * @param audioBuffer Pointer to the buffer for fill
 * @param size Size, in bytes of the buffer to fill
 */
void fill_buffer(void* audioBuffer, size_t size);

/**
 * @brief Frees the memory held in the audio buffers if there is any
 */
void free_buffers(void);

/**
 * @brief Closes everything, called when the application is closing
 */
void player_exit(void);

/**
 * @brief Figures out what format the audio inside unknownfile is in based on the file's magic
 * 
 * @param unknownfile Pointer to the file that needs to analyzed
 * 
 * @return A Format
 */
int recognize(FILE* unknownfile);

/**
 * @brief Initializes all the necessary things to prepare for audio playback
 * 
 * @return Pointer to the boolean that keeps track of whether the audio is currently playing
 */
bool* player_init(void);

/**
 * @brief Stops playback of the current audio file so another can be played or the application can be closed
 */
void ceaseplayback(void);

/**
 * @brief Sets the progress of the currently playing audio to percentage
 * 
 * @param percentage A percentage of the audio to jump to, 0-100
 */
void seekaudio(int percentage);

/**
 * @brief Loads an audio file at the path of filename, and configures the DSP to begin playback of it
 * 
 * @param filename Path to where the file is located
 * 
 * @return 0 for success or failure code (0x10 is not an audio file, anything else if from the individual format)
 */
int playfile(const char* filename);

/**
 * @brief Gets how far into the audio the player is
 * 
 * @return A percentage of the audio played, 0-100
 */
int get_progress(void);

/**
 * @brief Call this in the main loop of the program, keeps audio data fresh and returns playback progress
 * 
 * @return The progress of the audio played, a percentage 0-100
 */
int play_audio(void);

bool* get_playing_handle();

/**
 * @brief If audio is playing, pause it. If audio is paused, resume it.
 */
void toggle_playback(void);