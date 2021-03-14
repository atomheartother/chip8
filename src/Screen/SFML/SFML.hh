#pragma once

#include <SFML/Graphics.hpp>
#include "../Screen.hh"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Window/Window.hpp"

class SFML : public Screen {
public:
    SFML();

    sf::Window* Window();
    void    Close();
    void    ThreadDraw();
    // Overrides
    bool    isOpen() const;
    bool    Poll();
    void    Clear();
    void    Draw();
    // Draw a sprite at coordinates
    bool    DrawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t spriteSize);
    void    Deactivate();
    void    Activate();
private:
    void    SetPixel(unsigned x, unsigned y, const sf::Color& color);

    sf::RenderWindow    _window;
    sf::Image           _image;
    sf::Texture         _texture;
    sf::Sprite          _sprite;

    unsigned            _pixelWidth;
    unsigned            _pixelHeight;
};