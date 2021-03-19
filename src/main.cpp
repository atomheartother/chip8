#include <cstdlib>
#include <exception>
#include <iostream>
#include "CPU/CPU.hh"
#include "Keys/Keys.hh"
#include "Memory/Memory.hh"
#include "Screen/SDL/SDL.hh"
#include <chrono>
#include <ratio>
#ifdef EMSCRIPTEN
# include <emscripten.h>
# include <cstdlib>
#endif

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

struct context
{
    Screen* screen;
    CPU*    cpu;
    timestamp*  lastTimerUpdate;
    timestamp*  lastExecution;
    timestamp*  lastFrame;
    double    executionInterval;
};

// Set the clock resolution
typedef std::chrono::nanoseconds ClockResolution;
const double oneSecond = ClockResolution::period::den;
const std::string unitString  = "ns";

const double timerUpdateInterval = oneSecond / 60;
const double frameInterval = oneSecond / 60;

void    mainloop(void* arg) {
    context *ctx = static_cast<context*>(arg);
    ctx->screen->Poll();
    const auto preTimers = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<ClockResolution>(preTimers - *ctx->lastTimerUpdate).count() > timerUpdateInterval) {
        *ctx->lastTimerUpdate = preTimers;
        ctx->cpu->DecrementTimers();
    }
    const auto preCpu = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<ClockResolution>(preCpu - *ctx->lastExecution).count() > ctx->executionInterval) {
        *ctx->lastExecution = preCpu;
        ctx->cpu->Tick();
    }
    const auto preDraw = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<ClockResolution>(preDraw - *ctx->lastFrame).count() > frameInterval) { 
        *ctx->lastFrame = preDraw;
        ctx->screen->Draw();
    }
}

int loadRom(const char* filename, double execInterval) {
    Memory* memory = new Memory();
    const int err = memory->Load(filename);
    if (err) { return err; }
    Keys* keys = new Keys();
    Screen* screen = new ScreenSDL();

    CPU* cpu = new CPU(memory, keys, screen);
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();
    auto lastExecution = std::chrono::high_resolution_clock::now();
    auto lastFrame = std::chrono::high_resolution_clock::now();
    context ctx;
    ctx.screen = screen;
    ctx.cpu = cpu;
    ctx.lastTimerUpdate = &lastTimerUpdate;
    ctx.lastExecution = &lastExecution;
    ctx.executionInterval = execInterval;
    ctx.lastFrame = &lastFrame;
    std::cout << "[EMU] CPU interval is " << +execInterval << unitString << std::endl;
    std::cout << "[EMU] Frame interval is " << +frameInterval << unitString << std::endl;
    std::cout << "[EMU] 60Hz clock interval is " << +timerUpdateInterval << unitString << std::endl;
    std::cout << "[EMU] Starting execution loop." << std::endl;
#ifdef EMSCRIPTEN
        emscripten_set_main_loop_arg(mainloop, &ctx, 10000, 0);
        // We can't return or everything crashes!
        std::exit(0);
#else
    while (screen->isOpen()) {
        mainloop(&ctx);
    }
    delete cpu;
    delete screen;
    delete keys;
    delete memory;
    std::cout << "[EMU] Exiting cleanly." << std::endl;
    return 0;
#endif
}

#ifdef EMSCRIPTEN

# ifdef __cplusplus
extern "C" {
# endif

EMSCRIPTEN_KEEPALIVE void    emLoadRom(char *filename) {
    loadRom(filename, oneSecond / 500);
    free(filename);
}

# ifdef __cplusplus
}
# endif
#endif

double getExecInterval(int ac, const char** av) {
    unsigned instructionsPerSecond = 350;
    if (ac > 2) {
        try {
            instructionsPerSecond = std::stoi(av[2]);
        } catch (const std::exception& e) {
            std::cerr << "Can't convert " << av[2] << " to a number, using default value of " << instructionsPerSecond << std::endl;
        }
    }
    return oneSecond / instructionsPerSecond;
}

int main(int ac, const char **av) {
    if (ac < 2) {
        std::cerr << "Usage: " << av[0] << " <ROM path> [instructions per second]" << std::endl;
        return 1;
    }
    return loadRom(av[1], getExecInterval(ac, av));
}