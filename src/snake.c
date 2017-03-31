#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "lib/snake.h"

void renderSnake(Window* W, Snake const * const S){
    CoordLL* body = S->first;
    while(body){
        wmvaddch(W,body->loc->y,body->loc->x,ACS_DIAMOND);
        body = body->next;
    }
    wrefresh(W);
}

bool reachingFood(Snake* S, int dir){
    switch(dir){
        case KEY_UP:
            return S->foodLoc->x == S->first->loc->x &&
                S->foodLoc->y == (S->first->loc->y-1);
            break;
        case KEY_DOWN:
            return S->foodLoc->x == S->first->loc->x &&
                S->foodLoc->y == (S->first->loc->y+1);
            break;
        case KEY_LEFT:
            return S->foodLoc->x == S->first->loc->x-1 &&
                S->foodLoc->y == (S->first->loc->y);
            break;
        case KEY_RIGHT:
            return S->foodLoc->x == S->first->loc->x+1 &&
                S->foodLoc->y == (S->first->loc->y);
            break;
    }
    return false;
}

void placeFood(GameWindow* GW, Snake* S){
    Coord *foodLoc = calloc(1,sizeof(Coord));
    if(!foodLoc){
        printf("Allocation of food coordinate failed!");
        exit(1);
    }
    do{
        foodLoc->x = (rand() % (S->bounds->x-1)) + 1;
        foodLoc->y = (rand() % (S->bounds->y-1)) + 1;
    } while(GW->isOccupied[toOneD(foodLoc->y,foodLoc->x,S->bounds->x)]);

    GW->isOccupied[toOneD(foodLoc->y,foodLoc->x,S->bounds->x)] = true;
    free(S->foodLoc);
    S->foodLoc = foodLoc;
    wmvaddch(GW->W,foodLoc->y,foodLoc->x,ACS_DIAMOND); }

bool moveSnake(GameWindow* GW, Snake* S, int choice){
    bool eating = reachingFood(S,choice);
    int xShift, yShift, newX, newY;
    xShift = yShift = 0;
    if(S->lastDir == KEY_UP && choice == KEY_DOWN) choice = KEY_UP;
    if(S->lastDir == KEY_DOWN && choice == KEY_UP) choice = KEY_DOWN;
    if(S->lastDir == KEY_LEFT && choice == KEY_RIGHT) choice = KEY_LEFT;
    if(S->lastDir == KEY_RIGHT && choice == KEY_LEFT) choice = KEY_RIGHT;
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
    newX = S->first->loc->x + xShift;
    newY = S->first->loc->y + yShift;
    if(newX > 0 && newY > 0 && newX < S->bounds->x && newY < S->bounds->y
            && (eating || !isOccupied(GW,newY,newX,S->bounds->x))){
            if(!eating){
                mvwprintw(GW->W,S->last->loc->y,S->last->loc->x," ");
                GW->isOccupied[toOneD(S->last->loc->y,S->last->loc->x,S->bounds->x)] = false;
                S->first->prev = S->last; // Reusing tail memory as head
                S->last = S->last->prev;
                S->first = S->first->prev;
                S->first->prev = S->last->next = NULL;
                S->first->loc->y = newY;
                S->first->loc->x = newX;
            }
            else{
                CoordLL* newHead = calloc(1,sizeof(CoordLL));
                Coord* newCoord = calloc(1,sizeof(Coord));
                if(!newHead || !newCoord){
                    printf("Allocation of head or head's coordinate failed!");
                    exit(1);
                }
                newHead->loc = newCoord;
                newHead->loc->x = newX;
                newHead->loc->y = newY;
                newHead->next = S->first;
                S->first->prev = newHead;
                S->first = S->first->prev;
                S->first->prev = NULL;
                S->len++;
                placeFood(GW,S);
            }
            GW->isOccupied[toOneD(newY,newX,S->bounds->x)] = true;
            S->lastDir = choice;
            return false;
    }
    return true;
}

Snake* newSnake(int xMax, int yMax){

    Snake *S = calloc(1,sizeof(Snake));
    if(!S){
        printf("Allocation of snake failed!");
        exit(1);
    }

    Boundaries *b = calloc(1,sizeof(Boundaries));
    if(!b){
        printf("Allocation of boundaries failed!");
        exit(1);
    }
    b->x = xMax, b->y = yMax;
    S->bounds = b;

    CoordLL* first = calloc(1,sizeof(CoordLL));
    if(!first){
        printf("Allocation of coordinate array failed!");
        exit(1);
    }
    first->next = first->prev = NULL;

    Coord* coord = calloc(1,sizeof(Coord));
    if(!coord){
        printf("Allocation of coordinate failed!");
        exit(1);
    }
    first->loc = coord;

    // Snake will start in the center, moving right, and with length 1
    first->loc->x = (xMax+1)/2;
    first->loc->y = (yMax+1)/2;
    S->first = S->last = first;
    S->lastDir = KEY_RIGHT;

    return(S);
}

void delSnake(Snake* S){
    CoordLL* temp;
    while(S->first){
        temp = S->first;
        S->first = S->first->next;
        free(temp);
    }
    free(S->bounds);
    free(S);
}

