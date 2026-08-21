#include <SDL.h>

int main(int argc, char **argv)
{
    SDL_Window *window;
    SDL_Event event;
    int running = 1;

    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "SDL2 Smoke Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        400,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}