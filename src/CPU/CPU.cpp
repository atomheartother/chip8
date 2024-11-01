#include "CPU.hh"
#include <functional>
#include <iostream>

CPU::CPU(Memory *mem, const Keys *keys, Screen* screen) : _memory(mem), _keys(keys), _screen(screen) {
        std::cout << "[CPU] CPU initialized." << std::endl;
}

CPU::~CPU() {
    delete _memory;
    delete _keys;
    delete _screen;
    std::cout << "[CPU] Terminating CPU." << std::endl;
}

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
    _pc += (_V[getX(instruction)] == getKK(instruction)) << 1;
}

void CPU::SneByte(uint16_t instruction) {
    _pc += (_V[getX(instruction)] != getKK(instruction)) << 1;
}

void CPU::SeReg(uint16_t instruction) {
        _pc += (_V[getX(instruction)] == _V[getY(instruction)]) << 1;
}

void CPU::Ld(uint16_t instruction) {
    _V[getX(instruction)] = getKK(instruction);
}

void CPU::Add(uint16_t instruction) {
    _V[getX(instruction)] += getKK(instruction);
}

void CPU::Op(uint16_t instruction) {
    const uint8_t x = getX(instruction);
    const uint8_t y = getY(instruction);

    // Branchless - compute all possible operations
    uint8_t res[9];
    uint8_t carryFlag[9] = { _V[0xF] };

    const uint16_t add = _V[x] + _V[y];
    // LD
    res[0] = _V[y];
    // OR
    res[1] = _V[x] | _V[y];
    // AND
    res[2] = _V[x] & _V[y];
    // XOR
    res[3] = _V[x] ^ _V[y];
    // ADD
    res[4] = add & 0xFF;
    carryFlag[4] = (add & 0xFF00) > 0;
    // SUB
    res[5] = _V[x] - _V[y];
    carryFlag[5] = _V[x] >= _V[y];
    // SHR
    res[6] = _V[x] >> 1;
    carryFlag[6] = _V[x] & 1;
    // SUBN
    res[7] = _V[y] - _V[x];
    carryFlag[7] = _V[y] >= _V[x];
    // SHL
    res[8] = _V[x] << 1;
    carryFlag[8] = _V[x] >> 7;

    const uint8_t operation = instruction & 0xF;
    // Compute the destination register (x if operation anything else than 7)
    // The res reg we use is always == to the opcode unless the opcode is 0xe, in which case it's 8 (0xe-6)
    const uint8_t srcIndex = operation - (6 & ((operation != 0xe) - 1));
    _V[x] = res[srcIndex];
    // The carry flag is equal to itself (carryFlag[0], or carryFlag[operation-operation]) if operation < 4
    // Otherwise we grab the result from carryFlag[operation-0] ;D
    _V[0xF] = carryFlag[srcIndex & ((operation < 4) - 1)];
    // And that's how you make unlegible VERY fast code!
}

void CPU::SneReg(uint16_t instruction) {
    _pc += (_V[getX(instruction)] != _V[getY(instruction)]) << 1;
}

void CPU::SetI(uint16_t instruction) {
    _i = nnn(instruction);
}

void CPU::JpV0(uint16_t instruction) {
    _pc = _V[0] + nnn(instruction);
}

void CPU::Rnd(uint16_t instruction) {
    _V[getX(instruction)] = std::rand() & getKK(instruction);
}

void CPU::Drw(uint16_t instruction) {
    const uint8_t* sprite = _memory->Sprite(_i);
    _V[0xF] = _screen->DrawSprite(_V[getX(instruction)], _V[getY(instruction)], sprite, instruction & 0xF);
}

void CPU::Skips(uint16_t instruction) {
    const uint8_t x = getX(instruction);
    const uint8_t kkk = getKK(instruction);
    const bool pressed = _keys->isKeyPressed(_V[x]);
    // If kkk === 0x9e we skip if the key is pressed
    // If kkk === 0xa1 we check is the key is not pressed
    _pc += (((kkk == 0x9e) & pressed) + ((kkk == 0xa1) & !pressed)) << 1;
}

void CPU::Fxx(uint16_t instruction) {
    // Fxx are special methods all applied to register x
    const uint8_t x = getX(instruction);
    switch (getKK(instruction)) {
        case 0x07:
            _V[x] = _delayTimer;
            break;
        case 0x0a: {
            uint8_t k = _keys->firstKeyPressed();
            uint8_t noKeys = k == NO_KEYS_PRESSED;
            _pc -= noKeys << 1;
            _V[x] = k & (noKeys - 1);
            break;
        }
        case 0x15:
            _delayTimer = _V[x];
            break;
        case 0x18:
            _soundTimer = _V[x];
            if (_soundTimer > 0) {
                _screen->Beep();
            }
            break;
        case 0x1e:
            _i += _V[x];
            break;
        case 0x29:
            _i = _memory->HexSpriteAddress(_V[x]);
            break;
        case 0x33:
            _memory->WriteB(_V[x] / 100, _i);
            _memory->WriteB((_V[x] / 10) % 10, _i + 1);
            _memory->WriteB(_V[x] % 10, _i + 2);
            break;
        case 0x55:
            _memory->Write(_V, _i, x + 1);
            break;
        case 0x65:
            _memory->Read(_V, _i, x + 1);
            break;
        default:
            std::cout << "Unknown Fx sub-opcode: " << +getKK(instruction) << std::endl;
    }
}
