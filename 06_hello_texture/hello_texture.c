#include "SDL2/SDL.h"

// See: http://wiki.libsdl.org/wiki/index

#define VIDEO_WIDTH 1280
#define VIDEO_HEIGHT 720

int main(int argc, char *args[])
{
    // See: http://wiki.libsdl.org/SDL_Init
    SDL_Init(SDL_INIT_VIDEO);

    // When SDL_WINDOW_FULLSCREEN is ignored the width and height
    SDL_Window *window = SDL_CreateWindow("SDL Render screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("!!! Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // http://wiki.libsdl.org/SDL_CreateRenderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *tempSurface = SDL_LoadBMP("test_image.bmp");
    SDL_Texture *riderTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // clear the window to black
    // See: http://wiki.libsdl.org/SDL_RenderClear
    SDL_RenderClear(renderer);

    SDL_Rect sourceRectangle;
    if ( SDL_QueryTexture(riderTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h) != 0 ) { printf("SDL_Init failed: %s\n", SDL_GetError()); } 

    SDL_Rect destinationRectangle;
    destinationRectangle.x = sourceRectangle.x = 0;
    destinationRectangle.y = sourceRectangle.y = 0;
    destinationRectangle.w = sourceRectangle.w;
    destinationRectangle.h = sourceRectangle.h;

    SDL_RenderCopy(renderer, riderTexture, &sourceRectangle, &destinationRectangle);

    // show the window
    SDL_RenderPresent(renderer);
    // set a delay before quitting
    SDL_Delay(5000);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // https://wiki.libsdl.org/SDL_Quit
    SDL_Quit();

    return 0;
}