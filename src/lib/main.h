#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdbool.h>


typedef WINDOW Window;

typedef struct {
    int x;
    int y;
} Coord, *CoordPtr;

bool wmvaddch(Window* W, int y, int x, int ch);

#endif /* MAIN_H */

