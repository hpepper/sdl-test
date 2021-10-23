#include "SDL2/SDL.h"
#include "SDL2/SDL_gamecontroller.h"

// See: https://davidgow.net/handmadepenguin/ch6.html

#define MAX_GAME_CONTROLLERS 8

// Main entrypoint
int main(int argc, char *args[])
{
       SDL_GameController *ControllerHandles[MAX_GAME_CONTROLLERS];

       SDL_Init(SDL_INIT_JOYSTICK);

       int numberOfJoysticksDetected = SDL_NumJoysticks();
       printf("Number of Joysticks detected: %d\n", numberOfJoysticksDetected);
       int gameControllerIndex = 0;
       for (int joystickIndex = 0; joystickIndex < numberOfJoysticksDetected; joystickIndex++)
       {
              if (SDL_IsGameController(joystickIndex))
              {
                     printf("Joystick %d is GameController %d\n", joystickIndex, gameControllerIndex);
                     if (gameControllerIndex < MAX_GAME_CONTROLLERS)
                     {
                            ControllerHandles[gameControllerIndex] = SDL_GameControllerOpen(joystickIndex);
                            gameControllerIndex++;
                     }
              }
              else
              {
                     printf("Joystick %d is not a GameController\n", joystickIndex);
              }
       }

       return 0;
}