#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    int n = 0;
    int c;

    while ((c = getchar()) != 'Q') {
        printf("char %3d is %c code %d\n", n++, c, c);
    }
    return 0;
}
