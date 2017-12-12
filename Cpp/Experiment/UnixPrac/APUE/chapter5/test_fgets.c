#include <stdio.h>
#include <stdlib.h>


int main() {

    char buf[10];

    if (fgets(buf, 10, stdin) != NULL)
        printf("%s", buf);
    printf("END\n");
    return 0;
}
