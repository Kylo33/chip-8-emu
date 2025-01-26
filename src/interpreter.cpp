#include <fstream>
#include <cstdint>

#include "interpreter.h"
#include <string.h>
#include <iostream>

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
        case 0x0000: // 0x00E0 => clear the screen
            for (uint8_t i = 0; i < 32; i++)
            {
                _screen[i] = 0;
            }
            break;
        case 0x1000: // 0x1NNN => jump
            _pc = instruction & 0x0FFF;

            // TEMPORARY
            for (int i = 0; i < 32; i++)
            {
                std::cout << _screen[i] << std::endl;
            }
            exit(0);

            break;
        case 0x6000: // 0x6XNN => set register VX to NN
            _registers[instruction & 0x0F00] = instruction & 0x00FF;
            break;
        case 0x7000: // 0x7XNN => increment register VX by NN
            _registers[instruction & 0x0F00] += instruction & 0x00FF;
            break;
        case 0xA000: // 0xANNN => set index register to NNN
            _index = instruction & 0x0FFF;
            break;
        case 0xD000: // 0xDXYN => draw
            uint8_t x = _registers[instruction & 0x0F00] % DISPLAY_WIDTH;
            uint8_t y = _registers[instruction & 0x00F0] % DISPLAY_HEIGHT;

            _registers[0xF] = 0;

            for (uint8_t i = 0, n = instruction & 0x000F; i < n; i++)
            {
                uint64_t sprite_row = _memory[_index + i];
                
                // Make sure this works for sprites that go off the screen
                sprite_row <<= DISPLAY_WIDTH - x - 8;

                if (_screen[y + i] & sprite_row != 0)
                {
                    _registers[0xF] = 1;
                }
                
                _screen[y + i] |= sprite_row;
            }

            break;
    }
}
