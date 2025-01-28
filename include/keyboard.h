#ifndef _KEYBOARD
#define _KEYBOARD

#include <unordered_set>

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_keyboard.h>


class Keyboard
{
public:
    bool is_pressed(uint8_t key);
    const bool *state = SDL_GetKeyboardState(NULL);
    const bool *old_state = SDL_GetKeyboardState(NULL);
    static const uint8_t keymap[];
};

#endif
