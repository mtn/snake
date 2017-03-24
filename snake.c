#include <ncurses.h>
#include <string.h>

typedef WINDOW Window;

int main(){

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int yMax, xMax, borderTB, borderLR, menuHeight=10, menuWidth=40;
    float relSize = 1.5; // ~1/3 of terminal should be border
    getmaxyx(stdscr,yMax,xMax);
    printw("%d %d",yMax,xMax);
    if(yMax <= menuHeight || xMax <= menuWidth){
        printw("Terminal window too small!");
        getch();
        endwin();
        return -1;
    }

    Window *menuwin = newwin(menuHeight,menuWidth,(yMax-menuHeight)/2,(xMax-menuWidth)/2);
    refresh();
    box(menuwin,0,0);
    char* title = "Snake";
    attron(A_BOLD);
    mvwprintw(menuwin,1,(menuWidth-strlen(title))/2,title);
    attroff(A_BOLD);



    wrefresh(menuwin);
    keypad(menuwin,TRUE);

    borderTB = (yMax/relSize)/2;
    borderLR = (xMax/relSize)/2;

    getch();
    endwin();
}
