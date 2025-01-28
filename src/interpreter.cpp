#include <fstream>
#include <cstdint>

#include <string.h>
#include <iostream>

#include "interpreter.h"

constexpr uint8_t font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Interpreter::Interpreter()
{
    memcpy(_memory + 0x050, font, 80);
}

bool Interpreter::load_rom(char *path)
{
    std::ifstream byte_stream(path);

    if (!byte_stream.is_open()) {
        byte_stream.close();
        return false;
    }

    byte_stream.read((char *) _memory + 0x200, MEMORY_SIZE - 0x200);

    // The entire ROM didn't fit in memory
    if (!byte_stream.eof()) {
        byte_stream.close();
        return false;
    }

    byte_stream.close();
    return true;
}

void Interpreter::execute()
{
    uint16_t instruction = (_memory[_pc] << 8) + _memory[_pc + 1];
    _pc += 2;

    switch (instruction & 0xF000) {
        case 0x0000:
            {
                switch (instruction & 0x00FF) {
                    case 0x00E0: // 0x00E0 => clear the screen
                        for (uint8_t i = 0; i < 32; i++)
                        {
                            screen[i] = 0;
                        }
                        break;
                    case 0x0EE: // 0x00EE => return from subroutine
                        _pc = _substack.top();
                        _substack.pop();
                        break;
                }
            }
            break;
        case 0x1000: // 0x1NNN => jump
            _pc = instruction & 0x0FFF;
            break;
        case 0x2000: // 0x2NNN => call subroutine at NNN
            _substack.push(_pc);
            _pc = instruction & 0x0FFF;
            break;
        case 0x3000: // 3XNN => PC +=2 if VX == NN
            if (_registers[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF))
            {
                _pc += 2;
            }
            break;
        case 0x4000: // 4XNN => PC +=2 if VX != NN
            if (_registers[(instruction & 0x0F00) >> 8] != (instruction & 0x00FF))
            {
                _pc += 2;
            }
            break;
        case 0x5000: // 5XY0 => PC +=2 if VX == VY
            if (_registers[(instruction & 0x0F00) >> 8] == _registers[(instruction & 0x00F0) >> 4])
            {
                _pc += 2;
            }
            break;
        case 0x6000: // 0x6XNN => set register VX to NN
            _registers[(instruction & 0x0F00) >> 8] = instruction & 0x00FF;
            break;
        case 0x7000: // 0x7XNN => increment register VX by NN
            _registers[(instruction & 0x0F00) >> 8] += instruction & 0x00FF;
            break;
        case 0x8000:
            {
                uint8_t x = (instruction & 0x0F00) >> 8;
                uint8_t y = (instruction & 0x00F0) >> 4;
                switch (instruction & 0x000F) {
                case 0x0000: // 8XY0 => set the value of VX to the value of VY 
                    _registers[x] = _registers[y];
                    break;
                case 0x0001: // 8XY1 => set VX to VX binary or VY
                    _registers[x] = _registers[x] | _registers[y];
                    break;
                case 0x0002: // 8XY2 => set VX to VX binary and VY
                    _registers[x] = _registers[x] & _registers[y];
                    break;
                case 0x0003: // 8XY3 => set VX to VX binary xor VY
                    _registers[x] = _registers[x] ^ _registers[y];
                    break;
                case 0x0004: // 8XY4 => set VX to VX + VY
                    _registers[x] = _registers[x] + _registers[y];
                    break;
                case 0x0005: // 8XY5 => set VX to VX - VY
                    _registers[x] = _registers[x] - _registers[y];
                    break;
                case 0x0006: // 8XY6 => (maybe set VX to VY) VX >> 1
                    _registers[x] >>= 1;
                    break;
                case 0x0007: // 8XY7 => set VX to VY - VX
                    _registers[x] = _registers[y] - _registers[x];
                    break;
                case 0x000E: // 8XYE => (maybe set VX to VY) VX << 1
                    _registers[x] <<= 1;
                    break;
                }
            }
            break;
        case 0x9000: // 9XY0 => PC +=2 if VX == VY
            if (_registers[(instruction & 0x0F00) >> 8] != _registers[(instruction & 0x00F0) >> 4])
            {
                _pc += 2;
            }
            break;
        case 0xA000: // 0xANNN => set index register to NNN
            _index = instruction & 0x0FFF;
            break;
        case 0xB000: // 0xBNNN => jump to NNN + V0
            // This should be configurable as BXNN for CHIP-48 and SUPER-CHIP
            _pc = (instruction & 0x0FFF) + _registers[0];
            break;
        case 0xC000: // 0xCXNN => set VX to rand binary and NN
            _registers[(instruction & 0x0F00) >> 8] = instruction & 0x00FF & rand();
            break;
        case 0xD000: // 0xDXYN => draw
            {
                display_changed = true;
                uint8_t x = _registers[(instruction & 0x0F00) >> 8] % DISPLAY_WIDTH;
                uint8_t y = _registers[(instruction & 0x00F0) >> 4] % DISPLAY_HEIGHT;
                uint8_t n = std::min(instruction & 0x000F, DISPLAY_HEIGHT - y);

                _registers[0xF] = 0;

                for (int i = 0; i < n; i++)
                {
                    uint64_t sprite_row = _memory[_index + i];
                    
                    int shift_left_by = DISPLAY_WIDTH - x - 8;
                    if (shift_left_by >= 0)
                    {
                        sprite_row <<= shift_left_by;
                    }
                    else
                    {
                        sprite_row >>= -shift_left_by;
                    }

                    if ((screen[y + i] & sprite_row) != 0)
                    {
                        _registers[0xF] = 1;
                    }
                    
                    screen[y + i] ^= sprite_row;
                }
            }

            break;
        case 0xE000:
            {
                uint8_t vx_val = _registers[(instruction & 0x0F00) >> 8];
                switch (instruction & 0x00FF) {
                    case 0x009E: // Skip an instruction if key equal to value in VX is pressed
                        if (keyboard.is_pressed(vx_val));
                        {
                            _pc += 2;
                        }
                        break;
                    case 0x00A1: // Skip an instruction if key equal to value in VX isn't pressed
                        if (! keyboard.is_pressed(vx_val));
                        {
                            _pc += 2;
                        }
                        break;
                }
            }
            break;
        case 0xF000:
            {
                switch (instruction & 0x00FF)
                {
                    case 0x000A: // FX0A => Decrement PC unless a key is released, then put that key in VX
                        for (int i = 0; i < 16; i++)
                        {
                            if (keyboard.old_state[Keyboard::keymap[i]] && ! keyboard.state[Keyboard::keymap[i]])
                            {
                                _registers[(instruction && 0x0F00) >> 8] = i;
                                break;
                            }
                        }
                        _pc -= 2;
                        break;
                    case 0x0029: // FX29 => set the index to the font character for the value in vx
                        uint8_t x = _registers[(instruction & 0x0F00) >> 8] & 0x000F;
                        _index = (5 * x) + 0x050;
                        std::cout << _index << std::endl;
                        break;
                }
            }
            break;
    }
}
