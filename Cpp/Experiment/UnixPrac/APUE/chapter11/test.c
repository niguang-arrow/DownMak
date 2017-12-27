#include <stdio.h>

int x = 1000;
int y = 1000;

int main() {
    printf("&x=0x%x, &y=0x%x\n", &x, &y);
    /*x = -0.0;*/
    foo();
    
    printf("x=0x%x, y=0x%x;\n", x, y);
    printf("x=%d, y=%d;\n", x, y);
}
