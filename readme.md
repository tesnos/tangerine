<img src="https://github.com/tesnos/tangerine/raw/master/meta/logo.png" alt="tangerine logo">

# tangerine
### A work-in-progress replacement for the 3DS's music player


## Overview:
- Put all music in "/music" folder on your SD card as file browsing is not currently supported yet
- Support for multiple formats (wav, flac, and mp3 currently)
- Skip forward and back a song using the L and R buttons
- Use the progress bar on the touch screen to scrub through the audio (SLOW)
- Pause/Play with A, and return to file selection with B
- *Touchscreen* controls (yay?)


## Planned Features:
- [ ] Update internal documentation
- [ ] Actual file browsing
- [ ] Support for more formats
- [ ] Ability to create and play playlists
- [ ] Record and save bits of audio using the microphone like the built in app
- [ ] UI overhaul (currently it's not super terrible, but it's definitely not where I want it to be)
- [ ] Themes maybe?
+ anything else the 3DS's sound application does

## Building
Make sure you have the latest releases of [ctrulib](https://github.com/smealum/ctrulib/releases) and [citro3d](https://github.com/fincs/citro3d/releases) installed.  Then download the above linked in commit of [buildtools](https://github.com/Steveice10/buildtools/tree/adf5fa3e8382a0a3847ec3f7dcff781417c8751c) into the preexisting "buildtools" folder.  Also install libmpg123 for devkitARM via
[deltabeard's tutorial](https://github.com/deltabeard/3ds_portlibs), then just type
```
make
```


## Known Issues:
- Some flac files have issues with playback speed
- Scrubbing within wav files can cause odd behavior with the progress of the file
- Scrubbing within flac files may take several seconds depending on the size of the file

WARNING: This was my first time ever writing C. There are going to be bugs and 90% percent of this is probably terrible code, so don't hesitate to report anything because I probably wouldn't have found it otherwise.



## Credits:
Thanks to
- @smealum for [ctrulib](https://github.com/smealum/ctrulib)
- @fincs for [citro3d](https://github.com/fincs/citro3d)
- @Steveice10 for [buildtools](https://github.com/Steveice10/buildtools)
- @BernardoGiordano for [pp2d](https://github.com/BernardoGiordano/Checkpoint/tree/master/source/pp2d)
- [VLC for android](https://www.videolan.org/vlc/download-android.html) for the rough UI design concept
- @Fenrir#3268, @ahoischen#9453, and @TurtleP#2954 (and anyone else!) in #dev in the Nintendo Homebrew discord for help with my beginner C mistakes
- [mackron](https://mackron.github.io/) for their [doctor libs](https://github.com/mackron/dr_libs) (dr_flac and dr_wav specifically)
- devkitPro for the example Makefile
- The big N for the Nintendo 3DS Sound application
- [Erokia](https://freesound.org/people/Erokia) for ["Heartbleed Synth"](https://freesound.org/people/Erokia/sounds/414770/)
- and @deltabeard for their libmpg123 portlib fork and [ctrmus](https://github.com/deltabeard/ctrmus) (big inspiration, helped me to understand the DSP a lot better)



## Licensing Information:
See LICENSE.md