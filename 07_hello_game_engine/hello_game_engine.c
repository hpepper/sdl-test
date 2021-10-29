#include "SDL2/SDL.h"

// See: http://wiki.libsdl.org/wiki/index

#define VIDEO_WIDTH 1280
#define VIDEO_HEIGHT 720

#define NUMBER_OF_FRAMES_IN_ANIMATION 250

Uint32 framesPerSecond = 50;

struct coordinates
{
    Uint32 x;
    Uint32 y;
};

// TODO UPS -  Updates per second

// This is an arbitrary number
// TODO find out what this number should be. On a multi lCPPU couldn't this simply be 0?
#define MINIMUM_MILISECONDS_BETWEEN_FRAMES 2

SDL_Texture *loadSceneAssets(SDL_Renderer *renderer)
{
    SDL_Surface *tempSurface = SDL_LoadBMP("test_image.bmp");
    SDL_Texture *riderTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return(riderTexture);
}

void initializeScene(SDL_Renderer *renderer, SDL_Texture *riderTexture)
{
    SDL_RenderClear(renderer);
    SDL_Rect sourceRectangle;
    if ( SDL_QueryTexture(riderTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h) != 0 ) { printf("SDL_Init failed: %s\n", SDL_GetError()); } 

    SDL_Rect destinationRectangle;
    destinationRectangle.x = sourceRectangle.x = 0;
    destinationRectangle.y = sourceRectangle.y = 0;
    destinationRectangle.w = sourceRectangle.w;
    destinationRectangle.h = sourceRectangle.h;

    SDL_RenderCopy(renderer, riderTexture, &sourceRectangle, &destinationRectangle);
}

// This is where all coordinates get calculates
//   and I assume collision detection etc.?
//   so in reality the x an y of the sprite should be updated here.
struct coordinates gameUpdate(int loopIteration)
{
    struct coordinates textureTopleftCorner;
    textureTopleftCorner.x = loopIteration * (VIDEO_WIDTH/NUMBER_OF_FRAMES_IN_ANIMATION);
    textureTopleftCorner.y = loopIteration * (VIDEO_HEIGHT/NUMBER_OF_FRAMES_IN_ANIMATION);
    SDL_Delay(1);
    return(textureTopleftCorner);
}

void screenRender(SDL_Renderer *renderer, SDL_Texture *riderTexture, struct coordinates textureTopleftCorner)
{
    SDL_RenderClear(renderer);

    SDL_Rect sourceRectangle;
    if ( SDL_QueryTexture(riderTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h) != 0 ) { printf("SDL_Init failed: %s\n", SDL_GetError()); } 

    SDL_Rect destinationRectangle;
    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    destinationRectangle.x = textureTopleftCorner.x;
    destinationRectangle.y = textureTopleftCorner.y;
    destinationRectangle.w = sourceRectangle.w;
    destinationRectangle.h = sourceRectangle.h;

    SDL_RenderCopy(renderer, riderTexture, &sourceRectangle, &destinationRectangle);
}

void showRenderedScreen(SDL_Renderer *renderer)
{
    // show the window
    SDL_RenderPresent(renderer);
}

int main(int argc, char *args[])
{
    Uint32 milisecondsPerFrame = 1000 / framesPerSecond;
    Uint32 startMiliseconds = 0;
    long deltaMiliseconds = 0;

    // See: http://wiki.libsdl.org/SDL_Init
    SDL_Init(SDL_INIT_VIDEO);

    // When SDL_WINDOW_FULLSCREEN is ignored the width and height
    SDL_Window *window = SDL_CreateWindow("SDL Render screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

    // http://wiki.libsdl.org/SDL_GetTicks
    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("!!! Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // http://wiki.libsdl.org/SDL_CreateRenderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // In a real game, I expect this would return a class
    //  and the class would hold all the textures/assets
    //  I assume sound is also an asset.
    SDL_Texture *riderTexture = loadSceneAssets(renderer);

    initializeScene(renderer, riderTexture);

    SDL_bool running = SDL_TRUE;
    // this is just to have an automatic end to the loop.
    //  normally it would be the user that would end this loop.
    int loopIteration = 0;
    // TODO move a box accross the screen in 10 seconds
    while (running == SDL_TRUE)
    {
        // TODO this counter wraps around after this program has been running for 49 days
        startMiliseconds = SDL_GetTicks();

        struct coordinates  textureTopleftCorner = gameUpdate(loopIteration);
        screenRender(renderer, riderTexture, textureTopleftCorner);
        showRenderedScreen(renderer);
        deltaMiliseconds = milisecondsPerFrame - (SDL_GetTicks() - startMiliseconds);
        // the '|| (deltaMiliseconds > 500 )' is because I saw this calculated as: DDD deltaMiliseconds = 4294967293
        if ( (deltaMiliseconds < MINIMUM_MILISECONDS_BETWEEN_FRAMES)
            || (deltaMiliseconds > 500 )
        )
        {
            // TODO Update the game state without rendering until it catches up. P52 Killer game programming in java.
            // TODO log telemetry about the wait time?
            deltaMiliseconds = MINIMUM_MILISECONDS_BETWEEN_FRAMES;
        }

        printf("DDD deltaMiliseconds = %ld\n", deltaMiliseconds);
        SDL_Delay(deltaMiliseconds);
        loopIteration++;
        if (loopIteration >= NUMBER_OF_FRAMES_IN_ANIMATION)
        {
            running = SDL_FALSE;
        }
    }

    // set to black // This function expects Red, Green, Blue and
    // http://wiki.libsdl.org/SDL_SetRenderDrawColor
    SDL_SetRenderDrawColor(renderer, 0, 126, 0, SDL_ALPHA_OPAQUE);

    // clear the window to black
    // See: http://wiki.libsdl.org/SDL_RenderClear
    SDL_RenderClear(renderer);

    // show the window
    SDL_RenderPresent(renderer);
    // set a delay before quitting
    SDL_Delay(500);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // https://wiki.libsdl.org/SDL_Quit
    SDL_Quit();

    return 0;
}