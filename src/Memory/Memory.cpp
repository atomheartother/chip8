#include <cstring>
#include <iostream>
#include <fstream>
#include "Memory.hh"

// Total size of RAM for the emulator
// Interpreter took up the first 0x200 bytes of memory, which we'll mostly be ignoring,
// but we'll be leaving room for the hex sprites.
const uint16_t VIRT_RAM_SIZE = 0xFFFF;
const uint8_t HEX_SPRITES_SIZE = 16 * 5;
// This value is how much we need to offset input offsets by to get the real RAM values.
const uint16_t PROGRAM_OFFSET = PROGRAM_LOAD_LOCATION - HEX_SPRITES_SIZE;
// Physical size of ram
const unsigned short RAM_SIZE = VIRT_RAM_SIZE - PROGRAM_OFFSET;

const uint8_t hexSprites[HEX_SPRITES_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

Memory::Memory() {
    _ram = new uint8_t[RAM_SIZE];
    RAMWrite(hexSprites, 0, HEX_SPRITES_SIZE);
    std::cout << "[RAM] Initialized " <<  RAM_SIZE << "B of memory." << std::endl;
}

Memory::~Memory() {
    delete[] _ram;
    std::cout << "[RAM] Unloaded memory." << std::endl;
}

int Memory::Load(const char* filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "File not found: " << filename << std::endl;
        return 2;
    }

    in.seekg(0, in.end);
    const int length = in.tellg();
    in.seekg(0, in.beg);
    std::cout << "[RAM] Loading program " << filename << " (" << length << "B)." << std::endl;
    if (length > RAM_SIZE) {
        std::cerr << "[RAM] Program " << filename << " is too large: " << length << "B" << std::endl;
        return 3;
    }

    in.read(reinterpret_cast<char*>(_ram + HEX_SPRITES_SIZE), length);
    if (!in) {
        std::cerr << "[RAM] Couldn't load program, only read " << in.gcount() << "B" << std::endl;
        return 4;
    }
    return 0;
}

void Memory::WriteB(const uint8_t byte, uint16_t address) {
    RAMWrite(&byte, address - PROGRAM_OFFSET, 1);
}

void Memory::Write(const uint8_t *src, uint16_t address, uint8_t length) {
    RAMWrite(src, address - PROGRAM_OFFSET, length);
}

void Memory::Read(uint8_t *dst, uint16_t address, uint8_t length) const {
    address -= PROGRAM_OFFSET;
    for (uint8_t i = 0 ; i < length ; i += 1) {
        dst[i] = _ram[address + i];
    }
}

uint8_t Memory::ReadB(uint16_t address) const {
    return _ram[address - PROGRAM_OFFSET];
}

void Memory::RAMWrite(const uint8_t* src, uint16_t offset, uint8_t length) {
    std::memcpy(_ram + offset, src, length);
}

uint16_t Memory::HexSpriteAddress(uint8_t code) const {
    return (code * 5) + PROGRAM_OFFSET;
}

const uint8_t* Memory::Sprite(uint16_t address) const {
    return _ram + (address - PROGRAM_OFFSET);
}