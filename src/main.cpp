#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"
#include "Screen/Screen.hh"
#include <unistd.h>

int main(int ac, const char **av) {
    if (ac < 2) {
        std::cout << "Usage: " << av[0] << " <ROM path>" << std::endl;
        return 1;
    }
    Memory memory;
    const int err = memory.Load(av[1]);
    if (err) { return err; }
    Keys keys;
    Screen screen;

    CPU cpu(&memory, &keys, &screen);
    for (int i=0 ; i < 150 ; i += 1) {
        cpu.Tick();
        // screen.Draw();
    }
    screen.Draw();
    return 0;
}