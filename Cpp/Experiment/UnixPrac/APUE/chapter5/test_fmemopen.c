#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BSZ 16

#define oops(msg) {perror(msg); exit(1);}


int main() {
    FILE *fp;
    char buf[BSZ];

    memset(buf, 'a', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';

    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL)
        oops("fmemopen");
    printf("initial buffer contents: %s\n", buf);
    fprintf(fp, "hello,world");
    printf("before flush: %s\n", buf);
    fflush(fp);
    printf("after fflush: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));


    memset(buf, 'b', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello,world");
    fflush(fp);
    printf("after seek: %s\n", buf);
}
