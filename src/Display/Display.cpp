#include "Display.hh"

Display::Display(unsigned width, unsigned height) {
    unsigned pixelWidth = width / CHIP8_SCREEN_WIDTH;
    unsigned pixelHeight = height / CHIP8_SCREEN_HEIGHT;

    _window.create(sf::VideoMode(width, height), "CHIP-8 Emulator");
    _image.create(width, height, sf::Color(0,0,0));
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);

    _window.clear();
    _window.draw(_sprite);
    _window.display();
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