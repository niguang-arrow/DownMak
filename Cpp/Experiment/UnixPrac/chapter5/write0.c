#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4096

int main(int argc, char *argv[]) {
    int fd;
    char buf[BUFSIZE];
    if (argc != 2) {
        fprintf(stderr, "usage: %s <ttyname>", *argv);
        exit(1);
    }

    if ((fd = open(argv[1], O_WRONLY)) == -1) {
        perror(argv[1]);
        exit(1);
    }

    while (fgets(buf, BUFSIZE, stdin) != NULL) {
        if (write(fd, buf, BUFSIZE) == -1)
            break;
    }
    close(fd);
}
