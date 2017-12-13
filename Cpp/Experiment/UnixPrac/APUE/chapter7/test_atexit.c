#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


static void my_exit1(void);
static void my_exit2(void);

#define oops(msg) {perror(msg); exit(1);}

int main() {

    pid_t pid;

    if (atexit(my_exit2) != 0)
        oops("atexit");
    if (atexit(my_exit1) != 0)
        oops("atexit");
    if (atexit(my_exit1) != 0)
        oops("atexit");

    if ((pid = fork()) == -1)
        oops("fork");
    if (pid == 0) {
        printf("I am child\n");
        execlp("ls", "ls", "-l", NULL);
        oops("execlp");
    }
    else {
        if (wait(NULL) == -1)
            oops("wait");
        printf("I am father\n");
    }

    exit(0);

}


static void my_exit1() {
    printf("my_exit1\n");
}

static void my_exit2() {
    printf("my_exit2\n");
}
