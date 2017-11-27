/*open, close 函数用于文件的处理, 它们定义在 <fcntl.h> 文件中*/
/*之后从相应的文件中读取数据, 需要使用 read, 定义在 <unistd.h> 文件中*/
/*最后要将处理的结果输出到屏幕上, 需要 <stdio.h>*/
/*另外, 由于 who 命令需要处理 utmp 结构, 该结构定义在 <utmp.h> 文件中*/

#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define SHOW_HOST

void show_info(struct utmp *utbufp);

int main() {
    struct utmp current_record;
    int utmpfd;  /*file descriptor*/
    int reclen = sizeof(current_record);

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
    if (utbufp->ut_host[0] != '\0')
        printf("(%s", utbufp->ut_host);
#endif
    printf("\n");
}
