#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <stdbool.h>


typedef WINDOW Window;

typedef struct {
    Window* W;
    bool* isOccupied;
} GameWindow;

typedef struct {
    int x;
    int y;
} Coord, *CoordPtr;

// Doubly-LL implementation for easy growth
typedef struct CoordLL {
    Coord* loc;
    struct CoordLL* next;
    struct CoordLL* prev;
} CoordLL, *CoordLLPtr;

bool wmvaddch(Window* W, int y, int x, int ch);
int toOneD(int y, int x, int maxX);
bool isOccupied(GameWindow* GW, int y, int x, int xMax);

#endif /* MAIN_H */

