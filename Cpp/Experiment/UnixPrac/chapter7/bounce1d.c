#include <stdio.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE "hello"
#define BLANK   "     "

void move_msg(int);
int set_ticker(int n_msecs);

int row;
int col;
int dir;

/*void set_cr_noecho_mode() {*/
    /*struct termios ttystate;*/
    /*tcgetattr(0, &ttystate); [> read curr. setting <]*/
    /*ttystate.c_lflag &= ~ICANON; [> no buffering <]*/
    /*ttystate.c_cc[VMIN] = 1; [>get 1 char at a time <]*/
    /*ttystate.c_lflag &= ~ECHO;*/
    /*tcsetattr(0, TCSANOW, &ttystate); [>install settings <]*/
/*}*/

int main() {
    int delay;
    int ndelay;
    int c;
    initscr();
    crmode();
    noecho();
    clear();


    row = 10;
    col = 0;
    dir = 5;
    delay = 200; /* 200ms */
    move(row, col);
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while (1) {
        ndelay = 0;
        c = getch();
        if (c == 'Q') break;
        if (c == ' ') dir = -dir;
        if (c == 'f' && delay > 2)  ndelay = delay/2;
        if (c == 's') ndelay = delay * 2;
        if (ndelay)
            set_ticker(delay = ndelay);
    }
    endwin();
    return 0;
}


void move_msg(int signum) {
    signal(SIGALRM, move_msg);
    move(row, col);
    addstr(BLANK);
    col += dir;
    move(row, col);
    addstr(MESSAGE);
    refresh();
    if (dir == -5 && col <= 0)
        dir = 5;
    else if (dir == 5 && col + strlen(MESSAGE) >= COLS)
        dir = -5;
}


int set_ticker(int n_msecs) {
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;

    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
