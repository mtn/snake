#include <stdlib.h>
#include <stdio.h>

#include "lib/snake.h"

void moveUp(Snake* S){
    if(S->yLoc > S->b->yMin) (S->yLoc)--;
}

void moveDown(Snake* S){
    if(S->yLoc < S->b->yMax) (S->yLoc)++;
}

void moveLeft(Snake* S){
    if(S->xLoc > S->b->xMin) (S->xLoc)--;
}

void moveRight(Snake* S){
    if(S->xLoc < S->b->xMax) (S->xLoc)++;
}

Snake* newSnake(int xMin, int xMax, int yMin, int yMax){
    Snake *S = malloc(sizeof(Snake));
    if(!S){
        printf("Allocation failed");
        exit(1);
    }
    Boundaries *b = malloc(sizeof(Boundaries));
    b->xMin = xMin, b->xMax = b->xMax, b->yMin = yMin, b->yMax = yMax;
    S->b = b;

    // Snake will start in the center, with length 1
    S->xLoc = (xMin+xMax)/2;
    S->yLoc = (yMin+xMax)/2;
    S->len = 1;

    return(S);
}

void delSnake(Snake* S){
    free(S->b);
    free(S);
}

