#include <iostream>
#include <chrono> 

#include "chip8_memory.h"
#include "chip8_display.h"
#include "chip8_stack.h"
#include "chip8_beeper.h"
#include "chip8_timer.h"
#include "chip8_keypad.h"
#include "chip8_cpu.h"

using namespace chip8;

class Chip8
{
    public:
        Memory memory;
        
        Display display;
        
        Stack stack;
        
        Beeper beeper;
        
        Timer delayTimer;
        Timer soundTimer;

        Keypad keypad;

        CPU cpu;

        bool running = true;

        Chip8(uint32_t pixelOffColor = 0x222323FF,
          uint32_t pixelOnColor = 0xF0F6F0FF,
          int displayScale = 10,
          float cpuClockHz = 500.0f)   
        : memory(),
          display(pixelOffColor, pixelOnColor, displayScale),
          stack(),
          delayTimer(),
          soundTimer(),
          keypad(),
          cpu(cpuClockHz, memory, display, stack, keypad, soundTimer, delayTimer)  
    {
    }
};

void handleInput(Chip8&);
void updateTimers(Chip8&, float);
void updateAudio(Chip8&);
void render(Chip8&);
void tick(Chip8&, float);

int main() 
{
    if (!(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    
    {
        Chip8 chip8(0x1e1c32ffu, 0xc6baacffu, 10);

        auto prev = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();

        float dt = std::chrono::duration<float>(now - prev).count();
        
        chip8.memory.loadROM("assets/ibm.ch8");
        
        while (chip8.running)
        { 
            now = std::chrono::steady_clock::now();
            dt = std::chrono::duration<float>(now - prev).count();
            prev = now;

            handleInput(chip8);
            tick(chip8, dt); 
            updateTimers(chip8, dt);
            updateAudio(chip8);
            render(chip8);

            SDL_Delay(1);
        }
    }

    SDL_Quit();
    return 0;
}

void handleInput(Chip8& chip8)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            chip8.running = false;

        chip8.keypad.handleEvent(e);
    }
}

void updateTimers(Chip8& chip8, float dt)
{
    static float accumulator = 0.0f;
    const float timerInterval = 1.0f / Display::FRAMERATE;

    accumulator += dt;

    while (accumulator >= timerInterval)
    {
        chip8.delayTimer.decrement();
        chip8.soundTimer.decrement();
        accumulator -= timerInterval;
    }
}

void updateAudio(Chip8& chip8)
{
    static bool wasBeeping = false;

    bool isBeeping = chip8.soundTimer.get() > 0;

    if (isBeeping && !wasBeeping)
        chip8.beeper.play();
    else if (!isBeeping && wasBeeping)
        chip8.beeper.stop();

    wasBeeping = isBeeping;

    chip8.beeper.update(); 
}

void render(Chip8& chip8)
{
    chip8.display.draw();
}

void tick(Chip8& chip8, float dt)
{
    static float accumulator = 0.0f;
    const float cycleTime = 1.0f / chip8.cpu.getClockSpeed();

    accumulator += dt;

    if(accumulator >= cycleTime)
    {
        chip8.cpu.cycle();
        accumulator -= cycleTime;
    }
}