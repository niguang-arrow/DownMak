#include <stdio.h>
#include <stdlib.h>

int main() {

    char *cp = getenv("PATH");
    printf("PATH: %s\n", cp);
    return 0;

}
