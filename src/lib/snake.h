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
    int len; // Also corresponds to score
} Snake;

void moveUp(Snake* S);
void moveDown(Snake* S);
void moveLeft(Snake* S);
void moveRight(Snake* S);
Snake* newSnake(int xMax, int yMax);
void delSnake(Snake* S);
void renderSnake(Window* W, Snake* S);

#endif /* SNAKE_H */

