#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define oops(msg) {perror(msg); exit(1);}
#define N 3

int main() {

    pid_t pid;
    int i, status;

    for (i = 0; i < N; ++i) {
        if ((pid = fork()) == -1)
            oops("fork");
        if (pid == 0) {
            printf("pid: %d\n", getpid());
            exit(100 + i);
        }
    }


    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n",
                    pid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", pid);
    }


    if (errno != ECHILD)
        oops("waitpid error");

    char *argv[] = {"a", "b", "c", NULL};
    char **a = argv;
    while (*a) {

        printf("%s\n", *a++);
    }

    exit(0);
}
