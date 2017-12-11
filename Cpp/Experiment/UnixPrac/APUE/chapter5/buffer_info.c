#include <stdio.h>
#include <stdlib.h>


void pr_stdio(const char*, FILE*);
int is_unbuffered(FILE*);
int is_linebuffered(FILE*);
int buffer_size(FILE*);

#define oops(msg) {perror(msg); exit(1);}

int main() {
    FILE *fp;

    fputs("enter any character\n", stdout);
    if (getchar() == EOF)
        oops("getchar error");
    fputs("one line to standard error\n", stderr);


    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);


    if ((fp = fopen("/etc/passwd", "r")) == NULL)
        oops("fopen");

    if (getc(fp) == EOF)
        oops("getc error");

    pr_stdio("/etc/passwd", fp);
    exit(0);
}


void pr_stdio(const char *name, FILE *fp) {
    printf("stream = %s,  ", name);
    if (is_unbuffered(fp))
        printf("unbuffered");
    else if (is_linebuffered(fp))
        printf("line buffered");

    else
        printf("fully buffered");


    printf (", buffer size = %d\n", buffer_size(fp));
}


int
is_unbuffered(FILE *fp)
{
	return(fp->_flags & _IO_UNBUFFERED);
}

int
is_linebuffered(FILE *fp)
{
	return(fp->_flags & _IO_LINE_BUF);
}

int
buffer_size(FILE *fp)
{
	return(fp->_IO_buf_end - fp->_IO_buf_base);
}
