#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>

#define PRAY_THRESHHOLD 50
#define STARTING_HEALTH 100

typedef struct{
    Uint8 type;
    int health;
}Tile;

typedef struct {
    Uint8 height, width;
    Tile ***tileMap;
}Grid;

void InitGrid(Grid *grid, int, int);
void DestroyGrid(Grid *grid);

void GridRender(Grid *grid, SDL_Renderer *renderer);
void GridTick(Grid *grid);

void DoPredatorLogic(Grid *grid, int x, int y);
void DoPrayLogic(Grid *grid, int x, int y);
void moveTile(Grid *grid, int xSrc, int ySrc,int xDes,int yDes);

void getRandomXYInGrid(Grid *grid, int *x, int *y);

#endif // GRID_H
