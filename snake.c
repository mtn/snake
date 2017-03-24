#include <ncurses.h>
#include <string.h>

typedef WINDOW Window;

int main(){

    initscr();
    noecho();
    curs_set(0);
    cbreak();

    int yMax, xMax, borderTB, borderLR, menuHeight=10, menuWidth=40;
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
        printw("highlight: %d",highlight);
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
                printw("keydown pressed");
                refresh();
                if(highlight < 2) highlight++;
                break;
            case KEY_UP:
                printw("keyup pressed");
                refresh();
                if(highlight > 0) highlight--;
            default:
                break;
        }
        refresh();
        // wgetch 10  -> enter -> no more rendering
        if(choice==10) break;
        wrefresh(menuwin);
    }
    printw("selected difficulty: %s",difficulties[highlight]);
    refresh();
    getch();

    keypad(menuwin,TRUE);

    borderTB = (yMax/relSize)/2;
    borderLR = (xMax/relSize)/2;

    getch();
    endwin();
}
