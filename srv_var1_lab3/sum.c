#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <memory.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    printf("---  SUM. init.  --- \n");
    int fd, shmfd, n, shlen = sizeof(pthread_mutex_t);
    char *fpath = "./fifo_sum";
    pthread_mutex_t* mutex;
    double a, b, c;

    shmfd = open("/dev/shm/myshm.file", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
    mutex = (pthread_mutex_t*)mmap(NULL, shlen, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
 
    while(mutex) {
        if((fd = open(fpath, O_RDONLY)) < 0) {
            printf("SUM. cannot open fifo for read. \n");
        } else {
            printf("SUM. fifo was opened for read. \n");
        }
        read(fd, &a, sizeof(a));
        read(fd, &b, sizeof(b));

        close(fd);

        c = a + b;
        pthread_mutex_lock(mutex);
        printf("SUM. mutex locks. \n");
        
        if((fd = open(fpath, O_WRONLY| O_TRUNC)) < 0) {
            printf("SUM. cannot open fifo for write. \n");
        } else {
            printf("SUM. fifo was opened for write. \n");
        }
        write(fd, &c, sizeof(c));
        printf("SUM. res = %lf. \n", c);
        pthread_mutex_unlock(mutex);
        printf("SUM. mutex unlocks. \n");
    }
}