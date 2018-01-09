#include "Grid.h"
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"

void InitGrid(Grid *grid, int height, int width){
    int x,y, randomInt, count=0;

    (*grid).height = height;
    (*grid).width = width;

    //Init the width of the array
    (*grid).tileMap = malloc(sizeof(Tile*) * width);
    for(x = 0; x < width; x++){
        //for the current x init the height of the array
        grid->tileMap[x] = malloc(sizeof(Tile) * height);
        for(y = 0; y < height; y++){
            grid->tileMap[x][y] = malloc(sizeof(Tile));
            //printf("Creating tile at %i:%i with a value of 0\n", x, y);
            randomInt = rand() % 100;
            if(randomInt <= 10){
                grid->tileMap[x][y]->type = 1;
            }else if(randomInt <= 20) {
                grid->tileMap[x][y]->type = 2;
            }else{
                grid->tileMap[x][y]->type = 0;
            }
            count++;
            if(grid->tileMap[x][y]->type == 1){
                grid->tileMap[x][y]->health = 1;
            }else{
                grid->tileMap[x][y]->health = STARTING_HEALTH;
            }
        }
    }

    //printf("The int at %i\n", count);
}

void DestroyGrid(Grid *grid){
    int x;
    for(x = 0; x < (*grid).width; x++){
        //for the current x init the height of the array
        free((*grid).tileMap[x]);
    }
    free((*grid).tileMap);
}

void GridRender(Grid *grid, SDL_Renderer *renderer){

    //SDL_Rect rect = {2,2,20,20};
    //SDL_RenderFillRect(renderer, &rect);

    //Calculate the width and height of a single rectangle based on grid tilemap
    int width = GAMEWIDTH, height = GAMEHEIGHT;
    SDL_Rect rect;
    int x,y;
    width /= grid->width;
    height /= grid->height;

    //printf("Drawing the grid with rect size %i:%i\n", width, height);

    //Set rect width and height
    rect.w = width;
    rect.h = height;

    //Draw all tiles
    for(x = 0; x < grid-> width; x++){
        for(y = 0; y < grid->height; y++){
            //Set the renderer color
            switch(grid->tileMap[x][y]->type){
            case 1:
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                break;
            case 2:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                break;
            }

            //Set the rect variables and render it
            rect.x = x * width;
            rect.y = y * height;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void GridTick(Grid *grid){
    int x,y;

    for(x = 0; x < grid->width; x++){
        for(y = 0; y < grid->height; y++){
            if(grid->tileMap[x][y]->type != 0){//Its either a prey or predator
                if(grid->tileMap[x][y]->type == 1){//Its a pray
                    DoPrayLogic(grid, x, y);
                }else{//Its a predator
                    DoPredatorLogic(grid, x,y);
                }
            }
        }
    }
}

void DoPredatorLogic(Grid *grid, int x, int y){
    grid->tileMap[x][y]->health--;//Lose health
    if(grid->tileMap[x][y]->health == 0){
        grid->tileMap[x][y]->type = 0; //The predator dies
    }
    int newX, newY;
    getRandomXYInGrid(grid, &newX, &newY);


    if(grid->tileMap[newX][newY]->type == 0){//Moving onto an empty tile
        //printf("Predator moving onto tile %i:%i, with type %i\n", newX, newY, grid->tileMap[newX][newY]->type);
        moveTile(grid, x, y, newX, newY);
    }else if(grid->tileMap[newX][newY]->type == 1){//Moving onto a pray tile
        grid->tileMap[newX][newY]->type = 2;//The pray is now a predator
        //grid->tileMap[newX][newY]->health = STARTING_HEALTH;
        grid->tileMap[x][y]->health += grid->tileMap[newX][newY]->health;
    }//Else its a predator so we cant move
}

void DoPrayLogic(Grid *grid, int x, int y){
    grid->tileMap[x][y]->health++;//Gain health

    int newX = x, newY = y;
    getRandomXYInGrid(grid, &newX, &newY);

    if(grid->tileMap[x][y]->health >= PRAY_THRESHHOLD){//Check health threshhold
        int count;
        while(grid->tileMap[newX][newY]->type != 0 && count < 10){//Try 10 times for a random position if its empty
            count++;
            getRandomXYInGrid(grid, &newX, &newY);
        }
        if(count != 10){//We found a random position
            grid->tileMap[newX][newY]->type = 1;
            grid->tileMap[newX][newY]->health = 10;
            grid->tileMap[x][y]->health = 10;
        }
    }

    getRandomXYInGrid(grid, &newX, &newY);

    if(grid->tileMap[newX][newY]->type == 0){//Moving onto an empty tile
        moveTile(grid, x, y, newX, newY);
    }//We can only move onto an empty tile
}

void moveTile(Grid *grid, int xSrc,int ySrc,int xDes,int yDes){
    grid->tileMap[xDes][yDes]->health = grid->tileMap[xSrc][ySrc]->health;
    grid->tileMap[xDes][yDes]->type = grid->tileMap[xSrc][ySrc]->type;

    grid->tileMap[xSrc][ySrc]->type = 0;
}

void getRandomXYInGrid(Grid *grid, int *x, int *y){
    *x += rand() % 3 - 1;
    *y += rand() % 3 - 1;

    //Make sure we dont move off the screen
    if(*x < 0) *x = 0;
    if(*x >= grid->width) *x = grid->width - 1;
    if(*y < 0) *y = 0;
    if(*y >= grid->height) *y = grid->height - 1;

    //printf("Returned %i:%i for random numbers\n", *x, *y);
}
