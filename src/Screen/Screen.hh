#pragma once

#include <cstdint>
#include <bitset>

const uint8_t SCREEN_WIDTH = 64;
const uint8_t SCREEN_HEIGHT = 32;

class Screen {
    public:
        Screen();
        // Clear the screen
        void    Clear();
        // Draw a sprite at coordinates
        bool    DrawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t spriteSize);

        // Draw updated screen state
        void Draw() const;
    protected:
        std::bitset<SCREEN_WIDTH * SCREEN_HEIGHT> _pixels;
};