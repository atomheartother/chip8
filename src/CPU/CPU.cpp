#include "CPU.hh"
#include <bits/stdint-uintn.h>
#include <functional>

#include <iostream>

void CPU::ExecuteInstruction(uint16_t instruction) {
    const uint8_t instructionCode = instruction / 4096;
    std::invoke(_instructions[instructionCode], this, instruction);
}

void CPU::ClsRet(uint16_t instruction) {
    std::cout << "Executing 0xx" << std::endl;
}

void CPU::Jp(uint16_t instruction) {

}

void CPU::Call(uint16_t instruction) {

}

void CPU::SeByte(uint16_t instruction) {

}

void CPU::SneByte(uint16_t instruction) {

}

void CPU::SeReg(uint16_t instruction) {

}

void CPU::Ld(uint16_t instruction) {

}

void CPU::Add(uint16_t instruction) {

}

void CPU::Op(uint16_t instruction) {

}

void CPU::SneReg(uint16_t instruction) {

}

void CPU::SetI(uint16_t instruction) {
    std::cout << "Executing SetI" << std::endl;

}

void CPU::JpV0(uint16_t instruction) {

}

void CPU::Rnd(uint16_t instruction) {

}

void CPU::Drw(uint16_t instruction) {

}

void CPU::Skips(uint16_t instruction) {

}

void CPU::Fxx(uint16_t instruction) {

}
