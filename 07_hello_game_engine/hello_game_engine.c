#include "SDL2/SDL.h"

// See: http://wiki.libsdl.org/wiki/index

#define VIDEO_WIDTH 1280
#define VIDEO_HEIGHT 720

#define NUMBER_OF_FRAMES_IN_ANIMATION 250

#define MAX_FRAMES_TO_SKIP 5

Uint32 framesPerSecond = 50;

// TODO UPS -  Updates per second

// This is an arbitrary number
// TODO find out what this number should be. On a multi lCPPU couldn't this simply be 0?
#define MINIMUM_MILISECONDS_BETWEEN_FRAMES 1

SDL_Texture *loadSceneAssets(SDL_Renderer *renderer)
{
    SDL_Surface *tempSurface = SDL_LoadBMP("test_image.bmp");
    SDL_Texture *riderTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return (riderTexture);
}

void initializeScene(SDL_Renderer *renderer, SDL_Texture *riderTexture)
{
    SDL_RenderClear(renderer);
    SDL_Rect sourceRectangle;
    if (SDL_QueryTexture(riderTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h) != 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }

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
SDL_Point gameUpdate(int loopIteration)
{
    SDL_Point textureTopleftCorner;
    textureTopleftCorner.x = loopIteration * (VIDEO_WIDTH / NUMBER_OF_FRAMES_IN_ANIMATION);
    textureTopleftCorner.y = loopIteration * (VIDEO_HEIGHT / NUMBER_OF_FRAMES_IN_ANIMATION);
    SDL_Delay(1);
    return (textureTopleftCorner);
}

void screenRender(SDL_Renderer *renderer, SDL_Texture *riderTexture, SDL_Point textureTopleftCorner)
{
    SDL_RenderClear(renderer);

    SDL_Rect sourceRectangle;
    if (SDL_QueryTexture(riderTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h) != 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }

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
    long milisecondsPerFrame = (long)(1000 / framesPerSecond);
    Uint32 startMiliseconds = 0;
    Uint32 currentMiliseconds = 0;
    long elapsedMiliseconds = 0;
    long sleepTimeMiliseconds = 0;
    // The excess concept is from 'killer game programming in java' p36
    long excessTimeSpentMiliseconds = 0;

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
    startMiliseconds = SDL_GetTicks();
    // TODO move a box accross the screen in 10 seconds
    while (running == SDL_TRUE)
    {
        // TODO this counter wraps around after this program has been running for 49 days

        SDL_Point textureTopleftCorner = gameUpdate(loopIteration);
        screenRender(renderer, riderTexture, textureTopleftCorner);

        // TODO should this be moved after the delta calcultations?
        //   I don't think so, but (,p176 thinks so)
        showRenderedScreen(renderer);

        // Getting the elapsed and current time this way will entsure the full round through the while is included.
        currentMiliseconds = SDL_GetTicks();
        elapsedMiliseconds = (long)(currentMiliseconds - startMiliseconds);
        startMiliseconds = currentMiliseconds;

        sleepTimeMiliseconds = milisecondsPerFrame - elapsedMiliseconds;
        if (sleepTimeMiliseconds < 0)
        {
            // this is actually + because sleepTimeMiliseconds is < 0.
            excessTimeSpentMiliseconds -= sleepTimeMiliseconds;
        }

        if (excessTimeSpentMiliseconds > milisecondsPerFrame)
        {
            printf("WWW skipping frames because  excessTimeSpentMiliseconds(%ld) > milisecondsPerFrame(%ld)\n", excessTimeSpentMiliseconds, milisecondsPerFrame);
            int framesSkipped = 0;
            while ((excessTimeSpentMiliseconds > milisecondsPerFrame) && (framesSkipped < MAX_FRAMES_TO_SKIP))
            {
                // ++loopIteration because the update has to be called on the new value.
                gameUpdate(++loopIteration);
                excessTimeSpentMiliseconds -= milisecondsPerFrame;
                framesSkipped++;
            }
        }
        // the '|| (sleepTimeMiliseconds > 500 )' is because I saw this calculated as: DDD sleepTimeMiliseconds = 4294967293
        if ((sleepTimeMiliseconds < MINIMUM_MILISECONDS_BETWEEN_FRAMES) || (sleepTimeMiliseconds > 500))
        {
            // TODO Update the game state without rendering until it catches up. P52 Killer game programming in java.
            // TODO log telemetry about the wait time?
            sleepTimeMiliseconds = MINIMUM_MILISECONDS_BETWEEN_FRAMES;
        }

        printf("DDD sleepTimeMiliseconds = %ld, elapsedMiliseconds = %ld\n", sleepTimeMiliseconds, elapsedMiliseconds);
        SDL_Delay(sleepTimeMiliseconds);
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