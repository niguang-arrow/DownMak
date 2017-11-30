#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buf[4096];
    int fd, n;

    fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);

    while (1) {
        n = read(fd, buf, 4096);
        if (n > 0) {
            printf("ByeBye\n");
            break;
        }
        printf("I'm waiting!\n");
        sleep(1);
    }

    write(1, buf, n);
    close(fd);
    return 0;
}
