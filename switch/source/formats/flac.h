#include <stdbool.h>

#include "../common.h"

#define FLACBUFSIZE 4096

//Errors that may be returned by process_header
typedef enum
{
	FLACERR_NONE = 0x0,
	FLACERR_WRONG_MAGIC = 0x31,
	FLACERR_DF_FAIL = 0x32,
	FLACERR_EXTRA_CHANNELS = 0x33,
	FLACERR_MULTIPLE = 0x34
} FLACError;

/**
 * @brief Gets the sample rate of the audio
 * 
 * @return The sample rate of the audio
 */
int get_samplerateflac(void);

/**
 * @brief Gets how far into the audio the program is
 * 
 * @return A percentage of the audio played, 0-100
 */
int get_progressflac(void);

/**
 * @brief Gets the number of channels of the audio
 * 
 * @return The number of channels of the audio
 */
int get_channelsflac(void);

/**
 * @brief Reads FLACBUFSIZE bytes of data from the audio file into audiobuf
 * 
 * @param audiobuf Pointer to a buffer to read audio data into
 */
void read_samplesflac(void* audiobuf);

/**
 * @brief Closes out the currently playing audio file
 */
void exitflac(void);

/**
 * @brief Sets the current position of the audio playing to the point at percentage
 * 
 * @param percentage A position in percentage to continue playback at, 0-100
 */
void seekflac(int percentage);

/**
 * @brief Gets the size of the buffer for flac audio files
 * 
 * @return The size of the buffer for flac audio files
 */
int get_bufsizeflac(void);

/**
 * @brief Verifies that a file is a proper flac audio file
 * 
 * @return A FLACError
 */
int process_headerflac(void);

/**
 * @brief Initializes audio playback from a audio file, verifying the file first
 *
 * @param filename Path to the audio file
 * 
 * @return A FLACError
 */
int init_audioflac(const char* filename);

TrackMetadata* get_tmdflac(const char* filename);