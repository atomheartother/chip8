# C++ip-8
This is a super simple CHIP-8 emulator weekend project. It uses SDL2 and can be transpiled to WASM using EMScripten to deploy on a website.

Demo: https://chip8.navennec.net/

Fully functional on Linux, Windows, Firefox, not tested on other platforms.

!["Brick screenshot"](res/screenshot.png?raw=true)

## Installation
- `git clone git@github.com:atomheartother/chip8.git`
- Install `cmake`
- `mkdir build && cd build && cmake .. && cmake --build .`

### EMSCripten build
Same as above but you'll want `emcmake` from the `emsdk`. Then:

`emcmake cmake .. && cmake --build .`

## Running
`./chip8 <rom file> [instructions per second]`

(The default value for instructions per second is 350).

## Bugs
- On Windows, the beeping sound is way too low
- Sometimes the screen disappears or doesn't draw anymore in browser if you keep loading new ROMs.
