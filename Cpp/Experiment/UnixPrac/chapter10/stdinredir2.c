#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define USE_DUP2

int main() {

    int fd;
    int newfd;
    char line[100];

    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);

    fd = open("Makefile", O_RDONLY);
#ifndef USE_DUP2
    close(0);
    newfd = dup(fd);
    printf("not using dup2\n");
#else
    newfd = dup2(fd, 0);
    printf("using dup2\n");
#endif
    if (newfd != 0) {
        fprintf(stderr, "Could not duplicate fd to 0\n");
        exit(1);
    }
    close(fd);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
}
