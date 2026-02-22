#include "chip8_cpu.h"
#include <random>

using namespace chip8;

void CPU::advancePC()
{
    _pc += 2;
}

void CPU::retractPC()
{
    _pc -= 2;
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

uint16_t CPU::getPC() const
{
    return _pc;
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
        if (screenY >= _display.HEIGHT)
            return;

        uint8_t spriteByte = _memory.read(getI() + row);

        for (int bit = 0; bit < 8; ++bit)
        {
            int screenX = x + bit;

            if (screenX >= _display.WIDTH)
                break;

            bool isSpritePixelOn = (spriteByte & (0x80 >> bit)) != 0;
            bool isScreenPixelOn = _display.getPixel(screenX, screenY);

            if (isSpritePixelOn)
            {
                if (isScreenPixelOn)
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
    uint16_t low = _memory.read(_pc + 1);

    advancePC();

    return (high << 8) | low;
}

void CPU::decodeExecute(uint16_t op)
{
    uint8_t opcodeGroup = (op >> 12) & 0x0F;
    uint8_t x = (op >> 8) & 0x0F;
    uint8_t y = (op >> 4) & 0x0F;
    uint8_t n = op & 0x0F;
    uint8_t nn = op & 0xFF;
    uint16_t nnn = op & 0x0FFF;

    switch (opcodeGroup)
    {
    case 0x00:
    {
        switch (nnn)
        {
        case 0x00E0:
        {
            _display.clearBuffers();
            break;
        }
        case 0x00EE:
        {
            jump(_stack.pop());
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

    case 0x02:
    {
        _stack.push(getPC());
        jump(nnn);
        break;
    }

    case 0x03:
    {
        if (getRegister(x) == nn)
        {
            advancePC();
        }
        break;
    }

    case 0x04:
    {
        if (getRegister(x) != nn)
        {
            advancePC();
        }
        break;
    }

    case 0x05:
    {
        if (n != 0)
            throw std::runtime_error("Invalid 5xyN opcode");

        if (getRegister(x) == getRegister(y))
        {
            advancePC();
        }

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

    case 0x09:
    {
        if (n != 0)
            throw std::runtime_error("Invalid 9xyN opcode");

        if (getRegister(x) != getRegister(y))
        {
            advancePC();
        }

        break;
    }

    case 0x08:
    {
        uint8_t Vx = getRegister(x);
        uint8_t Vy = getRegister(y);

        switch (n)
        {
        case 0x00:
        {
            setRegister(x, Vy);
            break;
        }

        case 0x01:
        {
            setRegister(x, Vx | Vy);
            break;
        }

        case 0x02:
        {
            setRegister(x, Vx & Vy);
            break;
        }

        case 0x03:
        {
            setRegister(x, Vx ^ Vy);
            break;
        }

        case 0x04:
        {
            uint16_t sum = Vx + Vy;

            setRegister(0x0F, sum > 255 ? 1 : 0);
            setRegister(x, static_cast<uint8_t>(sum));
            break;
        }

        case 0x05:
        {
            uint8_t diff = Vx - Vy;

            setRegister(0x0F, Vx >= Vy ? 1 : 0);
            setRegister(x, diff);
            break;
        }

        case 0x06:
        {
            uint8_t value = _useVyForShift ? Vy : Vx;
            uint8_t lsb = value & 0x01;

            value >>= 1;

            setRegister(x, value);
            setRegister(0x0F, lsb);
            break;
        }

        case 0x07:
        {
            uint8_t diff = Vy - Vx;

            setRegister(0x0F, Vy >= Vx ? 1 : 0);
            setRegister(x, diff);
            break;
        }

        case 0x0E:
        {
            uint8_t value = _useVyForShift ? Vy : Vx;
            uint8_t msb = value & 0x80;

            value <<= 1;

            setRegister(x, value);
            setRegister(0x0F, (msb ? 1 : 0));
            break;
        }

        default:
            throw std::runtime_error("Unknown 8xyN opcode");
        }
        break;
    }

    case 0x0A:
    {
        setI(nnn);
        break;
    }

    case 0x0B:
    {
        uint8_t base = _jumpUsesVx ? getRegister(x) : getRegister(0);
        jump(nnn + base);
        break;
    }

    case 0x0C:
    {
        uint8_t rand = _randByte(_randGen);
        rand &= nn;
        setRegister(x, rand);
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

    case 0x0E:
    {
        uint8_t Vx = getRegister(x);

        switch (nn)
        {
        case (0x9E):
        {
            if (_keypad.isDown(Vx))
            {
                advancePC();
            }
            break;
        }

        case (0xA1):
        {
            if (!_keypad.isDown(Vx))
            {
                advancePC();
            }
            break;
        }

        default:
            throw std::runtime_error("Unknown Exnn opcode");
        }
        break;
    }

    case (0x0F):
    {
        uint8_t Vx = getRegister(x);

        switch (nn)
        {
        case (0x07):
        {
            uint8_t delayValue = _delayTimer.get();
            setRegister(x, delayValue);
            break;
        }

        case (0x15):
        {
            _delayTimer.set(Vx);
            break;
        }

        case (0x18):
        {
            _soundTimer.set(Vx);
            break;
        }

        case (0x1E):
        {
            uint16_t sum = getI() + Vx;

            if (sum > 0x0FFF)
            {
                setRegister(0x0F, 1);
            }

            setI(sum);
            break;
        }

        case (0x0A):
        {
            auto key = _keypad.waitForKey();
            if (!key)
                retractPC();
            else
                setRegister(x, *key);
            break;
        }

        case (0x29):
        {
            uint16_t address = _memory.FONTSET_START_ADDRESS + Vx;
            setI(address);
            break;
        }

        case (0x33):
        {
            uint8_t value = Vx;
            uint8_t units = value % 10;
            value /= 10;
            uint8_t tens = value % 10;
            value /= 10;
            uint8_t hundreds = value % 10;

            _memory.write(getI(), hundreds);
            _memory.write(getI() + 1, tens);
            _memory.write(getI() + 2, units);
            break;
        }

        case (0x55):
        {
            for (int i = 0; i <= x; ++i)
            {
                uint8_t data = getRegister(i);

                _memory.write(getI() + i, data);
            }

            if (_legacyFx55Fx65Behavior)
            {
                setI(getI() + x + 1);
            }
            break;
        }

        case (0x65):
        {
            for (int i = 0; i <= x; ++i)
            {
                uint8_t data = _memory.read(getI() + i);

                setRegister(i, data);
            }

            if (_legacyFx55Fx65Behavior)
            {
                setI(getI() + x + 1);
            }
            break;
        }

        default:
            throw std::runtime_error("Unknown Fxnn opcode");
        }
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