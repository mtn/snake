#include <ncurses.h>
#include <string.h>

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

int main(){

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

    Window *gamewin = newwin(boundY,boundX,borderTB,borderLR);
    keypad(gamewin,TRUE);
    refresh();
    box(gamewin,0,0);

    Snake* S = newSnake(boundX-1,boundY-1);
    renderSnake(gamewin,S);

    halfdelay(5);
    do{
        choice = wgetch(gamewin);
        switch(choice){
            case KEY_UP:
                printw("u pressed");
                moveUp(S);
                break;
            case KEY_DOWN:
                printw("d pressed");
                moveDown(S);
                break;
            case KEY_LEFT:
                printw("l pressed");
                moveLeft(S);
                break;
            case KEY_RIGHT:
                printw("r pressed");
                moveRight(S);
                break;
            case ERR:
                break;
        }
        refresh();
        renderSnake(gamewin,S);;
    } while(true);

    getch();
    endwin();
}

