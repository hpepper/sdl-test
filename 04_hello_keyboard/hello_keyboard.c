#include "SDL2/SDL.h"

#include <errno.h>
#include <stdbool.h>
#include <time.h>

// See: http://wiki.libsdl.org/wiki/index

#define VIDEO_WIDTH 1280
#define VIDEO_HEIGHT 720

/* msleep(): Sleep for the requested number of milliseconds. */
// Source: https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    res = nanosleep(&ts, &ts);

    if (res == -1)
    {
        printf("EEE usleep() failed with %d\n", errno);
    }

    return res;
}

int main(int argc, char *args[])
{

    const Uint8 *keyState = NULL;

    SDL_Init(SDL_INIT_EVERYTHING);

    printf("III loop until you press ESC\n");

    // https://wiki.libsdl.org/SDL_CreateWindow
    // It seems the window has to be shown it order for the keyboard input to work.
    SDL_Window *window = SDL_CreateWindow("SDL Full screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_SHOWN);

    bool endLoop = false;
    while (endLoop == false)
    {
        // It seems this read through all the events, and possibly put the key events into the the state array.
        SDL_PumpEvents();
        keyState = SDL_GetKeyboardState(NULL);
        if (keyState != NULL)
        {
            // http://wiki.libsdl.org/SDL_Scancode
            if (keyState[SDL_SCANCODE_ESCAPE] == 1)
            {
                printf("<ESC> is pressed.\n");
                endLoop = true;
            }
            if (keyState[SDL_SCANCODE_LEFT] == 1)
            {
                printf("Left arrow is pressed.\n");
            }
            if (keyState[SDL_SCANCODE_RETURN])
            {
                printf("<RETURN> is pressed.\n");
            }
            if (keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_UP])
            {
                printf("Right and Up Keys Pressed.\n");
            }
        }
        else
        {
            printf("EEE keyState is a nullptr   ");
        }
        // sleep for 20 ms
        msleep(20);
    }

    SDL_DestroyWindow(window);

    // https://wiki.libsdl.org/SDL_Quit
    SDL_Quit();

    return 0;
}