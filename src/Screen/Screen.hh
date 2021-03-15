#pragma once

#include <bits/stdint-uintn.h>
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
        bool    DrawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t spriteSize);

        virtual bool isOpen() const;
        virtual bool Poll();
        // Draw updated screen state
        virtual void Draw();
        virtual void Deactivate() {};
        virtual void Activate() {};
        virtual void Beep();
        virtual void ContinueBeep();
        virtual void StopBeep() {};
    protected:
        // This function is called at each pixel update
        // it is called from within DrawSprite
        virtual void UpdatePixel(uint8_t, uint8_t) {};

        bool    _shouldDraw = true;
        std::bitset<SCREEN_WIDTH * SCREEN_HEIGHT> _pixels;
};