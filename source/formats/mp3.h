#include <mpg123.h>
#include <stdbool.h>

#define MP3BUFSIZE 4096

//Errors that may be returned by process_header
typedef enum
{
	MP3ERR_NONE = 0,
	MP3ERR_EXTRA_CHANNELS = 1,
	MP3ERR_MULTIPLE = 2
} MP3_Errors;

/**
 * @brief Gets the sample rate of the music
 * 
 * @return The sample rate of the music
 */
int get_sampleratemp3(void);

/**
 * @brief Gets the number of channels of the music
 * 
 * @return The number of channels of the music
 */
int get_channelsmp3(void);

/**
 * @brief Reads MP3BUFSIZE * numchannels bytes of data from the file into audiobuf
 * 
 * @param audiobuf Buffer to read data into
 */
void read_samplesmp3(void* audiobuf);

/**
 * @brief Gets the the position within the file
 * 
 * @return The the position within the file
 */
int get_fposmp3(void);

void exitmp3(void);

/**
 * @brief Gets the size of the buffer for mp3 files
 * 
 * @return The size of the buffer for mp3 files
 */
int get_bufsizemp3(void);

/**
 * @brief Verifies that a file is a proper mp3 file and extracts the information
 * 
 * @return One of MP3_Errors
 */
int process_headermp3(void);

/**
 * @brief Initializes mp3 playback from a mp3 file, extracting the information and verifying the file to be a mp3 file
 *
 * @param filename Name of the file to play
 */
int init_audiomp3(const char* filename);