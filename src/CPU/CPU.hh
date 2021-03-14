#pragma once

#include <bits/stdint-uintn.h>

class CPU {
public:
    typedef void (CPU::*InstructionSetPtr)(uint16_t instruction);
    typedef void InstructionSet(uint16_t instruction);
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

    unsigned char   _Vx[16] = {0};
    unsigned short  _i = 0;
    unsigned short  _pc = 0;
    unsigned short  _stack[16] = {0};

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
    unsigned char   _delayTimer = 0;
    unsigned char   _soundTimer = 0;

    unsigned char  _sp = 0;
};