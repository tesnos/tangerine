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
- [x] Update internal documentation
- [ ] Actual file browsing
- [ ] Support for more formats
- [ ] Ability to create and play playlists
- [ ] Record and save bits of audio using the microphone like the built in app
- [ ] UI overhaul (currently it's not super terrible, but it's definitely not where I want it to be)
- [ ] Themes maybe?
+ anything else the 3DS's sound application does

## Building
Make sure you have the latest releases of [ctrulib](https://github.com/smealum/ctrulib/releases) and [citro3d](https://github.com/fincs/citro3d/releases) installed.  Also install libmpg123 for devkitARM via
[deltabeard's tutorial](https://github.com/deltabeard/3ds_portlibs), then just type
```
make 3dsx
```
to get a .3dsx and .smdh, or
```
make cia
```
to get a cia


## Known Issues:
- Some flac files have issues with playback speed on hardware but work fine in Citra
- Scrubbing within flac/mp3 files may take several seconds on Old 3DS depending on the size of the file

WARNING: This was my first time ever writing C. There are going to be bugs and 90% percent of this is probably terrible code, so don't hesitate to report anything because I probably wouldn't have found it otherwise.



## Credits:
Thanks to
- @smealum for [ctrulib](https://github.com/smealum/ctrulib)
- @fincs for [citro3d](https://github.com/fincs/citro3d)
- @Steveice10 for [buildtools](https://github.com/Steveice10/buildtools)
- @BernardoGiordano for [pp2d](https://github.com/BernardoGiordano/Checkpoint/tree/master/source/pp2d) and help with Makefile/rsf files
- [VLC for android](https://www.videolan.org/vlc/download-android.html) for the rough UI design concept
- @Sergey#5402 for help testing
- @Fenrir#3268, @ahoischen#9453, and @TurtleP#2954 (and anyone else!) in #dev in the Nintendo Homebrew discord for help with my beginner C mistakes
- [mackron](https://mackron.github.io/) for their [doctor libs](https://github.com/mackron/dr_libs) (dr_flac and dr_wav specifically)
- devkitPro for the example Makefile
- The big N for the Nintendo 3DS Sound application
- [Erokia](https://freesound.org/people/Erokia) for ["Heartbleed Synth"](https://freesound.org/people/Erokia/sounds/414770/)
- and @deltabeard for their libmpg123 portlib fork and [ctrmus](https://github.com/deltabeard/ctrmus) (big inspiration, helped me to understand the DSP a lot better)



## Licensing Information:
See LICENSE.md