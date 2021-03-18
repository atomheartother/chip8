#include "SDL.hh"
#include "SDL_audio.h"
#include "SDL_error.h"
#include "SDL_timer.h"
#include <SDL.h>
#include <algorithm>
#include <iostream>
#include <cmath>

#define PI2 6.28318530718

const unsigned PIXEL_SIZE = 12;
const unsigned PIXEL_GAP = 3;

const double SmplRate = 44100;
#ifndef EMSCRIPTEN
const float synthFreq = 440;
#else
// Web for some reason plays it at double pitch??
const float synthFreq = 220;
#endif
void audioCallback(void* ptr, uint8_t* stream, int len) {
    ScreenSDL* screen = static_cast<ScreenSDL*>(ptr);
	short * snd = reinterpret_cast<short*>(stream);
	len /= sizeof(*snd);
	for(int i = 0; i < len; i++)
	{
		snd[i] = 5000 * sin(screen->time);
		screen->time += synthFreq * PI2 / SmplRate;
		if(screen->time >= PI2)
			screen->time -= PI2;
	}
}

ScreenSDL::ScreenSDL() {
    uint16_t width = SCREEN_WIDTH * PIXEL_SIZE;
    uint16_t height = SCREEN_HEIGHT * PIXEL_SIZE;

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO );
    _window = SDL_CreateWindow("Ch++8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!_window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return;
    }
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) { 
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return; 
    }
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!_texture) { 
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return; 
    }
    Clear();
    std::cout << "[SDL] Window initialized." << std::endl;
    // Init audio
    SDL_zero(_spec);
    _spec.freq = SmplRate;
    _spec.format = AUDIO_S16SYS;
    _spec.channels = 1;
    _spec.samples = 512;
    _spec.callback = audioCallback;
    _spec.userdata = this;

    _audioDeviceId = SDL_OpenAudioDevice(nullptr, 0, &_spec, &_aspec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (_audioDeviceId <= 0) {
        std::cerr << "[SDL] Error opening audio device: " << SDL_GetError() << std::endl;
        return; 
    }
    std::cout << "[SDL] Initialized audio device with ID " << +_audioDeviceId << std::endl;
    // Mark the window as opened, allowing the program to start
    _open = true;
    std::cout << "[SDL] Initialization successful." << std::endl;
}

ScreenSDL::~ScreenSDL() {
    SDL_DestroyTexture(_texture);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void ScreenSDL::Clear() {
    Screen::Clear();
    _rects.clear();
}

void ScreenSDL::SetForegroundColor() {
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
}

void ScreenSDL::SetBackgroundColor() {
    SDL_SetRenderDrawColor(_renderer, 100, 0, 50, 255);
}

void ScreenSDL::UpdatePixel(uint8_t x, uint8_t y, bool set) {
    int realX = x * PIXEL_SIZE + PIXEL_GAP;
    int realY = y * PIXEL_SIZE + PIXEL_GAP;
    if (set) {
        SDL_Rect r;
        r.x = realX;
        r.y = realY;
        r.w = PIXEL_SIZE - PIXEL_GAP;
        r.h = PIXEL_SIZE - PIXEL_GAP;
        _rects.push_back(r);
    } else {
        _rects.erase(std::remove_if(
            _rects.begin(), _rects.end(),
            [realX, realY](const SDL_Rect& r) {
                return r.x == realX && r.y == realY;
            }), _rects.end());
    }
}

void ScreenSDL::Draw() {
    SetBackgroundColor();
    SDL_RenderClear(_renderer);
    if (_rects.size() >= 1) {
        SetForegroundColor();
        SDL_Rect* rects = &_rects[0];
        SDL_RenderFillRects(_renderer, rects, _rects.size());
    }
    SDL_RenderPresent(_renderer);
}

bool ScreenSDL::isOpen() const {
    return _open;
}

bool ScreenSDL::Poll() {
    SDL_Event e;
    bool res = SDL_PollEvent(&e);
    switch (e.type) {
        case SDL_QUIT:
            _open = false;
            break;
        default:
            break;
    }
    return res;
}

void    ScreenSDL::Beep() {
    SDL_PauseAudioDevice(_audioDeviceId, 0);
}

void    ScreenSDL::StopBeep() {
    SDL_PauseAudioDevice(_audioDeviceId, 1);
}