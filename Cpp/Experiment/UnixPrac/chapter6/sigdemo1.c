#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void f(int signum) {
    printf("OUCH!\n");
}

int main() {
    int i;
    signal(SIGINT, f);
    for (i = 0; i < 5; ++i) {
        printf("hello\n");
        sleep(2);
    }
}
