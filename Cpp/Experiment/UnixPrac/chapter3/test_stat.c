#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


void show_info(struct stat *st) {
    printf("   mode: %o\n", st->st_mode);
    printf("  links: %ld\n", st->st_nlink);
    printf("   user: %d\n", st->st_uid);
    printf("  group: %d\n", st->st_gid);
    printf("   size: %ld\n", st->st_size);
    printf("modtime: %ld", st->st_mtime);
    printf("\n");
}

int main() {

    struct stat sb;
    if (stat("ls1.c", &sb) == -1) {
        perror("ls1.c");
        exit(1);
    }

    show_info(&sb);
    return 0;

}
