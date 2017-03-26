#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "lib/snake.h"

void renderSnake(Window* W, Snake* S){
    // Pretty naive? TODO check how rendering updates work
    for(int i = 0; i < S->len; i++)
        wmvaddch(W,S->loc[i]->y,S->loc[i]->x,ACS_DIAMOND);
    wrefresh(W);
}

bool reachingFood(Snake* S, int dir){
    switch(dir){
        case KEY_UP:
            return S->foodLoc->x == S->loc[S->lastInd]->x &&
                S->foodLoc->y == (S->loc[S->lastInd]->y-1);
            break;
        case KEY_DOWN:
            return S->foodLoc->x == S->loc[S->lastInd]->x &&
                S->foodLoc->y == (S->loc[S->lastInd]->y+1);
            break;
        case KEY_LEFT:
            return S->foodLoc->x == S->loc[S->lastInd]->x-1 &&
                S->foodLoc->y == (S->loc[S->lastInd]->y);
            break;
        case KEY_RIGHT:
            return S->foodLoc->x == S->loc[S->lastInd]->x+1 &&
                S->foodLoc->y == (S->loc[S->lastInd]->y);
            break;
    }
    return false;
}

void growSnake(Snake* S, int newY, int newX){
    for(int i = S->len; i > S->lastInd; i++){
        S->loc[i] = S->loc[i-1];
    }
    // Adding the head
    Coord *new = malloc(sizeof(Coord));
    new->x = newX, new->y = newY;
    S->loc[S->lastInd] = new;
    S->len++;
}

bool moveSnake(GameWindow* GW, Snake* S, int choice){
    bool eating = reachingFood(S,choice);
    int xShift=0, yShift=0, newX, newY;
    switch(choice){
        case KEY_UP:
            yShift = -1;
            break;
        case KEY_DOWN:
            yShift = 1;
            break;
        case KEY_LEFT:
            xShift = -1;
            break;
        case KEY_RIGHT:
            xShift = 1;
            break;
    }
    refresh();
    newX = S->loc[S->lastInd]->x + xShift;
    newY = S->loc[S->lastInd]->y + yShift;
    if(newX > 0 && newY > 0 && newX < S->b->xMax && newY < S->b->yMax
            && (eating || !isOccupied(GW,newY,newX,S->b->xMax))){
            if(!eating){
                mvwprintw(GW->W,S->loc[S->lastInd]->y,S->loc[S->lastInd]->x," ");
                GW->isOccupied[toOneD(newY-yShift,newX-xShift,S->b->xMax)] = false;
                S->loc[S->lastInd]->y = newY;
                S->loc[S->lastInd]->x = newX;
            }
            else growSnake(S,newY,newX);
            GW->isOccupied[toOneD(newY,newX,S->b->xMax)] = true;
            S->lastInd = (S->lastInd + 1) % S->len;

            // Unfortunately clunky for now
            switch(choice){
                case KEY_UP:
                    S->lastDir = KEY_UP;
                    break;
                case KEY_DOWN:
                    S->lastDir = KEY_DOWN;
                    break;
                case KEY_LEFT:
                    S->lastDir = KEY_LEFT;
                    break;
                case KEY_RIGHT:
                    S->lastDir = KEY_RIGHT;
                    break;
            }
            return false;
    }
    printw("couldnt advance");
    printw("yMax: %d xMax: %d newY: %d newX: %d eating: %d isOccupied: %d",S->b->yMax,S->b->xMax,newY,newX,eating,!isOccupied(GW,newY,newX,S->b->xMax));
    refresh();
    return true;
}

void placeFood(GameWindow* GW, Snake* S){
    Coord *foodLoc = malloc(sizeof(Coord));
    if(!foodLoc){
        printf("Allocation of food coord failed!");
        exit(1);
    }
    do{
        foodLoc->x = rand() % S->b->xMax;
        foodLoc->y = rand() % S->b->yMax;
    } while(GW->isOccupied[toOneD(foodLoc->x,foodLoc->y,S->b->xMax)]);

    wmvaddch(GW->W,foodLoc->y,foodLoc->y,ACS_DIAMOND);
    S->foodLoc = foodLoc;
}

void consumeFood(GameWindow* GW, Snake* S){


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
    S->lastDir = KEY_RIGHT;
    S->len = 1;

    return(S);
}

void delSnake(Snake* S){
    for(int i = 0; i < S->len; i++)
        free(S->loc[i]);
    free(S->b);
    free(S);
}

