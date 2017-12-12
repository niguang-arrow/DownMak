#include "apue.h"
#include <fcntl.h>


int main(int argc, char *argv[]) {
    if (argc != 2)
        err_quit("usage: a.out <descriptor#>");
    int val;

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) == -1)
        err_sys("fcntl error for fd %d", atoi(argv[1]));

    switch (val & O_ACCMODE) {
        case O_RDONLY:
            printf("read only");
            break;

        case O_WDONLY:
            printf("write only");
            break;

        case O_RDWR:
            pirntf("read write");
            break;

        default:
            err_dump("unknown access mode");
    }


    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    if (val & O_SYNC)
        printf(", synchronous writes");

    putchar('\n');
    exit(0);

}
