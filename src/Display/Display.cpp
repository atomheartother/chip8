#include "Display.hh"
#include <iostream>

Display::Display(unsigned width, unsigned height) :
    _width(width), _height(height)
{
    const auto dm = sf::VideoMode::getDesktopMode();
 
    const unsigned screenWidth = dm.width;
    const unsigned screenHeight = dm.height;
    _pixelWidth = screenWidth / width;
    _pixelHeight = screenHeight / height;

    _window.create(sf::VideoMode(screenWidth, screenHeight), "CHIP-8 Emulator");
    _window.setVerticalSyncEnabled(true);
    _image.create(screenWidth, screenHeight, sf::Color(0,0,0));
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);

    Draw();
}

bool Display::isOpen() {
    return _window.isOpen();
}

bool Display::Poll(sf::Event& e) {
    bool evt = _window.pollEvent(e);
    switch (e.type) {
        case sf::Event::EventType::Closed:
            Close();
            break;
        case sf::Event::EventType::Resized:
            // Resize();
            break;
    }
    return evt;
}

void Display::Close() {
    _window.close();
}

void Display::SetPixel(unsigned pixelX, unsigned pixelY) {
    unsigned xCoord = pixelX * _pixelWidth;
    unsigned yCoord = pixelY * _pixelHeight;
    for (unsigned x = xCoord ; x < xCoord + _pixelWidth ; x += 1) {
        for (unsigned y = yCoord ; y < yCoord + _pixelHeight ; y += 1) {
            _image.setPixel(x, y, sf::Color(255, 255, 255));
        }
    }
    _texture.update(_image);
}

void Display::Draw() {
    _window.clear();
    _window.draw(_sprite);
    _window.display();
}