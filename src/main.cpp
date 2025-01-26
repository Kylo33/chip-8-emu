#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <array>
#include <fstream>
#include <iostream>

#include "interpreter.h"

#define MEMORY_SIZE 4096

constexpr unsigned int window_width = 1024;
constexpr unsigned int window_height = 512;

struct AppContext {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Interpreter *interpreter = new Interpreter();
};

SDL_AppResult SDL_AppInit(void **app_state, int argc, char *argv[])
{
    SDL_SetAppMetadata("Chip-8 Emulator", "1.0", "com.example.kylo.chip8emu");

    if (argc != 2)
    {
        SDL_Log("Usage: ./chip8emu [ROM FILE]");
        return SDL_APP_FAILURE;
    }

    AppContext *context = new AppContext();
    *app_state = context;
    
    if (!context->interpreter->load_rom(argv[1]))
    {
        SDL_Log("Error: unable to read file: '%s'\nUsage: ./chip8emu [ROM FILE]", argv[1]);
        return SDL_APP_FAILURE;
    }
    
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Unable to intialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    context->window = SDL_CreateWindow("Chip-8 Emulator", window_width, window_height, 0);
    if (context->window == NULL)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    context->renderer = SDL_CreateRenderer(context->window, NULL);
    if (context->renderer == NULL)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *app_state)
{
    SDL_Renderer *renderer = static_cast<AppContext*>(app_state)->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_ALPHA_OPAQUE);
    

    SDL_RenderPresent(renderer);
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *app_state, SDL_AppResult result)
{
    delete static_cast<AppContext*>(app_state)->interpreter;
}
