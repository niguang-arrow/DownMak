#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; ++i)
        printf("%.2x ", start[i]);
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *p) {
    show_bytes((byte_pointer) &p, sizeof(void *));
}


int main() {

    int x = 100;
    show_int(x);
    show_float((float)x);
    show_pointer(&x);

    const char *s = "abcde";
    show_bytes((byte_pointer) s, strlen(s));

    int y = 0x87654321;
    show_int(y);
    int z = ((y ^ ~0xFF) & ~0xFF) | (y & 0xFF);
    show_int(z);

    return 0;
}
