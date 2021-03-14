#include "CPU.hh"
#include <bits/stdint-uintn.h>
#include <functional>
#include <cstdlib>
#include <iostream>

CPU::CPU(Memory *mem, const Keys *keys, Screen* screen) : _memory(mem), _keys(keys), _screen(screen) {}

void CPU::ExecuteInstruction(uint16_t instruction) {
    const uint8_t instructionCode = instruction >> 12;
    std::invoke(_instructions[instructionCode], this, instruction);
}

void CPU::Tick() {
    uint8_t instruction[2];
    _memory->Read(instruction, _pc, 2);
    _pc += 2;
    ExecuteInstruction((instruction[0] << 8) + instruction[1]);
}

void CPU::DecrementTimers() {
    if (_delayTimer > 0) {
        _delayTimer -= 1;
    }
    if (_soundTimer > 0) {
        _soundTimer -= 1;
    }
}

void CPU::ClsRet(uint16_t instruction) {
    if (instruction == 0x00E0) {
        _screen->Clear();
    } else if (instruction == 0x00EE && _sp > 0) {
        _sp -= 1;
        _pc = _stack[_sp];
    }
}

void CPU::Jp(uint16_t instruction) {
    _pc = nnn(instruction);
}

void CPU::Call(uint16_t instruction) {
    _stack[_sp] = _pc;
    _sp += 1;
    _pc = nnn(instruction);
}

void CPU::SeByte(uint16_t instruction) {
    if (_Vx[getX(instruction)] == getKK(instruction)) {
        _pc += 2;
    }
}

void CPU::SneByte(uint16_t instruction) {
    if (_Vx[getX(instruction)] != getKK(instruction)) {
        _pc += 2;
    }
}

void CPU::SeReg(uint16_t instruction) {
    if (_Vx[getX(instruction)] == _Vx[getY(instruction)]) {
        _pc += 2;
    }
}

void CPU::Ld(uint16_t instruction) {
    _Vx[getX(instruction)] = getKK(instruction);
}

void CPU::Add(uint16_t instruction) {
    _Vx[getX(instruction)] += getKK(instruction);
}

void CPU::Op(uint16_t instruction) {
    const uint8_t x = getX(instruction);
    const uint8_t y = getY(instruction);

    switch (instruction & 0xF) {
        case 0:
            _Vx[x] = _Vx[y];
            break;
        case 1:
            _Vx[x] |= _Vx[y];
            break;
        case 2:
            _Vx[x] &= _Vx[y];
            break;
        case 3:
            _Vx[x] ^= _Vx[y];
            break;
        case 4: {
            uint16_t res = _Vx[x] + _Vx[y];
            _Vx[x] = res & 0xFF;
            _Vx[0xF] = (res >> 8) ? 1 : 0;
            break;
        }
        case 5:
            _Vx[0xF] = (_Vx[x] > _Vx[y]);
            _Vx[x] -= _Vx[y];
            break;
        case 6:
            _Vx[0xF] = _Vx[x] & 1;
            _Vx[x] >>= 1;
            break;
        case 7:
            _Vx[0xF] = (_Vx[x] < _Vx[y]);
            _Vx[y] -= _Vx[x];
            break;
        case 0xe:
            _Vx[0xF] = _Vx[x] >> 7;
            _Vx[x] <<= 1;
    }
}

void CPU::SneReg(uint16_t instruction) {
    if (_Vx[getX(instruction)] == _Vx[getY(instruction)]) {
        _pc += 2;
    }
}

void CPU::SetI(uint16_t instruction) {
    _i = nnn(instruction);
}

void CPU::JpV0(uint16_t instruction) {
    _pc = _Vx[0] + nnn(instruction);
}

void CPU::Rnd(uint16_t instruction) {
    _Vx[getX(instruction)] = std::rand() & getKK(instruction);
}

void CPU::Drw(uint16_t instruction) {
    const uint8_t* sprite = _memory->Sprite(_i);
    _Vx[0xF] = _screen->DrawSprite(_Vx[getX(instruction)], _Vx[getY(instruction)], sprite, instruction & 0xF);
}

void CPU::Skips(uint16_t instruction) {
    const uint8_t x = getX(instruction);
    switch (getKK(instruction)) {
        case 0x9e:
            if (_keys->isKeyPressed(_Vx[x])) {
                _pc += 2;
            }
            break;
        case 0xa1:
            if (!_keys->isKeyPressed(_Vx[x])) {
                _pc += 2;
            }
            break; 
    }
}

void CPU::Fxx(uint16_t instruction) {
    // Fxx are special methods all applied to register x
    const uint8_t x = getX(instruction);
    switch (getKK(instruction)) {
        case 0x07:
            _Vx[x] = _delayTimer;
            break;
        case 0x0a: {
            uint8_t k = _keys->firstKeyPressed();
            if (k == NO_KEYS_PRESSED) {
                _pc -= 2;
            } else {
                _Vx[x] = k;
            }
            break;
        }
        case 0x15:
            _delayTimer = _Vx[x];
            break;
        case 0x18:
            _soundTimer = _Vx[x];
            break;
        case 0x1e:
            _i += _Vx[x];
            break;
        case 0x29:
            _i = _memory->HexSpriteAddress(_Vx[x]);
            break;
        case 0x33:
            _memory->WriteB(_Vx[x] / 100, _i);
            _memory->WriteB((_Vx[x] / 10) % 10, _i + 1);
            _memory->WriteB(_Vx[x] % 10, _i + 2);
            break;
        case 0x55:
            _memory->Write(_Vx, _i, x);
            break;
        case 0x65:
            _memory->Read(_Vx, _i, x);
            break;
    }
}
