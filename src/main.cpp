#include <cstdlib>
#include <exception>
#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"
#include "Screen/SDL/SDL.hh"
#include <chrono>
#ifdef EMSCRIPTEN
# include <emscripten.h>
#endif

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

struct context
{
    Screen* screen;
    CPU*    cpu;
    timestamp* lastTimerUpdate;
    timestamp* lastExecution;
    unsigned    executionInterval;
};

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

int loadRom(const char* filename, unsigned execInterval) {
    Memory* memory = new Memory();
    const int err = memory->Load(filename);
    if (err) { return err; }
    Keys* keys = new Keys();
    Screen* screen = new ScreenSDL();

    CPU* cpu = new CPU(memory, keys, screen);
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();
    auto lastExecution = std::chrono::high_resolution_clock::now();
    std::cout << '\a' << std::endl;
    context ctx;
    ctx.screen = screen;
    ctx.cpu = cpu;
    ctx.lastTimerUpdate = &lastTimerUpdate;
    ctx.lastExecution = &lastExecution;
    ctx.executionInterval = execInterval;
#ifdef EMSCRIPTEN
        emscripten_set_main_loop_arg(mainloop, &ctx, -1, 0);
#else
    while (screen->isOpen()) {
        mainloop(&ctx);
    }
    delete cpu;
    delete screen;
    delete keys;
    delete memory;
    std::cout << "Exiting cleanly." << std::endl;
    return 0;
#endif
}

#ifdef EMSCRIPTEN

# ifdef __cplusplus
extern "C" {
# endif

EMSCRIPTEN_KEEPALIVE void    emLoadRom(char *filename) {
    loadRom(filename, 1000 / 350);
    free(filename);
}

# ifdef __cplusplus
}
# endif
#endif

unsigned getExecInterval(int ac, const char** av) {
    unsigned instructionsPerSecond = 350;
    if (ac > 2) {
        try {
            instructionsPerSecond = std::stoi(av[2]);
        } catch (const std::exception& e) {
            std::cerr << "Can't convert " << av[2] << " to a number, using default value of " << instructionsPerSecond << std::endl;
        }
    }
    return 1000 / instructionsPerSecond;
}

int main(int ac, const char **av) {
    if (ac < 2) {
        std::cerr << "Usage: " << av[0] << " <ROM path> [instructions per second]" << std::endl;
        return 1;
    }
    return loadRom(av[1], getExecInterval(ac, av));
}