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

typedef enum {BEGINNER, INTERMEDIATE, EXPERT} PlayerLevel;

bool wmvaddch(Window* W, int y, int x, int ch);
int toOneD(int y, int x, int maxX);
bool isOccupied(GameWindow* GW, int y, int x, int xMax);
void freeGW(GameWindow* GW);
int renderMenu(Window* W, int menuWidth, char* title, char* subtitle, int numOptions, char** options); // returns selected index

#endif /* MAIN_H */

