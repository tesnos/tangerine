#include <stdbool.h>

#include "../common.h"

#define DR_WAV_IMPLEMENTATION
#define WAVBUFSIZE 4096

//Errors that may be returned by process_header
typedef enum
{
	WAVERR_NONE = 0x0,
	WAVERR_DW_FAIL = 0x21,
	WAVERR_EXTRA_CHANNELS = 0x22,
	WAVERR_MULTIPLE = 0x23
} WAVError;

/**
 * @brief Gets the sample rate of the audio
 * 
 * @return The sample rate of the audio
 */
int get_sampleratewav(void);

/**
 * @brief Gets how far into the audio the program is
 * 
 * @return A percentage of the audio played, 0-100
 */
int get_progresswav(void);

/**
 * @brief Gets the number of channels of the audio
 * 
 * @return The number of channels of the audio
 */
int get_channelswav(void);

/**
 * @brief Reads WAVBUFSIZE bytes of data from the audio file into audiobuf
 * 
 * @param audiobuf Pointer to a buffer to read audio data into
 */
void read_sampleswav(void* audiobuf);

/**
 * @brief Closes out the currently playing audio file
 */
void exitwav(void);

/**
 * @brief Sets the current position of the audio playing to the point at percentage
 * 
 * @param percentage A position in percentage to continue playback at, 0-100
 */
void seekwav(int percentage);

/**
 * @brief Gets the size of the buffer for wav audio files
 * 
 * @return The size of the buffer for wav audio files
 */
int get_bufsizewav(void);

/**
 * @brief Verifies that a file is a proper wav audio file
 * 
 * @return One of WAVError
 */
int process_headerwav(void);

/**
 * @brief Initializes audio playback from a audio file, verifying the file first
 *
 * @param filename Path to the audio file
 * 
 * @return A WAVError
 */
int init_audiowav(const char* filename);

TrackMetadata* get_tmdwav(const char* filename);