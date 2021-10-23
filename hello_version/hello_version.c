#include "SDL2/SDL.h"
#include "SDL2/SDL_version.h"

// See: http://wiki.libsdl.org/SDL_GetVersion

// Main entrypoint
int main(int argc, char *args[])
{
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled against SDL version %d.%d.%d ...\n",
           compiled.major, compiled.minor, compiled.patch);
    printf("We are linking against SDL version %d.%d.%d.\n",
           linked.major, linked.minor, linked.patch);

    return 0;
}