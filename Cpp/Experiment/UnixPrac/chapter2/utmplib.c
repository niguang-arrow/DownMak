#include <stdio.h>
#include <utmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define NRECS 16
#define NULLUT ((struct utmp*)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open(char *filename) {
    if ((fd_utmp = open(filename, O_RDONLY)) == -1) {
        perror(filename);
        exit(1);
    }
    cur_rec = num_recs = 0;
    return fd_utmp;
}

/*每次读取 buf 中的一条记录*/
struct utmp* utmp_next() {
    struct utmp *recp;
    if (cur_rec == num_recs && utmp_reload() == 0)
        return NULLUT;

    recp = (struct utmp*)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}


int utmp_reload() {
    int amt_read;
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
    num_recs = amt_read / UTSIZE;
    cur_rec = 0;
    return num_recs;
}

void utmp_close() {
    if (fd_utmp != -1)
        if (close(fd_utmp) == -1) {
            fprintf(stderr, "Error Closing file");
        }
}
