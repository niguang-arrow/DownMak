#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

int main() {
    int i;
    initscr();
    clear();
    for (i = 0; i < LINES; ++i) {
        move(i, i + i);
        if (i % 2 == 1)
            standout();
        addstr("hello world");
        if (i % 2 == 1)
            standend();
        refresh();
        sleep(1);
        move(i, i + i);
        addstr("           ");
    }
    endwin();
}
