#include <fstream>

#include "interpreter.h"

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
