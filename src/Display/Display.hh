#pragma once

#include <SFML/Graphics.hpp>

class Display {
public:
    Display(unsigned width, unsigned height);

    bool    isOpen();
    bool    Poll(sf::Event& e);
    void    Close();

    void    Clear();
    void    Draw();

    void    SetPixel(unsigned x, unsigned y);

private:
    sf::RenderWindow    _window;
    sf::Image           _image;
    sf::Texture         _texture;
    sf::Sprite          _sprite;

    unsigned            _screenWidth;
    unsigned            _screenHeight;
    unsigned            _pixelWidth;
    unsigned            _pixelHeight;
};