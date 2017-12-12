#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>


#define oops(msg) {perror(msg); exit(1);}


int main() {
    char *ptr;
    struct stat buf;

    if (stat("/usr/bin/passwd", &buf) == -1)
        oops("lstat");

    if (S_ISREG(buf.st_mode))
        ptr = "regular";
    else if (S_ISDIR(buf.st_mode))
        ptr = "directory";
    else
        ptr = "** unknown mode **";

    if (buf.st_mode & S_ISUID)
        printf("is uid\n");

    if (buf.st_mode & S_ISGID)
        printf("is gid\n");

    printf("%s\n", ptr);
    return 0;
}
