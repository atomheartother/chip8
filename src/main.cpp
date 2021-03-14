#include <exception>
#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"
#include "SFML/System/Thread.hpp"
#include "Screen/SFML/SFML.hh"
#include "Screen/Screen.hh"
#include <chrono>
#include <string>

void drawThread(Screen* screen) {
    screen->Activate();
    while (screen->isOpen()) {
        screen->Draw();
    }
}

unsigned getExecInterval(int ac, const char** av) {
    unsigned instructionsPerSecond = 350;
    if (ac > 2) {
        try {
            instructionsPerSecond = std::stoi(av[2]);
        } catch (const std::exception& e) {
            std::cout << "Can't convert " << av[2] << " to a number, using default value of " << instructionsPerSecond << std::endl;
        }
    }
    return 1000 / instructionsPerSecond;
}

int main(int ac, const char **av) {
    if (ac < 2) {
        std::cout << "Usage: " << av[0] << " <ROM path> [instructions per second]" << std::endl;
        return 1;
    }
    Memory memory;
    const int err = memory.Load(av[1]);
    if (err) { return err; }
    Keys keys;
    Screen* screen = new SFML();

    CPU cpu(&memory, &keys, screen);
    screen->Deactivate();
    sf::Thread thread(&drawThread, screen);
    thread.launch();
    const unsigned timerUpdateInterval = 1000 / 60;
    const unsigned executionInterval = getExecInterval(ac, av);
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();
    auto lastExecution = std::chrono::high_resolution_clock::now();
    while (screen->isOpen()) {
        screen->Poll();
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastExecution).count() > executionInterval) {
            cpu.Tick();
            lastExecution = std::chrono::high_resolution_clock::now();
        }
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimerUpdate).count() > timerUpdateInterval) {
            cpu.DecrementTimers();
            lastTimerUpdate = std::chrono::high_resolution_clock::now();
        }
    }
    thread.wait();
    delete screen;
    return 0;
}