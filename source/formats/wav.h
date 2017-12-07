#include <stdio.h>

#define WAVBUFSIZE 1024 * 16

//Errors that may be returned by process_header
typedef enum
{
  WAVERR_NONE = 0,
  WAVERR_WRONG_MAGIC = 1,
  WAVERR_EXTRA_CHANNELS = 2,
  WAVERR_NOT_PCM = 3,
  WAVERR_MULTIPLE = 4
} WAV_Errors;

/**
 * @brief Gets the sample rate of the music
 * 
 * @return The sample rate of the music
 */
int get_samplerate(void);

/**
 * @brief Gets the number of channels of the music
 * 
 * @return The number of channels of the music
 */
int get_channels(void);

/**
 * @brief Reads WAVBUFSIZE * numchannels bytes of data from the wavfile into audiobuf
 * 
 * @param audiobuf Buffer to read data into
 */
void read_samples(void* audiobuf);

/**
 * @brief Gets the the position within the file
 * 
 * @return The the position within the file
 */
int get_fpos();

/**
 * @brief Verifies that a file is a proper wav file and extracts the information
 * 
 * @return One of WAV_Errors
 */
int process_header(void);

/**
 * @brief Initializes wav playback from a wav file, extracting the information and verifying the file to be a wav file
 *
 * @param wavfile Pointer to the .wav file
 */
int init_audio(FILE* wavfile);