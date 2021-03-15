#include <SDL.h>
#include "Keys.hh"

Keys::Keys() : 
    _keys {
        SDL_SCANCODE_X, // 0
        SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
        SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E,
        SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D,
        SDL_SCANCODE_Z,                 SDL_SCANCODE_C,
        SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V,
    }{ }

bool Keys::isKeyPressed(uint8_t code) const {
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    return state[_keys[code]];
}

uint8_t Keys::firstKeyPressed() const {
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    for (uint8_t i = 0; i < TOTAL_KEYS; i +=1) {
        if (state[_keys[i]]) { return i; }
    }
    return NO_KEYS_PRESSED;
}