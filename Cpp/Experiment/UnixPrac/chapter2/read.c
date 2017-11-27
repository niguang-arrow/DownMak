#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[]) {

    char buf[BUFFERSIZE];
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL)
        exit(1);

    while (!feof(fp)) {
        fgets(buf, BUFFERSIZE, fp);
        printf("%s\n", buf);
        break;
    }


    fclose(fp);

    return 0;
}
