#include <bits/stdint-uintn.h>
#include <exception>
#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"
#include "Screen/SDL/SDL.hh"
#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

struct context
{
    Screen* screen;
    CPU*    cpu;
    timestamp* lastTimerUpdate;
    timestamp* lastExecution;
    unsigned    executionInterval;
};

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

const unsigned timerUpdateInterval = 1000 / 60;

void    mainloop(void* arg) {
    context *ctx = static_cast<context*>(arg);
    ctx->screen->Poll();
    auto now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - *ctx->lastTimerUpdate).count() > timerUpdateInterval) {
        *ctx->lastTimerUpdate = now;
        ctx->cpu->DecrementTimers();
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - *ctx->lastExecution).count() > ctx->executionInterval) {
        *ctx->lastExecution = now;
        ctx->cpu->Tick();
    }
    ctx->screen->Draw();
}

#ifdef EMSCRIPTEN
# include <emscripten.h>

void    loadROM(uint8_t buffer) {
    // Somehow load the ROM here
    // emscripten_set_main_loop_arg(mainloop, &ctx, -1, 1);
}
#endif


int main(int ac, const char **av) {
#ifdef EMSCRIPTEN
    return 0;
#endif
    if (ac < 2) {
        std::cout << "Usage: " << av[0] << " <ROM path> [instructions per second]" << std::endl;
        return 1;
    }
    Memory memory;
    const int err = memory.Load(av[1]);
    if (err) { return err; }
    Keys keys;
    Screen* screen = new SDL();

    CPU cpu(&memory, &keys, screen);
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();
    auto lastExecution = std::chrono::high_resolution_clock::now();
    std::cout << '\a' << std::endl;
    context ctx;
    ctx.screen = screen;
    ctx.cpu = &cpu;
    ctx.lastTimerUpdate = &lastTimerUpdate;
    ctx.lastExecution = &lastExecution;
    ctx.executionInterval = getExecInterval(ac, av);
    while (screen->isOpen()) {
        mainloop(&ctx);
    }
    delete screen;
    return 0;
}