#ifndef _INTERPRETER
#define _INTERPRETER

#include <string>
#include <cstdint>
#include <stack>
#include <bitset>
#include <unordered_set>
#include "keyboard.h"

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

class Interpreter
{
public:
    bool display_changed = true;
    uint64_t screen[DISPLAY_HEIGHT] { 0 };
    Keyboard keyboard = Keyboard();

    bool load_rom(char *path);
    void execute();

    void press(uint8_t key) { std::cout << "pressed " << key << std::endl; }
    void release(uint8_t key) { std::cout << "released " << key << std::endl; }

    // Temporary, for debugging
    void print_memory() {
        for (size_t i = 0; i < MEMORY_SIZE; i++)
        {
            printf("%i : 0x%02X\n", i, _memory[i]);
        }
    }

    void print_screen() {
        for (uint64_t row: screen)
        {
            std::cout << std::bitset<64>(row).to_string() << "\n";
        }
        std::cout << std::endl;
    }

    Interpreter();
private:
    uint16_t _pc { 0x200 };
    uint16_t _index;
    uint8_t _registers[REGISTER_COUNT];
    uint8_t _memory[MEMORY_SIZE] { 0 };
    std::stack<uint16_t> _substack;
};

#endif
