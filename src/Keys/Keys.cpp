#include <SFML/Window/Keyboard.hpp>
#include "Keys.hh"

#include <iostream>

Keys::Keys() : 
    _keys {
        sf::Keyboard::X, // 0
        sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
        sf::Keyboard::Q, sf::Keyboard::W, sf::Keyboard::E,
        sf::Keyboard::A, sf::Keyboard::S, sf::Keyboard::D,
        sf::Keyboard::Z,                  sf::Keyboard::C,
        sf::Keyboard::Num4, sf::Keyboard::R, sf::Keyboard::F, sf::Keyboard::V,
    }{ }

bool Keys::isKeyPressed(uint8_t code) const {
    return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(_keys[code % TOTAL_KEYS]));
}

uint8_t Keys::firstKeyPressed() const {
    for (uint8_t i = 0; i < TOTAL_KEYS; i +=1) {
        if (isKeyPressed(i)) { return i; }
    }
    return NO_KEYS_PRESSED;
}