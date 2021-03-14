# C++ip-8
This is a super simple CHIP-8 emulator weekend project.
Only tested on linux but this *should* work on other platforms... :I

!["Brick screenshot"](res/screenshot.png?raw=true)

## Installation
- Clone the repo (along with the submodules!)
- Install `cmake`
- `mkdir build && cd build && cmake .. && cmake --build`

## Running
`./chip8 <rom file> [instructions per second]`

(The default value for instructions per second is 350).