#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdbool.h>


typedef WINDOW Window;

typedef struct {
    int x;
    int y;
    bool isOccupied;
} Coord;

#endif /* MAIN_H */

