#include <stdlib.h>
#include <stdio.h>

#include "lib/snake.h"


void moveUp(Snake* S){
    if(S->loc->y > 1){
        (S->loc->y)--;
    }
}

void moveDown(Snake* S){
    if(S->loc->y < S->b->yMax){
        (S->loc->y)++;
    }
}

void moveLeft(Snake* S){
    if(S->loc->x > 1){
        (S->loc->x)--;
    }
}

void moveRight(Snake* S){
    if(S->loc->x < S->b->xMax){
        (S->loc->x)++;
    }
}

Snake* newSnake(int xMax, int yMax){

    Snake *S = malloc(sizeof(Snake));
    if(!S){
        printf("Allocation of snake failed");
        exit(1);
    }

    Boundaries *b = malloc(sizeof(Boundaries));
    if(!b){
        printf("Allocation of boundaries failed");
        exit(1);
    }
    b->xMax = b->xMax, b->yMax = yMax;
    S->b = b;

    Coord* coordArr = malloc(sizeof(Coord));


    // Snake will start in the center, with length 1
    S->loc->x = (xMax+1)/2;
    S->loc->y = (yMax+1)/2;
    S->xLast = S->xLoc;
    S->yLast = S->yLoc;
    S->len = 1;

    return(S);
}

void delSnake(Snake* S){
    // TODO free loc arr
    free(S->b);
    free(S);
}

