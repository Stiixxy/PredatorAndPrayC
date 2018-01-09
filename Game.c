#include "Game.h"
#include <stdio.h>

void GameInit(Game *game){
    printf("Initializing the game\n");
    InitGrid(&(*game).grid, 1000, 1000);
}

void GameClose(Game *game){
    DestroyGrid(&(*game).grid);
}

void GameTick(Game *game){
    GridTick(&game->grid);
}

void GameRender(Game *game, SDL_Renderer *renderer){
    GridRender(&game->grid, renderer);
}
