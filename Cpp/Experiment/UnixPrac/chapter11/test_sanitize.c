#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void sanitize(char *str) {
    char *src, *dest;

    for (src = dest = str; *src; src++)
        if (*src == '/' || isalnum(*src))
            *dest++ = *src;

    *dest = '\0';
}

int main() {
    char buf[] = "/HOME/HE";
    sanitize(buf);

    printf("dirname: %s\n", buf);
    return 0;
}
