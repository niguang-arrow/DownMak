#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd;
    if ((fd = open("test", O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1) {
        perror("test");
        fprintf(stderr, "oh m g\n");
        exit(1);
    }

    char buf[] = "This is a test";
    int len;
    len = write(fd, buf, strlen(buf));
    if (len <= 0) {
        fprintf(stderr, "write Error");
        exit(1);
    }
    close(fd);


    return 0;
}
