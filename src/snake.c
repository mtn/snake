#include <stdlib.h>
#include <stdio.h>

#include "lib/snake.h"

void percolate(Snake* S, Coord* prev){
    Coord* temp;
    for(int i = S->len-2; i >= 0; i--){
        temp = S->loc[i];
        S->loc[i] = prev;
        prev = temp;
    }
}

void moveUp(Snake* S){
    if(S->loc[S->len-1]->y > 1){
        Coord *prev = S->loc[S->len-1];
        S->loc[S->len-1]->y--;
        percolate(S,prev);
        S->len++;
    }
}

/* void moveDown(Snake* S){ */
/*     if(S->loc->y < S->b->yMax){ */
/*         (S->loc->y)++; */
/*     } */
/* } */

/* void moveLeft(Snake* S){ */
/*     if(S->loc->x > 1){ */
/*         (S->loc->x)--; */
/*     } */
/* } */

/* void moveRight(Snake* S){ */
/*     if(S->loc->x < S->b->xMax){ */
/*         (S->loc->x)++; */
/*     } */
/* } */

Snake* newSnake(int xMax, int yMax){

    Snake *S = malloc(sizeof(Snake));
    if(!S){
        printf("Allocation of snake failed!");
        exit(1);
    }

    Boundaries *b = malloc(sizeof(Boundaries));
    if(!b){
        printf("Allocation of boundaries failed!");
        exit(1);
    }
    b->xMax = b->xMax, b->yMax = yMax;
    S->b = b;


    Coord** coordArr = malloc(sizeof(Coord)*xMax*yMax);
    if(!coordArr){
        printf("Allocation of coordinate array failed!");
        exit(1);
    }
    S->loc = coordArr;
    // Snake will start in the center, with length 1
    Coord *first = malloc(sizeof(Coord));
    first->x = (xMax+1)/2;
    first->y = (yMax+1)/2;
    first->isOccupied = true;
    S->loc[0] = first;
    S->len = 1;

    return(S);
}

void delSnake(Snake* S){
    // TODO free loc arr
    free(S->b);
    free(S);
}

