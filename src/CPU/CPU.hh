#pragma once

#include <cstdint>
#include "../Memory/Memory.hh"
#include "../Keys/Keys.hh"
#include "../Screen/Screen.hh"

#define nnn(x) (x & 0xFFF)
#define getX(x) ((x >> 8) & 0x0F)
#define getY(x) ((x >> 4) & 0x0F)
#define getKK(x) (x & 0xFF)

class CPU {
public:
    CPU(Memory* mem, const Keys* keys, Screen* screen);
    ~CPU();

    typedef void (CPU::*InstructionSetPtr)(uint16_t instruction);
    typedef void InstructionSet(uint16_t instruction);
    void    DecrementTimers();
    void    Tick();
    void    ExecuteInstruction(uint16_t instruction);
private:
    InstructionSet   ClsRet;
    InstructionSet   Jp;
    InstructionSet   Call;
    InstructionSet   SeByte;
    InstructionSet   SneByte;
    InstructionSet   SeReg;
    InstructionSet   Ld;
    InstructionSet   Add;
    InstructionSet   Op;
    InstructionSet   SneReg;
    InstructionSet   SetI;
    InstructionSet   JpV0;
    InstructionSet   Rnd;
    InstructionSet   Drw;
    InstructionSet   Skips;
    InstructionSet   Fxx;

    uint8_t   _Vx[16] = {0};
    uint16_t  _stack[16] = {0};

    InstructionSetPtr _instructions[16] = {
        &CPU::ClsRet, // 0xx operations
        &CPU::Jp,
        &CPU::Call,
        &CPU::SeByte,
        &CPU::SneByte,
        &CPU::SeReg,
        &CPU::Ld,
        &CPU::Add,
        &CPU::Op, // 8xx operations
        &CPU::SneReg,
        &CPU::SetI,
        &CPU::JpV0,
        &CPU::Rnd,
        &CPU::Drw,
        &CPU::Skips, // Exx operations
        &CPU::Fxx, // Fxx operations
    };
    Memory* _memory;
    const Keys*   _keys;
    Screen*       _screen;

    uint16_t  _i = 0;
    uint16_t  _pc = PROGRAM_LOAD_LOCATION;
    uint8_t   _delayTimer = 0;
    uint8_t   _soundTimer = 0;

    // SP matches the SIZE of the current stack
    // _stack[SP-1] to get the address at the top of the stack
    unsigned char  _sp = 0;
};