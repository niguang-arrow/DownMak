#include <stdio.h>


static long add(long x, long y) {
    long a = 10;
    long z = x + y;
    long res = a + z + x;
    return res;
}


static void str() {
    char p[] = "hello world";
    printf("%s\n", p);
}


int main() {

    long x = 10;
    long y = 100;


    long res = add(x, y);

    printf("result: %ld\n", res);
    return 0;
}
