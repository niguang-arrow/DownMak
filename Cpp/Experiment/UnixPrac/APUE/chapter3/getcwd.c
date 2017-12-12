#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define oops(msg) {perror(msg); exit(1);}
#define BUFSIZE 100


int main() {
    
    char path[BUFSIZE];
    int fd;

    if ((fd = openat(AT_FDCWD, "../include/", O_RDONLY)) == -1)
        oops("open");

    if (fchdir(fd) == -1)
        oops("chdir failed");

    if (getcwd(path, BUFSIZE) == NULL)
        oops("getcwd failed");
    printf("%s\n", path);
    return 0;
}
