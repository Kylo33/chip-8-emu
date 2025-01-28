#ifndef _KEYBOARD
#define _KEYBOARD

#include <unordered_set>

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_keyboard.h>


class Keyboard
{
public:
    bool is_pressed(uint8_t key);
    void save_state();

    bool state[16];
    bool old_state[16];
    static const uint8_t keymap[16];
};

#endif
