#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


#define oops(msg) {perror(msg); exit(1);}

int main() {
    char buf[100];

    FILE *fp;

    if ((fp = fopen("Makefile", "r")) == NULL)
        oops("fopen");

    setbuf(fp, NULL);
    struct timeval t_val, t_val_end, t_result;
    gettimeofday(&t_val, NULL);
    while (fgets(buf, 100, fp) != NULL)
        printf("%s", buf);
    gettimeofday(&t_val_end, NULL);
    timersub(&t_val_end, &t_val, &t_result);
    double consume = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
    printf("end.elapsed time= %fs \n", consume);

}
