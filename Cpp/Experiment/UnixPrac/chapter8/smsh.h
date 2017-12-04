#ifndef SMSH_H
#define SMSH_H

#include <stdlib.h>
#define YES 1
#define NO 0

#define BUFSIZE 100

char *next_cmd();
char **splitline(char*);
void freelist(char**);
void *emalloc(size_t);
void *erealloc(void*, size_t);
int execute(char**);
void fatal(char*, char*, int);
int process(char**);
int builtin_command(char **, int *);

#endif
