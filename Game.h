#ifndef GAME_H
#define GAME_H

//Inlcudes
#include <SDL2/SDL.h>
#include "Grid.h"

#define GAMEWIDTH 800
#define GAMEHEIGHT 800

typedef struct {
    Grid grid;
}Game;

void GameInit(Game *game);
void GameClose(Game *game);
void GameTick(Game *game);
void GameRender(Game *game, SDL_Renderer *renderer);

#endif // GAME_H
