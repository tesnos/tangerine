#include <stdio.h>
#include <string.h>
#include <3ds.h>

#include "pp2d/pp2d.h"
#include "player.h"

//Buffer for printing a string to the screen
char str[256];

//Buffers to hold audio data
int16_t* audioBuffer0;
int16_t* audioBuffer1;

//Define colors
u32 col_yellow = RGBA8(0xFF, 0xFF, 0x00, 0xFF);
u32 col_red = RGBA8(0xFF, 0x00, 0x00, 0xFF);
u32 col_blue = RGBA8(0x00, 0x00, 0xFF, 0xFF);
//u32 col_green = RGBA8(0x00, 0xFF, 0x00, 0xFF);
u32 col_white = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
u32 col_black = RGBA8(0x00, 0x00, 0x00, 0xFF);

//Audio file pointer
FILE* afile;

//Information about the audio file
int16_t buffersize;
int16_t samplerate;
int16_t numchannels;

void fill_buffer(void* audioBuffer, size_t size)
{
    readwav(audioBuffer);
    
    DSP_FlushDataCache(audioBuffer, size);
}

void cleanexit()
{
  //Properly exit all previously initialized libraries/services
  pp2d_exit();
  hidExit();
  ndspExit();
  
  //Free the memory allocated previously
  linearFree(audioBuffer0);
  linearFree(audioBuffer1);
  
  //Close file objects
  fclose(afile);
}


void errexit(int errcode)
{
  u32 kDown;
  
  while (1)
  {
    hidScanInput();
    kDown = hidKeysDown();
    if (kDown & KEY_START){
      break;
    }
	
	pp2d_begin_draw(GFX_TOP, GFX_LEFT);
	
	pp2d_draw_rectangle(0, 0, 400, 240, col_black);
	
	snprintf(str, sizeof(str), "%zu", errcode);
    pp2d_draw_text(20, 40, 1, 1, col_white, str);
	
	pp2d_end_draw();
  }
  
  cleanexit();
}

int main()
{
  ndspWaveBuf waveBuf[2];
  
  //Initialize the hid (human interface device (controls)), ndsp (sound chip), and pp2d (graphics)
  hidInit();
  ndspInit();
  pp2d_init();
  
  //Open our audio file
  afile = fopen("audio.wav", "rb");
  
  //Store local information about the audio for convience
  buffersize = get_bufsize();
  samplerate = get_samplerate();
  numchannels = get_channels();
  
  int initsucess = init_playback(afile);
  if (initsucess > 0)
  {
    errexit(openwav);
  }
  
  //Configure default colors for top/bottom screens and disable 3D
  pp2d_set_screen_color(GFX_TOP, col_black);
  pp2d_set_screen_color(GFX_BOTTOM, col_black);
  pp2d_set_3D(1);
  
  //Holds which keys are down
  u32 kDown;
  
  //Play stereo audio, regardless of whether input is mono
  ndspSetOutputMode(NDSP_OUTPUT_STEREO);
  
  //Reset,
  ndspChnReset(0);
  //clear,
  ndspChnWaveBufClear(0);
  //idk it's required,
  ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);
  //and set the samplerate of the NDSP
  ndspChnSetRate(0, samplerate);
  
  if (numchannels > 2)
  {
    //Dual channel audio
    ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
  }
  else
  {
    //Single channel audio
    ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM16);
  }
  
  //Allocate space in memory for our 2 wavebuffers the size of the buffer * # of channels
  audioBuffer0 = linearAlloc(buffersize * numchannels);
  audioBuffer1 = linearAlloc(buffersize * numchannels);
  
  //Set the front right and left volumes of our channel to 100% but silence everything else
  float mix[12];
  memset(mix, 0, sizeof(mix));
  mix[0] = 1.0;
  mix[1] = 1.0;
  ndspChnSetMix(0, mix);
  
  //Clear wavebuffers and configure their addresses and how many samples they contain
  memset(waveBuf, 0, sizeof(waveBuf));
  waveBuf[0].data_vaddr = &audioBuffer0[0];
  waveBuf[0].nsamples = (buffersize / numchannels);
  waveBuf[1].data_vaddr = &audioBuffer1[0];
  waveBuf[1].nsamples = (buffersize / numchannels);
  
  //Put the first bits of audio in them
  fill_buffer(audioBuffer0, buffersize * numchannels);
  fill_buffer(audioBuffer1, buffersize * numchannels);
  
  //Then queue them
  ndspChnWaveBufAdd(0, &waveBuf[0]);
  ndspChnWaveBufAdd(0, &waveBuf[1]);
  
  //Main loop of program
  while (aptMainLoop())
  {
    //Scan input and put all keys down into kDown
    hidScanInput();
    kDown = hidKeysDown();
    
    //If the start key is pressed, exit to the hbmenu
    if (kDown & KEY_START){
      break;
    }
    
    //Start drawing this frame's graphics
    pp2d_begin_draw(GFX_TOP, GFX_LEFT);
    
        //Clear the screen
        pp2d_draw_rectangle(0, 0, 400, 240, col_black);
        
        //Example rectangle
        pp2d_draw_rectangle(20, 20, 40, 20, col_yellow);
    
    //Audio stuffs
    if (waveBuf[0].status == NDSP_WBUF_DONE) {
        fill_buffer(waveBuf[0].data_pcm16, buffersize * numchannels);
        
        ndspChnWaveBufAdd(0, &waveBuf[0]);
        
        pp2d_draw_rectangle(60, 20, 40, 20, col_blue);
    }
    
    if (waveBuf[1].status == NDSP_WBUF_DONE) {
        fill_buffer(waveBuf[1].data_pcm16, buffersize * numchannels);
        
        ndspChnWaveBufAdd(0, &waveBuf[1]);
        
        pp2d_draw_rectangle(100, 20, 40, 20, col_red);
    }
	
	snprintf(str, sizeof(str), "%zu", get_fpos());
    pp2d_draw_text(20, 40, 1, 1, col_white, str);
	snprintf(str, sizeof(str), "%zu", get_samplerate());
    pp2d_draw_text(20, 60, 1, 1, col_white, str);
    
    //Finish drawing
    pp2d_end_draw();
  }
  
  cleanexit();

  return 0;
}