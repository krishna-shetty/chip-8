#include "chip8_cpu.h"

using namespace chip8;

void CPU::advancePC()
{
    _pc += 2;
}

void CPU::skipNext()
{
    _pc += 4;
}

void CPU::jump(uint16_t address)
{
    _pc = address;
}

uint16_t CPU::getI() const
{
    return _I;
}

void CPU::setI(uint16_t value)
{
    _I = value;
}

void CPU::setRegister(uint8_t reg, uint8_t value)
{
    _registers[reg] = value;
}

uint8_t CPU::getRegister(uint8_t reg) const
{
    return _registers[reg];
}

void CPU::addToRegister(uint8_t reg, uint8_t value)
{
    _registers[reg] += value;
}

void CPU::drawSprite(uint8_t x, uint8_t y, uint8_t height)
{
    bool collision = false;

    for (int row = 0; row < height; ++row)
    {
        int screenY = y + row;
        if (screenY >= _display.HEIGHT) return;

        uint8_t spriteByte = _memory.read(getI() + row);

        for(int bit = 0; bit < 8; ++bit)
        {
            int screenX = x + bit;

            if(screenX >= _display.WIDTH) break;

            bool isSpritePixelOn = (spriteByte & (0x80 >> bit)) != 0;
            bool isScreenPixelOn = _display.getPixel(screenX, screenY);

            if(isSpritePixelOn)
            {
                if(isScreenPixelOn)
                {
                    _display.setPixel(screenX, screenY, false);
                    collision = true;
                }
                else
                {
                    _display.setPixel(screenX, screenY, true);
                }
            }
        }    
    }

    setRegister(0x0F, collision ? 1 : 0);
}

float CPU::getClockSpeed() const
{
    return _clockSpeed;
}

uint16_t CPU::fetch()
{
    uint16_t high = _memory.read(_pc);
    uint16_t low  = _memory.read(_pc + 1);

    advancePC();

    return (high << 8) | low;
}

void CPU::decodeExecute(uint16_t op)
{
    uint8_t opcodeGroup = (op >> 12)    & 0x0F;
    uint8_t  x          = (op >> 8)     & 0x0F;
    uint8_t  y          = (op >> 4)     & 0x0F;
    uint8_t  n          =  op           & 0x0F;
    uint8_t  nn         =  op           & 0xFF;
    uint16_t nnn        =  op           & 0x0FFF;

    switch(opcodeGroup)
    {
        case 0x00:
        {
            switch(nnn)
            {
                case 0x00E0:
                {
                    _display.clearBuffers();
                    break;
                }
                    
                default: 
                    throw std::runtime_error("Unknown 0x00 opcode");
            }
            break;
        }       
        case 0x01:
        {
            jump(nnn);
            break;
        }      
        case 0x06:
        {
            setRegister(x, nn);
            break;
        }
            
        case 0x07:
        {
            addToRegister(x, nn);
            break;
        }
            
        case 0x0A:
        {
            setI(nnn);
            break;
        }
            
        case 0x0D:
        {
            uint8_t Vx = getRegister(x);
            uint8_t Vy = getRegister(y);
            Vx &= 63;
            Vy &= 31;
            drawSprite(Vx, Vy, n);
            break;
        }
            
        default:
            throw std::runtime_error("Unrecognized opcode");
            break;
    }
}

void CPU::cycle()
{
    uint16_t op = fetch();
    decodeExecute(op);
}