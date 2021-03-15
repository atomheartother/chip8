#include "SDL.hh"
#include "SDL_events.h"
#include "SDL_hints.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <SDL.h>
#include <algorithm>

const unsigned PIXEL_SIZE = 12;
const unsigned PIXEL_GAP = 2;

SDL::SDL() {
    uint16_t width = SCREEN_WIDTH * PIXEL_SIZE;
    uint16_t height = SCREEN_HEIGHT * PIXEL_SIZE;

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );
    _window = SDL_CreateWindow("Ch++8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);
    _open = true;
    Clear();
}

SDL::~SDL() {
    SDL_DestroyTexture(_texture);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void SDL::Clear() {
    Screen::Clear();
    _rects.clear();
}

void SDL::SetForegroundColor() {
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
}

void SDL::SetBackgroundColor() {
    SDL_SetRenderDrawColor(_renderer, 100, 0, 50, 255);
}

void SDL::UpdatePixel(uint8_t x, uint8_t y, bool set) {
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

void SDL::Draw() {
    SetBackgroundColor();
    SDL_RenderClear(_renderer);
    SetForegroundColor();
    SDL_Rect* rects = &_rects[0];
    SDL_RenderFillRects(_renderer, rects, _rects.size());
    SDL_RenderPresent(_renderer);
}

bool SDL::isOpen() const {
    return _open;
}

bool SDL::Poll() {
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