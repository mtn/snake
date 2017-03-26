#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "lib/snake.h"
#include "lib/main.h"

bool wmvaddch(Window* W, int y, int x, int ch){
    int xMax, yMax;
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

    GameWindow *gamewin = calloc(1,sizeof(GameWindow));
    gamewin->W = newwin(boundY,boundX,borderTB,borderLR);
    gamewin->isOccupied = calloc(boundX*boundY,sizeof(bool));
    keypad(gamewin->W,TRUE);
    refresh();
    box(gamewin->W,0,0);

    Snake* S = newSnake(boundX-1,boundY-1);
    gamewin->isOccupied[toOneD(S->loc[0]->y,S->loc[0]->x,boundX-1)] = true;
    renderSnake(gamewin->W,S);
    placeFood(gamewin,S);

    // Difficulty level determines delay
    int msDelay;
    switch(highlight){
        case 0:
            msDelay = 500000;
            halfdelay(5);
            break;
        case 1:
            msDelay = 300000;
            halfdelay(3);
            break;
        case 2:
            msDelay = 100000;
            halfdelay(1);
            break;
    }

    bool collided = false;
    clock_t t;
    double secsElapsed;
    int msElapsed;
    while(!collided){
        t = clock();
        flushinp();
        choice = wgetch(gamewin->W);
        t = clock() - t;
        secsElapsed = ((double)t)/CLOCKS_PER_SEC; // seconds
        msElapsed = (int)(secsElapsed*1000000); // microseconds
        if(choice == ERR) choice = S->lastDir;
        else usleep(msDelay-msElapsed);

        collided = moveSnake(gamewin,S,choice);
        refresh();
        renderSnake(gamewin->W,S);;
    }

    cbreak();
    getch();
    endwin();
}

