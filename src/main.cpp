#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"

int main(int ac, const char **av) {
    if (ac < 2) {
        std::cout << "Usage: " << av[0] << " <ROM path>" << std::endl;
        return 1;
    }
    Memory memory;
    const int err = memory.Load(av[1]);
    if (err) { return err; }
    Keys keys;

    CPU cpu(&memory, &keys);
    cpu.ExecuteInstruction(0x00e0);
    cpu.ExecuteInstruction(0xa220);
    return 0;
}