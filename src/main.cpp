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
#else
# include <thread>
#endif

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

struct context
{
    Screen* screen = nullptr;
    CPU*    cpu;
    double    executionInterval;
};

// We need a global context object in EMSCripten, we might as well use it everywhere
context ctx;

// Set the clock resolution
typedef std::chrono::nanoseconds ClockResolution;
const double oneSecond = ClockResolution::period::den;
const std::string unitString  = "ns";

// Run at 60fps, also the rate of the timers!
const unsigned long frameInterval = oneSecond / 60;

void    drawframe() {
    // This function is called once per frame.
    // It should do all the frame-drawy things
    ctx.cpu->DecrementTimers();
    ctx.screen->Poll();
    for (unsigned instructionCount = 0 ; instructionCount < frameInterval / ctx.executionInterval ; instructionCount += 1){
        ctx.cpu->Tick();
    }
    ctx.screen->Draw();
}

int loadRom(const char* filename, double execInterval) {
    Memory* memory = new Memory();
    const int err = memory->Load(filename);
    if (err) { return err; }
    Keys* keys = new Keys();
    Screen* screen = new ScreenSDL();

    CPU* cpu = new CPU(memory, keys, screen);
    ctx.screen = screen;
    ctx.cpu = cpu;
    ctx.executionInterval = execInterval;
    std::cout << "[EMU] CPU interval is " << +execInterval << unitString << std::endl;
    std::cout << "[EMU] Frame interval is " << +frameInterval << unitString << std::endl;
    std::cout << "[EMU] Starting execution loop." << std::endl;
#ifdef EMSCRIPTEN
        emscripten_set_main_loop(drawframe, 0, 0);
        // We can't return or everything crashes!
        std::exit(0);
#else
    while (screen->isOpen()) {
        auto pre = std::chrono::high_resolution_clock::now();
        drawframe();
        auto post = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(ClockResolution(frameInterval) - std::chrono::duration_cast<ClockResolution>((post-pre)));
    }
    // The CPU deletes the memory, screen & keys
    delete cpu;
    std::cout << "[EMU] Exiting cleanly." << std::endl;
    return 0;
#endif
}

#ifdef EMSCRIPTEN

# ifdef __cplusplus
extern "C" {
# endif

EMSCRIPTEN_KEEPALIVE
void    emLoadRom(char *filename, unsigned instructionsPerSecond) {
    loadRom(filename, oneSecond / instructionsPerSecond);
}

EMSCRIPTEN_KEEPALIVE
void    emStop() {
    emscripten_cancel_main_loop();
    delete ctx.cpu;
    std::exit(0);
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
#ifndef EMSCRIPTEN
int main(int ac, const char **av) {
    if (ac < 2) {
        std::cerr << "Usage: " << av[0] << " <ROM path> [instructions per second]" << std::endl;
        return 1;
    }
    return loadRom(av[1], getExecInterval(ac, av));
}
#endif