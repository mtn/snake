// Interface for snake movement

#ifndef SNAKE_H
#define SNAKE_H

#include "main.h"


typedef struct {
    int xMax; // These are max legal positions
    int yMax;
} Boundaries;

typedef struct {
    Coord** loc;
    Boundaries* b; // These aren't really snake properties, but convenient and unchanging
    int lastDir;
    int lastInd; // Index of last, because the coord array won't be kept in order for efficiency
    int len; // Also corresponds to score
} Snake;

void moveUp(Window* W, Snake* S);
void moveDown(Window* W, Snake* S);
void moveLeft(Window* W, Snake* S);
void moveRight(Window* W, Snake* S);
Snake* newSnake(int xMax, int yMax);
void delSnake(Snake* S);
void renderSnake(Window* W, Snake* S);

#endif /* SNAKE_H */

