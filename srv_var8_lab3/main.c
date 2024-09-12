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

int main(int argc, char *argv[]) {
    char *fpath_pow, *fpath_sum, *fpath_sqrt;
    int child, fifo, fd, parid, bytes, childs[3];
    double a, b, a2, b2, sum, sqrt1;

    fpath_pow = "./fifo_pow";
    fpath_sum = "./fifo_sum";
    fpath_sqrt = "./fifo_sqrt";

    unlink(fpath_pow); 
    unlink(fpath_sum);
    unlink(fpath_sqrt);
    
    printf("input a: ");
    scanf("%lf", &a);

    printf("input b: ");
    scanf("%lf", &b);


    parid = getpid();
    printf("%d\n", parid);
    
    signal(SIGUSR1, sig_handler1);
    signal(SIGUSR2, sig_handler2);

    if ((fifo = mkfifo(fpath_pow, 0777)) < 0) {
        printf("cannot create fifo for pow.\n");
    } else {
        printf("fifo was created for pow. fifo = %d\n", fifo);
    }
    if ((fifo = mkfifo(fpath_sum, 0777)) < 0) {
        printf("cannot create fifo for sum.\n");
    } else {
        printf("fifo was created for sum. fifo = %d\n", fifo);
    }
    if ((fifo = mkfifo(fpath_sqrt, 0777)) < 0) {
        printf("cannot create fifo for sqrt.\n");
    } else {
        printf("fifo was created for sqrt. fifo = %d\n", fifo);
    }

    child = fork();
    if(child == 0) {
        char *argv[2] = {"pow", 0};
        execvp("./pow", argv);
    } else {
        childs[0] = child; 
        //обработка a
        if((fd = open(fpath_pow, O_WRONLY| O_TRUNC)) < 0) {
            printf("main. cannot open fifo for write (pow). fd = %d, errno = %d\n", fd, errno);
        } else {
            printf("main. fifo was opened for write (pow), fd = %d\n", fd);
        }
        write(fd, &a, sizeof(a));
        close(fd);
        kill(child, SIGUSR1);
        if((fd = open(fpath_pow, O_RDONLY | O_NONBLOCK)) < 0) {
            printf("main. cannot open fifo for read (pow). fd = %d, errno = %d\n", fd, errno);
        } else {
            printf("main. fifo was opened for read (pow). fd = %d\n", fd);
        }
        
        while(flag1 == 0) {}
        flag1 = 0;

        bytes = read(fd, &a2, sizeof(a2));

        printf("parent give from fifo a2 = %f, bytes = %d\n", a2, bytes);

        //обработка b

        if((fd = open(fpath_pow, O_WRONLY| O_TRUNC)) < 0) {
            printf("main. cannot open fifo for write (pow). fd = %d, errno = %d\n", fd, errno);
        } else {
            printf("main. fifo was opened for write (pow), fd = %d\n", fd);
        }
        write(fd, &b, sizeof(b));
        close(fd);
        kill(child, SIGUSR1);
        if((fd = open(fpath_pow, O_RDONLY | O_NONBLOCK)) < 0) {
            printf("main. cannot open fifo for read (pow). fd = %d, errno = %d\n", fd, errno);
        } else {
            printf("main. fifo was opened for read (pow). fd = %d\n", fd);
        }
        
        while(flag1 == 0) {}
        flag1 = 0;

        read(fd, &b2, sizeof(b2));

        printf("parent give from fifo b2 = %f\n", b2);
        
        child = fork();


        if(child == 0) {
            char *argv[2] = {"sum", 0};
            execvp("./sum", argv);
        }
        else {
            childs[1] = child;
            if((fd = open(fpath_sum, O_WRONLY| O_TRUNC)) < 0) {
                printf("main. cannot open fifo for write (sum). fd = %d, errno = %d\n", fd, errno);
            } else {
                printf("main. fifo was opened for write (sum), fd = %d\n", fd);
            }
            write(fd, &a2, sizeof(a2));
            write(fd, &b2, sizeof(b2));
            close(fd);

            kill(child, SIGUSR1);

            if((fd = open(fpath_sum, O_RDONLY | O_NONBLOCK)) < 0) {
                printf("main. cannot open fifo for read (sum). fd = %d, errno = %d\n", fd, errno);
            } else {
                printf("main. fifo was opened for read (sum), fd = %d\n", fd);
            }
            while(flag1 == 0) { }
            flag1 = 0;

            read(fd, &sum, sizeof(sum));
            close(fd);
            printf("parent give from fifo sum = %f\n", sum);

            child = fork();

            if(child == 0) {
                char *argv[2] = {"sqrt", 0};
                execvp("./sqrt", argv);
            } else {
                childs[2] = child;
                if((fd = open(fpath_sqrt, O_WRONLY | O_TRUNC)) < 0) {
                    printf("main. cannot open fifo for write (sqrt). fd = %d, errno = %d\n", fd, errno);
                } else {
                    printf("main. fifo was opened for write (sqrt). fd = %d\n", fd);
                }
                write(fd, &sum, sizeof(sum));
                close(fd);

                kill(child, SIGUSR1);

                if((fd = open(fpath_sqrt, O_RDONLY | O_NONBLOCK)) < 0) {
                    printf("main. cannot open fifo for read (sqrt). fd = %d, errno = %d\n", fd, errno);
                } else {
                    printf("main. fifo was opened for read (sqrt). fd = %d\n", fd);
                }

                while(flag1 == 0) {}
                flag1 = 0;

                read(fd, &sqrt1, sizeof(sqrt1));

                printf("THE HYPOTENUSE IS EQUAL TO %f\n", sqrt1);

                unlink(fpath_pow); 
                unlink(fpath_sum);
                unlink(fpath_sqrt);

                for(int i = 0; i < 3; i++) {
                    kill(childs[i], SIGUSR2);
                }
            }
        }   
    }
}