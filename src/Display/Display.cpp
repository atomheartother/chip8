#include "Display.hh"

Display::Display(unsigned width, unsigned height) 
{
    const auto dm = sf::VideoMode::getDesktopMode();
    _screenWidth = dm.width;
    _screenHeight = dm.height;
    _pixelWidth = _screenWidth / width;
    _pixelHeight = _screenHeight / height;

    _window.create(sf::VideoMode(_screenWidth, _screenHeight), "CHIP-8 Emulator");
    _image.create(_screenWidth, _screenHeight, sf::Color(0,0,0));
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