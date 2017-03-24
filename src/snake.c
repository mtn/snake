#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "lib/snake.h"

void renderSnake(Window* W, Snake* S){
    // Pretty naive, for now
    for(int i = 0; i < S->len; i++){
        wmvaddch(W,S->loc[i]->y,S->loc[i]->x,ACS_DIAMOND);
    }
    wrefresh(W);
}

void moveUp(Window* W, Snake* S){
    if(S->loc[S->len-1]->y > 1){
        mvwprintw(W,S->loc[S->lastInd]->y,S->loc[S->lastInd]->x," ");
        S->loc[S->lastInd]->y = S->loc[S->lastInd]->y - 1;
        S->loc[S->lastInd]->x = S->loc[S->lastInd]->x;
        S->lastInd = (S->lastInd + 1) % S->len;
    }
}

void moveDown(Window* W, Snake* S){
    if(S->loc[S->len-1]->y < S->b->yMax){
        mvwprintw(W,S->loc[S->lastInd]->y,S->loc[S->lastInd]->x," ");
        S->loc[S->lastInd]->y = S->loc[S->lastInd]->y + 1;
        S->loc[S->lastInd]->x = S->loc[S->lastInd]->x;
        S->lastInd = (S->lastInd + 1) % S->len;
    }
}

void moveLeft(Window* W, Snake* S){
    if(S->loc[S->len-1]->x > 1){
        mvwprintw(W,S->loc[S->lastInd]->y,S->loc[S->lastInd]->x," ");
        S->loc[S->lastInd]->y = S->loc[S->lastInd]->y;
        S->loc[S->lastInd]->x = S->loc[S->lastInd]->x - 1;
        S->lastInd = (S->lastInd + 1) % S->len;
    }
}

void moveRight(Window* W, Snake* S){
    if(S->loc[S->len-1]->x < S->b->xMax){
        mvwprintw(W,S->loc[S->lastInd]->y,S->loc[S->lastInd]->x," ");
        S->loc[S->lastInd]->y = S->loc[S->lastInd]->y;
        S->loc[S->lastInd]->x = S->loc[S->lastInd]->x + 1;
        S->lastInd = (S->lastInd + 1) % S->len;
    }
}

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
    b->xMax = xMax, b->yMax = yMax;
    S->b = b;

    Coord** coordArr = malloc(sizeof(CoordPtr)*xMax*yMax);
    if(!coordArr){
        printf("Allocation of coordinate array failed!");
        exit(1);
    }
    S->loc = coordArr;

    // Snake will start in the center, with length 1
    Coord *first = malloc(sizeof(Coord));
    first->x = (xMax+1)/2;
    first->y = (yMax+1)/2;
    S->loc[0] = first;
    S->lastInd = 0;
    S->len = 1;

    return(S);
}

void delSnake(Snake* S){
    for(int i = 0; i < S->len; i++)
        free(S->loc[i]);
    free(S->b);
    free(S);
}

