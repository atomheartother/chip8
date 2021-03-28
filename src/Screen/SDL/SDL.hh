#pragma once

#include "../Screen.hh"
#include "SDL_audio.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <cstdint>
#include <vector>

class ScreenSDL : public Screen {
public:
    ScreenSDL();
    ~ScreenSDL();

    void Clear();
    bool isOpen() const;
    bool Poll();
    void Draw();
    void UpdatePixel(uint8_t x, uint8_t y, bool set);
    void Beep();
    void StopBeep();

    float           time = 0;
    SDL_AudioSpec   _aspec;
private:
    void    SetBackgroundColor();
    void    SetForegroundColor();
    std::vector<SDL_Rect> _rects;
    SDL_Window*     _window;
    SDL_Renderer*   _renderer;
    SDL_Texture*    _texture;
    SDL_AudioSpec   _spec;
    SDL_AudioDeviceID _audioDeviceId;
    bool            _open = false;
};