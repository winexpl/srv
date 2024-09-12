#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

volatile int flag1 = 0;
volatile int flag2 = 0;

void sig_handler1(int sig) {
    if(sig == SIGUSR1)
        flag1 = 1;
}

void sig_handler2(int sig) {
    if(sig == SIGUSR2)
        flag2 = 1;
}


int main() {
    char *fpath = "./fifo_pow";
    int fd;
    double a, a2;
    signal(SIGUSR1, sig_handler1);
    signal(SIGUSR2, sig_handler2);

    while(flag2 == 0) {
        printf("hi1\n");
        if((fd = open(fpath, O_RDONLY)) < 0) {
            printf("pow. cannot open fifo for read\n");
        } else {
            printf("pow. fifo was opened for read\n");
        }

        while(flag1 == 0) { }
        flag1 = 0;

        read(fd, &a, sizeof(a));
        close(fd);

        a2 = a * a;

        if((fd = open(fpath, O_WRONLY| O_TRUNC)) < 0) {
            printf("pow. cannot open fifo for write\n");
        } else {
            printf("pow. fifo was opened for write\n");
        }

        write(fd, &a2, sizeof(a2));
        kill(getppid(), SIGUSR1);
    }
}