#include "Screen.hh"
#include <algorithm>
#include <algorithm>
#include <bits/stdint-uintn.h>
#include <iterator>
#include <string>
#include <iostream>

Screen::Screen() {}

void Screen::Clear() {
    _pixels.reset();
}

bool Screen::DrawSprite(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t spriteSize) {
    // make a bitmask, then xor both bitsets
    bool pixelRemoved = false;
    for (uint8_t spriteY = 0 ; spriteY < spriteSize ; spriteY += 1) {
        uint8_t spriteByte = sprite[spriteY];
        uint16_t lineOffset = ((y + spriteY) % SCREEN_HEIGHT) * SCREEN_WIDTH;
        for (uint8_t spriteX = 0 ; spriteX < 8 ; spriteX += 1) {
            bool set = (spriteByte >> (7 - spriteX)) & 0x1;
            uint8_t col = (x + spriteX) % SCREEN_WIDTH;
            bool pxSet = _pixels.test(lineOffset + col);
            _pixels.set(lineOffset + col, pxSet ^ set);
            if (!pixelRemoved && pxSet && set) {
                pixelRemoved = true;
            }
        }
    }
    return pixelRemoved;
}

void Screen::Draw() const {
    auto px = _pixels.to_string(' ', 'X');
    std::string out;
    out.reserve(px.size() + px.size() / SCREEN_WIDTH);
    for (uint32_t i = 0 ; i < px.size() ; i += 1) {
        if (!(i % SCREEN_WIDTH) && i) {
            out.push_back('\n');
        }
        out.push_back(px[i]);
    }
    std::cout << "\x1B[2J\x1B[H" << std::string(out.rbegin(), out.rend()) << std::endl;
}