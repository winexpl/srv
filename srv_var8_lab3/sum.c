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
    flag1 = 1;
}

void sig_handler2(int sig) {
    flag2 = 1;
}


int main() {
    char *fpath = "./fifo_sum";
    int fd;
    double a2, b2, sum;
    signal(SIGUSR1, sig_handler1);
    signal(SIGUSR2, sig_handler2);


    while(flag2 == 0) {
        if((fd = open(fpath, O_RDONLY )) < 0) {
            printf("sum. cannot open fifo for read\n");
        } else {
            printf("sum. fifo was opened for read\n");
        }

        while(flag1 == 0) { }
        flag1 = 0;

        read(fd, &a2, sizeof(a2));
        read(fd, &b2, sizeof(b2));
        close(fd);

        sum = a2 + b2;
        
        if((fd = open(fpath, O_WRONLY| O_TRUNC)) < 0) {
            printf("sum. cannot open fifo for write\n");
        } else {
            printf("sum. fifo was opened for write\n");
        }

        write(fd, &sum, sizeof(sum));
        kill(getppid(), SIGUSR1);

    }
}