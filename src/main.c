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

void freeGW(GameWindow* GW){
    free(GW->isOccupied);
    delwin(GW->W);
    free(GW);
}

void updateScore(Window* scoreWin, Snake* S){
        wclear(scoreWin);
        box(scoreWin,0,0);
        wmove(scoreWin,2,1);
        wprintw(scoreWin,"Score: %d",S->len);
        wrefresh(scoreWin);
}

// This function is not very general, but could easily be changed as needed
int renderMenu(Window* W, int menuWidth, char* title, char* subtitle, int numOptions, char** options){
    wattron(W,A_REVERSE);
    mvwprintw(W,1,(menuWidth-strlen(title))/2,title);
    wattroff(W,A_REVERSE);
    mvwprintw(W,3,2,subtitle);

    int highlight = 0;
    while(true){
        for(int i = 0; i < numOptions; ++i){
            if(i==highlight){
                wattron(W,A_BOLD);
                mvwprintw(W,5+i,5,"*");
            }
            else
                mvwprintw(W,5+i,5," ");
            mvwprintw(W,5+i,6,options[i]);
            wattroff(W,A_BOLD);
        }
        int choice = wgetch(W);

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
        wrefresh(W);
        if(choice==10) break;
    }
    return highlight;
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

    char* difficulties[3] = { "Easy", "Medium", "Hard" };
    int choice, highlight = renderMenu(menuwin,menuWidth,"Snake","Select difficulty:",3,difficulties);
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
    gamewin->isOccupied[toOneD(S->first->loc->y,S->first->loc->x,boundX-1)] = true;
    renderSnake(gamewin->W,S);
    placeFood(gamewin,S);

    // Difficulty level determines delay
    int usDelay;
    switch(highlight){
        case 0:
            usDelay = 500000;
            halfdelay(5);
            break;
        case 1:
            usDelay = 300000;
            halfdelay(3);
            break;
        case 2:
            usDelay = 100000;
            halfdelay(1);
            break;
    }

    bool collided = false;
    clock_t t;
    double secsElapsed;
    int usElapsed;
    Window *scoreWin = newwin(4,12,yMax-3,xMax-12);
    refresh();
    updateScore(scoreWin,S);

    while(!collided){
        t = clock();
        flushinp();
        choice = wgetch(gamewin->W);
        t = clock() - t;
        secsElapsed = ((double)t)/CLOCKS_PER_SEC; // seconds
        usElapsed = (int)(secsElapsed*1000000); // microseconds
        if(choice == ERR) choice = S->lastDir;
        else usleep(usDelay-usElapsed);

        collided = moveSnake(gamewin,S,choice);
        renderSnake(gamewin->W,S);;
        updateScore(scoreWin,S);
    }
    int count = 0;
    for(int i = 0; i < boundX; ++i){
        for(int j = 0; j < boundY; ++j){
            if(gamewin->isOccupied[toOneD(j,i,S->bounds->x)]){
                wmove(gamewin->W,j,i);
                wprintw(gamewin->W,"x");
            }
        }
    }
    refresh();
    wrefresh(gamewin->W);

    cbreak();
    getch();
    freeGW(gamewin);
    clear();
    refresh();
    updateScore(scoreWin,S);
    Window *endGameWin = newwin(menuHeight,menuWidth,(yMax-menuHeight)/2,(xMax-menuWidth)/2);
    keypad(endGameWin,TRUE);
    refresh();

    box(endGameWin,0,0);
    char* endGameOptions[4] = { "Yes", "No" };
    choice = renderMenu(endGameWin,menuWidth,"End of game :(","Play Again?",2,endGameOptions);
    if(choice == 0) main();
    delwin(endGameWin);
    delwin(scoreWin);
    endwin();
}

