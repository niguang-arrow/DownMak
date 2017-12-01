#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    int i;
    signal(SIGINT, SIG_IGN);
    for (i = 0; i < 5; ++i) {
        printf("hello\n");
        sleep(2);
    }
}
