#include <stdio.h>
#include <stdbool.h>

#define FLACBUFSIZE 4096

//Errors that may be returned by process_header
typedef enum
{
	FLACERR_NONE = 0x0,
	FLACERR_WRONG_MAGIC = 0x31,
	FLACERR_DF_FAIL = 0x32,
	FLACERR_EXTRA_CHANNELS = 0x33,
	FLACERR_MULTIPLE = 0x34
} FLAC_Errors;

/**
 * @brief Gets the sample rate of the music
 * 
 * @return The sample rate of the music
 */
int get_samplerateflac(void);

int get_progressflac(void);

/**
 * @brief Gets the number of channels of the music
 * 
 * @return The number of channels of the music
 */
int get_channelsflac(void);

/**
 * @brief Reads FLACBUFSIZE * numchannels bytes of data from the file into audiobuf
 * 
 * @param audiobuf Buffer to read data into
 */
void read_samplesflac(void* audiobuf);

void exitflac(void);

void seekflac(int percentage);

/**
 * @brief Gets the size of the buffer for flac files
 * 
 * @return The size of the buffer for flac files
 */
int get_bufsizeflac(void);

/**
 * @brief Verifies that a file is a proper flac file and extracts the information
 * 
 * @return One of FLAC_Errors
 */
int process_headerflac(void);

/**
 * @brief Initializes flac playback from a flac file, extracting the information and verifying the file to be a flac file
 *
 * @param filename Name of the file to play
 */
int init_audioflac(const char* filename);