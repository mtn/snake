// Interface for snake-related movement

#ifndef SNAKE_H
#define SNAKE_H

#include "main.h"


typedef struct {
    int xMax; // max legal positions
    int yMax;
} Boundaries;

typedef struct {
    Coord** loc;
    Boundaries* b; // These aren't really snake properties, but convenient and unchanging
    Coord* foodLoc;
    int lastDir;
    int lastInd; // Index of last, because the coord array won't be kept in order for efficiency
    int len; // Also corresponds to score
} Snake;

// Movement functions return true if collision occurs
bool moveSnake(GameWindow* GW, Snake* S, int choice);
Snake* newSnake(int xMax, int yMax);
void delSnake(Snake* S);
void renderSnake(Window* W, Snake* S);
void placeFood(GameWindow* GW, Snake* S);
void growSnake(Snake* S, int newY, int newX);

#endif /* SNAKE_H */

