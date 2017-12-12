#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>


#define RWRWRW 0676

#define oops(msg) {perror(msg); exit(1);}

int main() {
    struct stat buf;
    if (creat("foo", RWRWRW) < 0)
        oops("creat error for foo");

    if (stat("foo", &buf) == -1)
        oops("stat");

    if (chmod("foo", (buf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        oops("chmod");
    return 0;
}
