#pragma once

#include <bits/stdint-uintn.h>
#include <cstdint>

const uint16_t PROGRAM_LOAD_LOCATION = 0x200;

class Memory {
public:
    Memory();
    ~Memory();

    // Load a program in memory
    int     Load(const char* filename);

    // Write from src into memory at offset, for length bytes
    void    Write(const uint8_t* src, uint16_t vOffset, uint8_t length);
    // Write byte at offset
    void    WriteB(const uint8_t byte, uint16_t vOffset);

    // Read from memory into dst at offset, for length bytes
    void    Read(uint8_t* dst, uint16_t vOffset, uint8_t length) const;
    // Read byte at address
    uint8_t ReadB(uint16_t address) const;
    // Returns the VMEM address of the hexsprite with this code
    uint16_t    HexSpriteAddress(uint8_t code) const;

    // Returns the sprite at this address
    const uint8_t* Sprite(uint16_t address) const;

private:
    void RAMWrite(const uint8_t* src, uint16_t offset, uint8_t length);
    uint8_t*  _ram;
};