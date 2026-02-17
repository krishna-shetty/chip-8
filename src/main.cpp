#include <iostream>
#include "chip8_memory.h"
#include "chip8_display.h"
#include "chip8_stack.h"

using namespace chip8;

struct Chip8
{
    Memory memory;
    Display display;
    Stack stack;

    Chip8(
        uint32_t pixelOffColor = 0x222323FF, 
        uint32_t pixelOnColor = 0xF0F6F0FF,
        int displayScale = 10)
        : display(pixelOffColor, pixelOnColor, displayScale)
    {}
};

int main() {
    Chip8 chip8(0x1e1c32ffu, 0xc6baacffu);
    chip8.display.clearBuffers();
    chip8.display.draw();

    uint16_t address = 0x050; 

    uint8_t value = chip8.memory.read(address);
    std::cout << "Value at address 0x" << std::hex << address << ": 0x" << std::hex << static_cast<int>(value) << std::endl;

    bool running = true;

    while (running)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
    }

    SDL_Quit();
    return 0;
}