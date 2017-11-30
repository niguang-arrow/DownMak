#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

    int res;
    if (argc != 3) {
        fprintf(stderr, "usage: %s oldname newname\n", *argv);
        exit(1);
    }

    if ((res = rename(argv[1], argv[2])) == -1) {
        perror(argv[1]);
        exit(1);
    }
    exit(EXIT_SUCCESS);
}
