#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SHOWHOST

void show_info(struct utmp *utbufp) {
    if (utbufp->ut_type != USER_PROCESS)
        return;
    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    long timeval = utbufp->ut_time;
    printf("%12.12s", ctime(&timeval) + 4);
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

int main() {
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    printf("%s\n", UTMP_FILE);
    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen) {
        show_info(&current_record);
    }

    close(utmpfd);
    return 0;
}
