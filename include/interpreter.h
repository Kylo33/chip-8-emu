#ifndef _INTERPRETER
#define _INTERPRETER

#include <string>

#define MEMORY_SIZE 4096

class Interpreter
{
public:
    bool load_rom(char *path);

    // Temporary, for debugging
    void print_memory() {
        for (size_t i = 0; i < MEMORY_SIZE; i++)
        {
            printf("%i : 0x%02X\n", i, _memory[i]);
        }
    }

private:
    unsigned char _memory[MEMORY_SIZE] = { 0 };
};

#endif
