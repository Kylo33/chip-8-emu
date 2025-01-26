#ifndef _INTERPRETER
#define _INTERPRETER

#include <string>
#include <cstdint>
#include <stack>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

class Interpreter
{
public:
    bool load_rom(char *path);
    void execute();

    // Temporary, for debugging
    void print_memory() {
        for (size_t i = 0; i < MEMORY_SIZE; i++)
        {
            printf("%i : 0x%02X\n", i, _memory[i]);
        }
    }

    Interpreter();

private:
    uint64_t _screen[32] { 0 }; 
    uint16_t _pc { 0x200 };
    uint16_t _index;
    uint8_t _registers[REGISTER_COUNT];
    uint8_t _memory[MEMORY_SIZE] { 0 };
    std::stack<uint16_t> _substack;
};

#endif
