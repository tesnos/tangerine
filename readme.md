<img src="https://github.com/tesnos/tangerine/raw/master/meta/logo.png" alt="tangerine logo">

# tangerine
### A work-in-progress music player for the Nintendo 3DS and Nintendo Switch


## Overview:
- The 3DS build is currently in heavy development as the shift is made from pp2d to citro2d. Please use the previous build instead of building yourself.
- Put all music in "/music" folder on your SD card as traditional file browsing is not currently supported yet
- Support for multiple formats (wav, flac, and mp3 currently)
- Skip forward and back a song using the L and R buttons
- Use the progress bar on the touch screen to scrub through the audio (SLOW)
- Pause/Play with A, and return to file selection with B
- *Touchscreen* controls (yay?)


## Planned Features:
- [ ] Update internal documentation
- [ ] Move to citro2d
- [ ] Switch version
- [ ] Actual file browsing
- [ ] Support for more formats
- [ ] Ability to create playlists
- [x] Ability to play playlists
- [ ] Record and save bits of audio using the microphone like the built in app
- [ ] UI overhaul (currently it's not super terrible, but it's definitely not where I want it to be)
- [ ] Themes maybe?
+ anything else the 3DS's sound application does

## Building

You can just run `make` or `make all` to get both 3ds and switch builds, but for just one of them follow this:

### 3DS
Make sure you have the latest releases of ctrulib, citro3d, citro2d, and 3ds-mpg123 installed via the devkitPro pacman. Then type
```
make 3ds
```
to get a .3dsx, .smdh, and .cia

### Switch
Make sure you have the latest releases of libnx and switch-mpg123 installed via the devkitPro pacman. Then type
```
make switch
```
to get an .nro

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
- devkitPro for the example Makefiles, portlibs, and [citro2d](https://github.com/devkitPro/citro2d)
- switchbrew/3dsbrew for documentation and examples
- The big N for the Nintendo 3DS Sound application
- [Erokia](https://freesound.org/people/Erokia) for ["Heartbleed Synth"](https://freesound.org/people/Erokia/sounds/414770/)
- and @deltabeard for their orginal libmpg123 portlib fork and [ctrmus](https://github.com/deltabeard/ctrmus) (big inspiration, helped me to understand the DSP a lot better)



## Licensing Information:
See LICENSE.md