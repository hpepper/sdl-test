#include "SDL2/SDL.h"
#include "SDL2/SDL_gamecontroller.h"

#include <errno.h>
#include <stdbool.h>
#include <time.h>

// See: https://davidgow.net/handmadepenguin/ch6.html

#define MAX_GAME_CONTROLLERS 8

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

void ShowXyAxisPostions(SDL_GameController *gameControllerHandle, SDL_GameControllerAxis xAxis, SDL_GameControllerAxis yAxis, char axisName[40])
{
       int stickX = SDL_GameControllerGetAxis(gameControllerHandle, xAxis);
       int stickY = SDL_GameControllerGetAxis(gameControllerHandle, yAxis);
       printf("%s %d, %d\n", axisName, stickX, stickY);
}

void ShowTriggerPostion(SDL_GameController *gameControllerHandle, SDL_GameControllerAxis axis, char triggerName[40])
{
       int triggerValue = SDL_GameControllerGetAxis(gameControllerHandle, axis);
       printf("%s %d\n", triggerName, triggerValue);
}

bool ShowButtonPushed(SDL_GameController *gameControllerHandle, SDL_GameControllerButton button, char buttonName[40])
{
       bool returnValue = false;
       if (SDL_GameControllerGetButton(gameControllerHandle, button))
       {
              returnValue = true;
              printf("%s button %s pressed\n", SDL_GameControllerName(gameControllerHandle), buttonName);
       }
       return (returnValue);
}

// Main entrypoint
int main(int argc, char *args[])
{
       SDL_GameController *gameControllerHandles[MAX_GAME_CONTROLLERS];

       for (int gamegameControllerIndex = 0; gamegameControllerIndex < MAX_GAME_CONTROLLERS; gamegameControllerIndex++)
       {
              gameControllerHandles[gamegameControllerIndex] = NULL;
       }

       SDL_Init(SDL_INIT_JOYSTICK);

       int numberOfJoysticksDetected = SDL_NumJoysticks();
       printf("III Number of Joysticks detected: %d\n", numberOfJoysticksDetected);

       // Detect all the game controllers, and open them TODO explain Open
       int gamegameControllerIndex = 0;
       for (int joystickIndex = 0; joystickIndex < numberOfJoysticksDetected; joystickIndex++)
       {
              if (SDL_IsGameController(joystickIndex))
              {
                     printf("III Joystick %d is GameController %d\n", joystickIndex, gamegameControllerIndex);
                     if (gamegameControllerIndex < MAX_GAME_CONTROLLERS)
                     {
                            gameControllerHandles[gamegameControllerIndex] = SDL_GameControllerOpen(joystickIndex);
                            printf("III GameController %d name: %s\n", gamegameControllerIndex, SDL_GameControllerName(gameControllerHandles[gamegameControllerIndex]));

                            gamegameControllerIndex++;
                     }
              }
              else
              {
                     printf("WWW Joystick %d is not a GameController\n", joystickIndex);
              }
       }

       // SDL_GameControllerEventState(SDL_ENABLE);

       printf("III loop until you press the guide button (X logo)\n");
       bool endLoop = false;
       while (endLoop == false)
       {
              // Manually pump game controller updates if not using the loop.
              SDL_GameControllerUpdate();
              for (int gameControllerIndex = 0;
                   gameControllerIndex < MAX_GAME_CONTROLLERS;
                   ++gameControllerIndex)
              {
                     if (gameControllerHandles[gameControllerIndex] != NULL && SDL_GameControllerGetAttached(gameControllerHandles[gameControllerIndex]))
                     {
                            // Controller button values: https://wiki.libsdl.org/SDL_GameControllerButton
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_A, "A");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_B, "B");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_X, "X");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_Y, "Y");

                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_START, "start");
                            endLoop = ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_BACK, "back");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_GUIDE, "guide");

                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_LEFTSTICK, "L-Stick");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_RIGHTSTICK, "R-Stick");

                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_UP, "dpad-Up");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_DOWN, "dpad-Down");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_LEFT, "dpad-Left");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "dpad-Right");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_LEFTSHOULDER, "LB");
                            ShowButtonPushed(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, "RB");

                            /* ENABLES this if you want to see the axis values
                            ShowXyAxisPostions(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY, "Left-Stick");
                            ShowXyAxisPostions(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY, "Right-Stick");
                            ShowTriggerPostion(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_AXIS_TRIGGERLEFT, "Left-Trigger");
                            ShowTriggerPostion(gameControllerHandles[gameControllerIndex], SDL_CONTROLLER_AXIS_TRIGGERRIGHT, "Right-Trigger");
                            */
                     }
              }
              // sleep for 20 ms
              msleep(20);
       }

       // Go through and close all the controllers.
       for (int gamegameControllerIndex = 0; gamegameControllerIndex < MAX_GAME_CONTROLLERS; gamegameControllerIndex++)
       {
              if (gameControllerHandles[gamegameControllerIndex])
              {
                     printf("III About to close GameController %d\n", gamegameControllerIndex);
                     SDL_GameControllerClose(gameControllerHandles[gamegameControllerIndex]);
              }
       }

       return 0;
}