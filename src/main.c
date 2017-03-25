#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lib/snake.h"
#include "lib/main.h"

bool wmvaddch(Window* W, int y, int x, int ch){ int xMax, yMax;
    getmaxyx(W,yMax,xMax);
    if(yMax < y || xMax < x) return false;

    wmove(W,y,x);
    waddch(W,ch);
    return true;
}

int toOneD(int y, int x, int maxX){ return y*maxX + x; }

bool isOccupied(GameWindow *GW, int y, int x, int xMax){
    int ind = toOneD(y,x,xMax);
    return GW->isOccupied[ind];
}

int main(){

    srand(time(NULL));

    initscr();
    noecho();
    curs_set(0);
    cbreak();

    int yMax, xMax, menuHeight=10, menuWidth=40;
    float relSize = 1.5; // ~1/3 of terminal should be border
    getmaxyx(stdscr,yMax,xMax);
    if(yMax <= menuHeight || xMax <= menuWidth){
        printw("Terminal window too small!");
        getch();
        endwin();
        return -1;
    }

    Window *menuwin = newwin(menuHeight,menuWidth,(yMax-menuHeight)/2,(xMax-menuWidth)/2);
    keypad(menuwin,TRUE);
    refresh();
    box(menuwin,0,0);

    wattron(menuwin,A_REVERSE);
    char* title = "Snake";
    mvwprintw(menuwin,1,(menuWidth-strlen(title))/2,title);
    wattroff(menuwin,A_REVERSE);

    char* difficulties[3] = { "Easy", "Medium", "Hard" };
    int choice, highlight=0;
    mvwprintw(menuwin,3,2,"Select difficulty:");

    while(true){
        for(int i = 0; i < 3; ++i){
            if(i==highlight){
                wattron(menuwin,A_BOLD);
                mvwprintw(menuwin,5+i,5,"*");
            }
            else
                mvwprintw(menuwin,5+i,5," ");
            mvwprintw(menuwin,5+i,6,difficulties[i]);
            wattroff(menuwin,A_BOLD);
        }
        choice = wgetch(menuwin);

        switch(choice){
            case KEY_DOWN:
                if(highlight < 2) highlight++;
                break;
            case KEY_UP:
                if(highlight > 0) highlight--;
            default:
                break;
        }
        refresh();
        // wgetch 10  -> enter -> no more rendering
        wrefresh(menuwin);
        if(choice==10) break;
    }
    delwin(menuwin);

    int boundY = yMax/relSize;
    int boundX = xMax/relSize;
    int borderTB = (yMax-boundY)/2;
    int borderLR = (xMax-boundX)/2;

    GameWindow *gamewin = malloc(sizeof(GameWindow));
    gamewin->W = newwin(boundY,boundX,borderTB,borderLR);
    gamewin->isOccupied = malloc(sizeof(bool)*boundX*boundY);
    for(int i = 0; i < boundX*boundY; ++i){
        if(i/boundX == 0 || i/boundX == boundY-1 || i % boundX == 0 || i % boundX == boundX-1)
            gamewin->isOccupied[i] = true;
        else gamewin->isOccupied[i] = false;
    }
    keypad(gamewin->W,TRUE);
    refresh();
    box(gamewin->W,0,0);

    Snake* S = newSnake(boundX-1,boundY-1);
    renderSnake(gamewin->W,S);
    placeFood(gamewin,S);

    // Difficulty level determines delay
    switch(highlight){
        case 0:
            halfdelay(10);
            break;
        case 1:
            halfdelay(5);
            break;
        case 2:
            halfdelay(1);
            break;
    }

    bool collided = false;
    while(!collided){
        choice = wgetch(gamewin->W);
        if(choice == ERR) choice = S->lastDir;
        switch(choice){
            case KEY_UP:
                collided = moveUp(gamewin,S);
                break;
            case KEY_DOWN:
                collided = moveDown(gamewin,S);
                break;
            case KEY_LEFT:
                collided = moveLeft(gamewin,S);
                break;
            case KEY_RIGHT:
                printw("moving right");
                refresh();
                collided = moveRight(gamewin,S);
                break;
        }
        refresh();
        renderSnake(gamewin->W,S);;
    }

    cbreak();
    getch();
    endwin();
}

