#include <ncurses.h>

typedef WINDOW Window;

int main(){

    initscr();
    cbreak();
    noecho();

    int yMax, xMax, borderTB, borderLR, menuHeight=80, menuWidth=40;
    float relSize = 1.5; // ~1/3 of terminal should be border
    getmaxyx(stdscr,yMax,xMax);
    if(yMax <= menuHeight || xMax <= menuWidth){
        printw("Terminal window too small!");
        getch();
        endwin();
        return -1;
    }

    Window *menuwin = newwin(menuHeight,menuWidth,(yMax-menuHeight)/2,(xMax-menuWidth)/2);
    refresh();
    box(menuwin,0,0);
    wrefresh(menuwin);
    keypad(menuwin,TRUE);

    borderTB = (yMax/relSize)/2;
    borderLR = (xMax/relSize)/2;


    /* int x,y; */
    /* move(10,10); */
    /* getyx(stdscr,y,x); */
    /* printw("\u2B24"); */

    getch();
    endwin();
}
