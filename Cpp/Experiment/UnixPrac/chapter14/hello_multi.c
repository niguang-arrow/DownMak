#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define NUM 5

void* print_msg(void*);


int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, print_msg, (void*)"hello");
    pthread_create(&t2, NULL, print_msg, (void*)"world\n");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}


void* print_msg(void *m) {
    int i;
    char *cp = (char*)m;
    for(i = 0; i < NUM; ++i) {
        printf("%s", cp);
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}
