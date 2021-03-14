#pragma once

#include <bits/stdint-uintn.h>
#include <cstdint>
class Memory {
public:
    Memory();
    ~Memory();

    // Load a program in memory
    int     Load(const char* filename);

    // Write from src into memory at offset, for length bytes
    void    Write(const uint8_t* src, uint16_t offset, uint8_t length);
    // Write byte at offset
    void    WriteB(const uint8_t byte, uint16_t offset);

    // Read from memory into dst at offset, for length bytes
    void    Read(uint8_t* dst, uint16_t offset, uint8_t length) const;
private:
    uint8_t*  _ram;
};