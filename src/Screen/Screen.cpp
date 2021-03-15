#include "Screen.hh"
#include <bits/stdint-uintn.h>
#include <string>
#include <iostream>

Screen::Screen() {}

void Screen::Clear() {
    _pixels.reset();
    _shouldDraw = true;
}

bool Screen::DrawSprite(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t spriteSize) {
    // make a bitmask, then xor both bitsets
    bool pixelRemoved = false;
    for (uint8_t spriteY = 0 ; spriteY < spriteSize ; spriteY += 1) {
        uint8_t spriteByte = sprite[spriteY];
        uint8_t line = (y + spriteY) % SCREEN_HEIGHT;
        uint16_t lineOffset = line * SCREEN_WIDTH;
        for (uint8_t spriteX = 0 ; spriteX < 8 ; spriteX += 1) {
            bool set = (spriteByte >> (7 - spriteX)) & 0x1;
            uint8_t col = (x + spriteX) % SCREEN_WIDTH;
            bool pxSet = _pixels.test(lineOffset + col);
            bool newSet = pxSet ^ set;
            _pixels.set(lineOffset + col, newSet);
            if (pxSet != newSet) {
                UpdatePixel(col, line);
            }
            if (!pixelRemoved && pxSet && set) {
                pixelRemoved = true;
            }
        }
    }
    _shouldDraw = true;
    return pixelRemoved;
}

void Screen::Beep() {
    std::cout << '\a';
}

void Screen::ContinueBeep() {
    Beep();
}

void Screen::Draw(){
    if (_shouldDraw == false) { return; }
    _shouldDraw = false;
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

bool Screen::isOpen() const {
    return true;
}

bool Screen::Poll() { return false; }