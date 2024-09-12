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
    printf("---  MUL. init.  --- \n");
    int fd, shmfd, n, shlen = sizeof(pthread_mutex_t);
    char *fpath = "./fifo_mul";
    pthread_mutex_t* mutex;
    double a, c, ac;

    shmfd = open("/dev/shm/myshm.file", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
    mutex = (pthread_mutex_t*)mmap(NULL, shlen, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
 
    // if(n = read(shmfd, mutex, sizeof(mutex))){
    //     printf("POW. %d bytes recived. \n", n);
    // } else {
    //     printf("POW. ERROR! mutex not received. \n");
    // }
    while(mutex) {
        if((fd = open(fpath, O_RDONLY)) < 0) {
            printf("MUL. cannot open fifo for read. \n");
        } else {
            printf("MUL. fifo was opened for read. \n");
        }
        read(fd, &a, sizeof(a));
        read(fd, &c, sizeof(c));

        close(fd);

        ac = a * c;
        pthread_mutex_lock(mutex);
        printf("MUL. mutex locks. \n");
        
        if((fd = open(fpath, O_WRONLY| O_TRUNC)) < 0) {
            printf("MUL. cannot open fifo for write. \n");
        } else {
            printf("MUL. fifo was opened for write. \n");
        }
        write(fd, &ac, sizeof(ac));
        printf("MUL. res = %lf. \n", ac);
        pthread_mutex_unlock(mutex);
        printf("MUL. mutex unlocks. \n");
    }
}