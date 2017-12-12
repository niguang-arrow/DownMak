#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>


#define RWRWRW 0666

#define oops(msg) {perror(msg); exit(1);}

int main() {
    /*umask(0);*/
    if (creat("foo", RWRWRW) < 0)
        oops("creat error for foo");
    
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (creat("bar", RWRWRW) < 0)
        oops("creat error for bar");

    return 0;
}
