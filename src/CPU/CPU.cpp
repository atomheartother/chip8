#include "CPU.hh"
#include <bits/stdint-uintn.h>
#include <functional>
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
    _delayTimer -= 1 & (_delayTimer > 0);
    if (_soundTimer > 0) {
        _soundTimer -= 1;
        if (_soundTimer == 0) {
            _screen->StopBeep();
        }
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

    // Branchless - compute all possible operations
    uint8_t res[9];
    uint8_t carryFlag[9] = { _Vx[0xF] };

    res[0] = _Vx[y];
    res[1] = _Vx[x] | _Vx[y];
    res[2] = _Vx[x] & _Vx[y];
    res[3] = _Vx[x] ^ _Vx[y];
    const uint16_t add = _Vx[x] + _Vx[y];
    res[4] = add & 0xFF;
    carryFlag[4] = add >> 8 > 0;
    res[5] = _Vx[x] - _Vx[y];
    carryFlag[5] = _Vx[x] > _Vx[y];
    res[6] = _Vx[x] >> 1;
    carryFlag[6] = _Vx[x] & 1;
    res[7] = _Vx[y] - _Vx[x];
    carryFlag[7] = _Vx[y] > _Vx[x];
    res[8] = _Vx[x] << 1;
    carryFlag[8] = _Vx[x] >> 7;


    const uint8_t operation = instruction & 0xF;
    // Compute the destination register (x if operation anything else than 8)
    // The res reg we use is always == to the opcode unless the opcode is 0xe, in which case it's 8 (0xe-6)
    _Vx[x - ((x-y) & ((operation != 7) - 1)) ] = res[operation - (6 & (operation == 0xe))];
    // The carry flag is equal to itself (carryFlag[0], or carryFlag[operation-operation]) if operation < 4
    // Otherwise we grab the result from carryFlag[operation-0] ;D
    _Vx[0xF] = carryFlag[operation - (operation & ((operation >= 4) - 1) )];
    // And that's how you make unlegible VERY fast code!
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
            if (_soundTimer > 0) {
                _screen->Beep();
            }
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
