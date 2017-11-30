#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define QUESTION "Do you want another transaction"

int get_response(char*);
void set_crmode();
void tty_mode(int);

int main() {

    int response;
    tty_mode(0);
    set_crmode();
    response = get_response(QUESTION);
    tty_mode(1);
    printf("\nthe response is %d\n", response);
    return 0;
}

int get_response(char *question) {
    int input;
    printf("%s (y/n)?\n", question);
    while (1) {
        switch(input = getchar()) {
            case 'y':
            case 'Y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1;
            default:
                printf("\ncannot understand %c, ", input);
                printf("Please type y or no\n");
        }
    }
}


void set_crmode() {
    struct termios ttystate;
    tcgetattr(0, &ttystate); /* read curr. setting */
    ttystate.c_lflag &= ~ICANON; /* no buffering */
    ttystate.c_cc[VMIN] = 1; /*get 1 char at a time */
    tcsetattr(0, TCSANOW, &ttystate); /*install settings */
}


void tty_mode(int how) {
    static struct termios original_mode;
    if (how == 0)
        tcgetattr(0, &original_mode);
    else
        tcsetattr(0, TCSANOW, &original_mode);
}
