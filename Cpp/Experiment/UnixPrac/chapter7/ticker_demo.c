#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>


void countdown(int);
int set_ticker(int n_msecs);

int main() {
    signal(SIGALRM, countdown);
    if (set_ticker(500) == -1) {
        printf("hahhhhhhh");
        perror("set_ticker");
        exit(1);
    } else {
        while (1) {
            printf("hello world\n");
            pause();
            printf("kkkkkkk");
        }
    }
}

void countdown(int signum) {
    static int num = 10;
    printf("%d ..\n", num--);
    /*fflush(stdout);*/
    if (num < 0) {
        printf("DONE!\n");
        exit(0);
    }
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
