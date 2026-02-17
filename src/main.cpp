#include <iostream>
#include "chip8_memory.h"
#include "chip8_display.h"

using namespace chip8;

struct Chip8
{
    Memory memory;
    Display display;

    Chip8(int displayScale = 10, 
        uint32_t pixelOffColor = 0xF0F6F0FF, 
        uint32_t pixelOnColor = 0x222323FF)
        : display(displayScale, pixelOffColor, pixelOnColor)
    {}

    Chip8(uint32_t pixelOffColor,
      uint32_t pixelOnColor)
    : display(10, pixelOffColor, pixelOnColor)
    {}

};

int main() {
    Chip8 chip8 = Chip8(0xC6BAACFF, 0x1E1C32FF);
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