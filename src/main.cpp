#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr unsigned int window_width = 1024;
constexpr unsigned int window_height = 512;

struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

SDL_AppResult SDL_AppInit(void **app_state, int argc, char *argv[])
{
    SDL_SetAppMetadata("Chip-8 Emulator", "1.0", "com.example.kylo.chip8emu");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator", window_width, window_height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    
    *app_state = new AppState{
        window,
        renderer,
    };

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

SDL_AppResult SDL_AppIterate(void *appstate)
{
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}
