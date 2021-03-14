#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"
#include "Screen/SFML/SFML.hh"
#include <chrono>


int main(int ac, const char **av) {
    if (ac < 2) {
        std::cout << "Usage: " << av[0] << " <ROM path>" << std::endl;
        return 1;
    }
    Memory memory;
    const int err = memory.Load(av[1]);
    if (err) { return err; }
    Keys keys;
    Screen* screen = new SFML();

    CPU cpu(&memory, &keys, screen);
    auto lastUpdate = std::chrono::high_resolution_clock::now();
    while (screen->isOpen()) {
        cpu.Tick();
        screen->Poll();
        auto sinceLast = std::chrono::high_resolution_clock::now() - lastUpdate;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(sinceLast).count() > 1000 / 60) {
            cpu.DecrementTimers();
            screen->Draw();
        }
    }
    delete screen;
    return 0;
}