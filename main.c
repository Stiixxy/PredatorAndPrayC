#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "Game.h"

int main(int argc, char** argv)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Open a 800x600 window and define an accelerated renderer
    SDL_Window* window = SDL_CreateWindow("PredatorAndPray", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          GAMEWIDTH, GAMEHEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initial renderer color
    //SDL_SetRenderDrawColor(renderer, randCol(), randCol(), randCol(), 255);

    bool running = true;
    int ticksPerSecond = 10000;
    int tickTime = 1000/ticksPerSecond;
    int lastTime = 0, deltaTime = 0;
    Uint32 lastSecond = 0, frames = 0;//Vars to hold render data like fps
    SDL_Event event;

    srand(time(NULL)); // randomize seed

    //Init the game
    Game game;
    GameInit(&game);


    while (running)
    {
        // Check for various events (keyboard, mouse, touch, close)
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                const char* key = SDL_GetKeyName(event.key.keysym.sym);
                if (strcmp(key, "Escape") == 0)
                {
                    running = false;
                }
            }
            else if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        int now = SDL_GetTicks();
        deltaTime += now - lastTime;
	    lastTime = now;
	    if(deltaTime > tickTime)
        {
            deltaTime -= tickTime;
            // Clear buffer
            SDL_RenderClear(renderer);
            //Run a tick

            // fill the scene with white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            //Draw the game here
            GameRender(&game, renderer);

            //Tick the game
            GameTick(&game);

            //Get ready for the next frame, and show current frame
            SDL_RenderPresent(renderer);
            deltaTime -= 0;
            frames++;
            if(lastSecond + 1000 < SDL_GetTicks()){
                lastSecond += 1000;
                printf("Frames last second: %u\n", frames);
                frames = 0;
            }
        }
    }

    //Were closing the game. Cleanup the resources of the game too
    GameClose(&game);

    // Release any of the allocated resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
