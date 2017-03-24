#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "lib/snake.h"

void renderSnake(Window* W, Snake* S){
    // Pretty naive, for now
    for(int i = 0; i < S->len; i++){
        mvwprintw(W,S->loc[i]->y,S->loc[i]->x,"\u25CF");
        wprintw(W,"hi");
    }
    wrefresh(W);
}

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
    }
}

void moveDown(Snake* S){
    if(S->loc[S->len-1]->y < S->b->yMax){
        Coord *prev = S->loc[S->len-1];
        S->loc[S->len-1]->y++;
        percolate(S,prev);
    }
}

void moveLeft(Snake* S){
    if(S->loc[S->len-1]->x > 1){
        Coord *prev = S->loc[S->len-1];
        S->loc[S->len-1]->x--;
        percolate(S,prev);
    }
}

void moveRight(Snake* S){
    if(S->loc[S->len-1]->x < S->b->xMax){
        Coord *prev = S->loc[S->len-1];
        S->loc[S->len-1]->x++;
        percolate(S,prev);
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
    b->xMax = b->xMax, b->yMax = yMax;
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
    S->len = 1;

    return(S);
}

void delSnake(Snake* S){
    // TODO free loc arr
    free(S->b);
    free(S);
}

