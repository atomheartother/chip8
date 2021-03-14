#include "SFML.hh"
#include "SFML/Graphics/Color.hpp"
#include <bits/stdint-uintn.h>
#include <iostream>

const sf::Color BgColor = sf::Color(25,0,0);
const sf::Color FgColor = sf::Color(0, 255, 0);

SFML::SFML()
{
    const unsigned ratio = SCREEN_WIDTH / SCREEN_HEIGHT;
    const unsigned  windowWidth = sf::VideoMode::getDesktopMode().width;
    const unsigned windowHeight = windowWidth / ratio;
    _pixelWidth = windowWidth / SCREEN_WIDTH;
    _pixelHeight = windowHeight / SCREEN_HEIGHT;

    _window.create(sf::VideoMode(windowWidth, windowHeight), "CHIP-8 Emulator");
    _window.setVerticalSyncEnabled(true);
    Clear();
}

bool SFML::isOpen() const {
    return _window.isOpen();
}

bool SFML::Poll() {
    sf::Event e;
    bool evt = _window.pollEvent(e);
    switch (e.type) {
        case sf::Event::EventType::Closed:
            Close();
            break;
        case sf::Event::EventType::Resized:
            // Resize();
            break;
        default:
            break;
    }
    return evt;
}

void SFML::Close() {
    _window.close();
}

void SFML::SetPixel(unsigned pixelX, unsigned pixelY, const sf::Color& color) {
    unsigned xCoord = pixelX * _pixelWidth;
    unsigned yCoord = pixelY * _pixelHeight;
    for (unsigned x = xCoord ; x < xCoord + _pixelWidth ; x += 1) {
        for (unsigned y = yCoord ; y < yCoord + _pixelHeight ; y += 1) {
            _image.setPixel(x, y, color);
        }
    }
    _texture.update(_image);
}

void SFML::Clear() {
    Screen::Clear();
    _image.create(_pixelWidth * SCREEN_WIDTH, _pixelHeight * SCREEN_HEIGHT, BgColor);
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);
}

bool SFML::DrawSprite(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t spriteSize) {
    const bool res = Screen::DrawSprite(x, y, sprite, spriteSize);
    for (unsigned pixelY=0 ; pixelY < spriteSize ; pixelY += 1) {
        uint8_t line = (y + pixelY) % SCREEN_HEIGHT;
        uint16_t lineOffset = line * SCREEN_WIDTH;
        for (unsigned pixelX = 0; pixelX < 8 ; pixelX += 1) {
            uint8_t col = (x + pixelX) % SCREEN_WIDTH;
            const bool set = _pixels.test(lineOffset + col);
            if (res || set) {
                SetPixel(col, line, set ? FgColor : BgColor);
            }
        }
    }
    return res;
}

void SFML::Draw() {
    _window.clear(BgColor);
    _window.draw(_sprite);
    _window.display();
}