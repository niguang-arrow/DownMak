#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>


#define oops(msg) {perror(msg); exit(1);}

int main() {

    pid_t pid;
    char *args[] = {"ls", "-a", "-l", NULL};


    if ((pid = fork()) < 0) {
        oops("fork");
    }
    else if (pid == 0)
        if (execvp("ls", args) < 0)
            oops("execvp");

    if (wait(NULL) < 0)
        oops("wait");

    printf("child job done!\n");
    exit(0);

}
