# pragma once

#include <cstdint>

const unsigned int TOTAL_KEYS = 16;
const unsigned int NO_KEYS_PRESSED = TOTAL_KEYS;

class Keys {
    public:
        Keys();
        bool isKeyPressed(uint8_t code) const;
        uint8_t firstKeyPressed() const;
    private:
        uint8_t _keys[TOTAL_KEYS];
};