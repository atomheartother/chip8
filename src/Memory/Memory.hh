#pragma once

#include <bits/stdint-uintn.h>
class Memory {
public:
    Memory();
    ~Memory();

    int Load(const char* filename);
private:
    uint8_t*  _ram;
};