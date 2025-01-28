#include <unordered_map>

#include "keyboard.h"
#include <SDL3/SDL_keyboard.h>

bool Keyboard::is_pressed(uint8_t key)
{
    return state[Keyboard::keymap[key]];
}

void Keyboard::save_state()
{
    memcpy(old_state, state, 16);

    const bool *sdl_state = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < 16; i++)
    {
        state[i] = sdl_state[Keyboard::keymap[i]];
    }
}

const uint8_t Keyboard::keymap[] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V,
};
