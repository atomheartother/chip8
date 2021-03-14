#pragma once

#include <cstdint>
#include <bitset>

const uint8_t SCREEN_WIDTH = 64;
const uint8_t SCREEN_HEIGHT = 32;

class Screen {
    public:
        Screen();
        virtual ~Screen() {}
        // Clear the screen
        virtual void    Clear();
        // Draw a sprite at coordinates
        virtual bool    DrawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t spriteSize);

        virtual bool isOpen() const;
        virtual bool Poll();
        // Draw updated screen state
        virtual void Draw();
        virtual void Deactivate() {};
        virtual void Activate() {};
    protected:
        bool    _shouldDraw = true;
        std::bitset<SCREEN_WIDTH * SCREEN_HEIGHT> _pixels;
};