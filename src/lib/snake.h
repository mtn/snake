// Interface for snake-related movement

#ifndef SNAKE_H
#define SNAKE_H

#include "main.h"


typedef Coord Boundaries; // Boundaries can be summarized by the right-most, bottom-most coordinate

typedef struct {
    CoordLL* first; // Head
    CoordLL* last; // End of tail
    Boundaries* bounds; // Not really a snake properties, but convenient and unchanging
    Coord* foodLoc;
    int lastDir;
    int len;
} Snake;

// Movement functions return true if collision occurs
bool moveSnake(GameWindow* GW, Snake* S, int choice);
Snake* newSnake(int xMax, int yMax);
void delSnake(Snake* S);
void renderSnake(Window* W, Snake* S);
void placeFood(GameWindow* GW, Snake* S);
void growSnake(Snake* S, int newY, int newX);

#endif /* SNAKE_H */

