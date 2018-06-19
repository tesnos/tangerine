#include <mpg123.h>
#include <stdbool.h>
#include <stdio.h>

#define MP3BUFSIZE 4096

//Errors that may be returned by process_header
typedef enum
{
	MP3ERR_NONE = 0x0,
	MP3ERR_EXTRA_CHANNELS = 0x41,
	MP3ERR_MULTIPLE = 0x42
} MP3Error;

/**
 * @brief Gets the sample rate of the audio
 * 
 * @return The sample rate of the audio
 */
int get_sampleratemp3(void);

/**
 * @brief Gets how far into the audio the program is
 * 
 * @return A percentage of the audio played, 0-100
 */
int get_progressmp3(void);

/**
 * @brief Gets the number of channels of the audio
 * 
 * @return The number of channels of the audio
 */
int get_channelsmp3(void);

/**
 * @brief Reads MP3BUFSIZE * numchannels bytes of data from the file into audiobuf
 * 
 * @param audiobuf Buffer to read data into
 */
void read_samplesmp3(void* audiobuf);

/**
 * @brief Closes out the currently playing audio file
 */
void exitmp3(void);

/**
 * @brief Sets the current position of the audio playing to the point at percentage
 * 
 * @param percentage A position in percentage to continue playback at, 0-100
 */
void seekmp3(int percentage);

/**
 * @brief Gets the size of the buffer for mp3 audio files
 * 
 * @return The size of the buffer for mp3 audio files
 */
int get_bufsizemp3(void);

/**
 * @brief Verifies that a file is a proper mp3 audio file
 * 
 * @return A MP3Error or an mpg123 error code
 */
int process_headermp3(void);

/**
 * @brief Initializes audio playback from a audio file, verifying the file first
 *
 * @param filename Path to the audio file
 * 
 * @return A MP3Error or an mpg123 error code
 */
int init_audiomp3(const char* filename);