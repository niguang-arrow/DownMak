#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


#define BUFSIZE 100

int main() {

    int len, i, apipe[2];
    char buf[BUFSIZE];

    if (pipe(apipe) == -1) {
        perror("could not make pipe");
        exit(1);
    }
    printf("Got a pipe! It is file descriptors: {%d, %d}\n",
            apipe[0], apipe[1]);
    while(fgets(buf, BUFSIZE, stdin)) {
        len = strlen(buf);
        if (write(apipe[1], buf, len) != len) {
            perror("writing to pipe");
            exit(1);
        }
        for (i = 0; i < len; ++i)
            buf[i] = 'X';
        len = read(apipe[0], buf, BUFSIZE);
        if (len == -1) {
            perror("reading from pipe");
            break;
        }
        if ((write(1, buf, len)) != len) {
            perror("writing to stdout");
            exit(1);
        }
    }
}
