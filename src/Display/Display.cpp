#include "Display.hh"

Display::Display(unsigned width, unsigned height) 
    : _screenWidth(width), _screenHeight(height)
{
    _pixelWidth = width / CHIP8_SCREEN_WIDTH;
    _pixelHeight = height / CHIP8_SCREEN_HEIGHT;

    _window.create(sf::VideoMode(width, height), "CHIP-8 Emulator");
    _image.create(width, height, sf::Color(0,0,0));
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);

    Draw();
}

bool Display::isOpen() {
    return _window.isOpen();
}

bool Display::Poll(sf::Event& e) {
    return _window.pollEvent(e);
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