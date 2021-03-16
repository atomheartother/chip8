# C++ip-8
This is a super simple CHIP-8 emulator weekend project.
Fully functional on Linux and Windows, not tested on MacOS.

!["Brick screenshot"](res/screenshot.png?raw=true)

## Installation
- `git clone git@github.com:atomheartother/chip8.git`
- Install `cmake`
- `mkdir build && cd build && cmake .. && cmake --build`

## Running
`./chip8 <rom file> [instructions per second]`

(The default value for instructions per second is 350).