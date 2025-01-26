#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <array>
#include <fstream>
#include <iostream>

#include "interpreter.h"
#include <unistd.h>

#define INSTRUCTIONS_PER_FRAME 11

constexpr unsigned int window_width { 2048 };
constexpr unsigned int window_height { 1024 };

constexpr SDL_Color on_color {198, 43, 219, 255};
constexpr SDL_Color off_color {0, 0, 0, 255};

struct AppContext {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint64_t frame_count = 0;
    Interpreter *interpreter = new Interpreter();

    ~AppContext() {
        SDL_DestroyTexture(texture);
        delete interpreter;
    }
};

SDL_AppResult SDL_AppInit(void **app_state, int argc, char *argv[])
{
    if (argc != 2)
    {
        SDL_Log("Usage: ./chip8emu [ROM FILE]");
        return SDL_APP_FAILURE;
    }

    SDL_SetAppMetadata("Chip-8 Emulator", "1.0", "com.example.kylo.chip8emu");

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

    context->texture = SDL_CreateTexture(
        context->renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);

    if (context->texture == NULL)
    {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetTextureScaleMode(context->texture, SDL_SCALEMODE_NEAREST);

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
    AppContext *context = static_cast<AppContext*>(app_state);
    for(uint8_t i { 0 }; i < INSTRUCTIONS_PER_FRAME; i++)
    {
        context->interpreter->execute();
    }

    context->frame_count++;
    if (context->frame_count % 120 == 0)
    {
        double seconds = SDL_GetTicks() / (double) 1000;
        std::cout << context->frame_count / seconds << std::endl;
    }
    
    if (context->interpreter->display_changed)
    {
        context->interpreter->display_changed = false;

        uint8_t *pixels;
        int pitch;

        SDL_LockTexture(context->texture, NULL, (void**) &pixels, &pitch);

        for (int i = 0; i < DISPLAY_HEIGHT; i++)
        {
            uint64_t row = context->interpreter->screen[i];
            uint64_t cursor = 1ULL << 63;
            for (int j = 0; j < DISPLAY_WIDTH; j++)
            {
                SDL_Color color = ((row & cursor) != 0) ? on_color : off_color;
                size_t starting_bit = (i * DISPLAY_WIDTH + j) * 3; 
                pixels[starting_bit] = color.r;
                pixels[starting_bit + 1] = color.g;
                pixels[starting_bit + 2] = color.b;

                cursor >>= 1;
            }
        }

        SDL_UnlockTexture(context->texture);
        
        SDL_RenderTexture(context->renderer, context->texture, NULL, NULL);
        SDL_RenderPresent(context->renderer);
    }

    // Temporary solution
    // Sleep for 16ms to hit 60fps
    usleep(16000); 

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *app_state, SDL_AppResult result)
{
    if (app_state != NULL)
    {
        delete static_cast<AppContext*>(app_state);
    }
}
