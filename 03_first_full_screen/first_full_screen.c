#include "SDL2/SDL.h"

#include <unistd.h>

// See: http://wiki.libsdl.org/wiki/index

#define VIDEO_WIDTH 1280
#define VIDEO_HEIGHT 720

int main(int argc, char *args[])
{
    // See: http://wiki.libsdl.org/SDL_Init
    SDL_Init(SDL_INIT_VIDEO);

    // When SDL_WINDOW_FULLSCREEN is ignored the width and height
    SDL_Window *window = SDL_CreateWindow("SDL Full screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("!!! Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    sleep(10);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // https://wiki.libsdl.org/SDL_Quit
    SDL_Quit();

    return 0;
}