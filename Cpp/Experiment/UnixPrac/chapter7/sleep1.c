#include <stdio.h>
#include <signal.h>
#include <unistd.h>


void wakeup(int x) {
#ifndef SHHHH
    printf("Alarm received from kernel\n");
#endif
}


int main() {
    printf("about to sleep for 4 seconds\n");
    signal(SIGALRM, wakeup);
    alarm(4);
    pause();
    printf("Morning so soon? \n");
    return 0;
}
