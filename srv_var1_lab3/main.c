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
#include <signal.h>
#include <sys/stat.h>

int create_fifo(void);
int delete_fifo(void);
void input_var(double*, double*, double*);
void multiple(pthread_mutex_t*, double*, double*, double*);
void subtraction(pthread_mutex_t*, double*, double*, double*);
void summ(pthread_mutex_t*, double*, double*, double*);
void squary_root(pthread_mutex_t*, double*, double*);


static char *fpath_mul, *fpath_sum, *fpath_sqrt, *fpath_div, *fpath_sub;


int main() {
    // x1 = -b+sqrt(D)/2a
    // x2 = -b-sqrt(D)/2a
    // D = b^2 - 4ac

    // 1) b^2
    // 2) a*c
    // 3) (4)*(ac)
    // 4) (b^2) - (4ac)     = D
    // 5) sqrt(D)
    // 6) (-b)+(sqrt(D))
    // 7) (-b)-(sqrt(D))
    // 8) 2*a
    // 9) (-b+sqrt(D))/(2a)
    // 10) (-b-sqrt(D))/(2a)

    fpath_mul = "./fifo_mul";
    fpath_sum = "./fifo_sum";
    fpath_sqrt = "./fifo_sqrt";
    fpath_div = "./fifo_div";
    fpath_sub = "./fifo_sub";
    double a, b, c, double_a, ac, quadruple_ac, b2, d, sqrt_d, inv_b, four = 4, t1, t2, two = 2, x1, x2;
    int child, fifo, fd, parid, bytes, childs[5];
    int shlen = sizeof(pthread_mutex_t);
    int n;
    pthread_mutex_t* mutex;
 
    parid = getpid();
    printf("%d\n", parid);

    create_fifo();

    // create the shared memory
    int shmfd = open("/dev/shm/myshm.file", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
    mutex = (pthread_mutex_t*)mmap(NULL, shlen, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
    pthread_mutexattr_t attr;  
    pthread_mutexattr_init(&attr); 
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attr);  

    if(n = write(shmfd, mutex, shlen)){
        printf("MAIN. %d bytes sent. \n", n);
    } else {
        printf("MAIN. ERROR! mutex not sent. \n");
    }

    child = fork();
    if(child) {
        char *argv[2] = {"mul", 0};
        execvp("./mul", argv);
        return 0;
    }
    childs[0] = child;
    child = fork();
    if(child) {
        char *argv[2] = {"sub", 0};
        execvp("./sub", argv);
        return 0;
    } 
    childs[1] = child; 

    child = fork();
    if(child) {
        char *argv[2] = {"div", 0};
        execvp("./div", argv);
        return 0;
    }
    childs[2] = child; 

    child = fork();
    if(child) {
        char *argv[2] = {"sqrt", 0};
        execvp("./sqrt", argv);
        return 0;
    }
    childs[3] = child;
    
    child = fork();
    if(child) {
        char *argv[2] = {"sum", 0};
        execvp("./sum", argv);
        return 0;
    }
    childs[4] = child;

    input_var(&a, &b, &c);
    printf("MAIN. thanks:) \n");
    printf("%.1lf x^2 + %.1lf x + %.1lf\n", a,b,c);

    // b^2
    multiple(mutex, &b, &b, &b2);
    printf("--- 1) MAIN. b^2 = %lf. ---\n", b2);

    // ac
    multiple(mutex, &a, &c, &ac);
    printf("--- 2) MAIN. ac = %lf. ---\n", ac);

    // 4*(ac)
    multiple(mutex, &four, &ac, &quadruple_ac);
    printf("--- 3) MAIN. 4ac = %lf. ---\n", quadruple_ac);

    // (b^2) - (4ac)    = D
    subtraction(mutex, &b2, &quadruple_ac, &d);
    printf("--- 4) MAIN. d = %lf. ---\n", d);
    if(d < 0) {
        printf("УРАВНЕНИЕ НЕ ИМЕЕТ РЕШЕНИЙ!! \n");
        return 0;
    }
    else if(d == 0) {
        printf("УРАВНЕНИЕ ИМЕЕТ ОДНО РЕШЕНИЕ!! \n");
        // 2*a
        multiple(mutex, &two, &a, &double_a);
        printf("--- 5) MAIN. 2a = %lf. ---\n", double_a);

        // (-b) / (2a)
        inv_b = -b;
        division(mutex, &inv_b, &double_a, &x2);
        printf("--- 6) MAIN. x = %lf. ---\n", x2);

        printf("--- x = %lf ---\n", x2);
        // for(int i = 0; i < 5; i++) {
        //     kill(childs[i], SIGABRT);
        // }
    }
    else {
        printf("УРАВНЕНИЕ ИМЕЕТ 2 РЕШЕНИЯ!! \n");
        
        // sqrt(D)
        squary_root(mutex, &d, &sqrt_d);
        printf("--- 5) MAIN. sqrt(d) = %lf. ---\n", sqrt_d);

        // (-b) + (sqrt(D))
        inv_b = -b;
        summ(mutex, &inv_b, &sqrt_d, &t1);
        printf("--- 6) MAIN. (-b) + (sqrt(D)) = %lf. ---\n", t1);

        // (-b) - (sqrt(D))
        subtraction(mutex, &inv_b, &sqrt_d, &t2);
        printf("--- 7) MAIN. (-b) - (sqrt(D)) = %lf. ---\n", t2);
        
        // 2*a
        multiple(mutex, &two, &a, &double_a);
        printf("--- 8) MAIN. 2a = %lf. ---\n", double_a);

        // (-b + sqrt(D)) / (2a)
        division(mutex, &t1, &double_a, &x1);
        printf("--- 9) MAIN. x1 = %lf. ---\n", x1);

        // (-b - sqrt(D)) / (2a)
        division(mutex, &t2, &double_a, &x2);
        printf("--- 10) MAIN. x2 = %lf. ---\n", x2);
        // for(int i = 0; i < 5; i++) {
        //     kill(childs[i], SIGABRT);
        // }
        printf("--- x1 = %lf --- \n--- x2 = %lf ---\n ", x1, x2);
    }

    delete_fifo();
    munmap(mutex, shlen);
    close(shmfd);
    
    return 0;
}


int create_fifo() {
    int fifo;

    if ((fifo = mkfifo(fpath_mul, 0777)) < 0) {
        printf("MAIN. cannot create fifo for mul.\n");
    } else {
        printf("MAIN. fifo was created for mul. fifo = %d\n", fifo);
    }
    if ((fifo = mkfifo(fpath_sum, 0777)) < 0) {
        printf("MAIN. cannot create fifo for sum.\n");
    } else {
        printf("MAIN. fifo was created for sum. fifo = %d\n", fifo);
    }
    if ((fifo = mkfifo(fpath_sqrt, 0777)) < 0) {
        printf("MAIN. cannot create fifo for sqrt.\n");
    } else {
        printf("MAIN. fifo was created for sqrt. fifo = %d\n", fifo);
    }
    if ((fifo = mkfifo(fpath_div, 0777)) < 0) {
        printf("MAIN. cannot create fifo for div.\n");
    } else {
        printf("MAIN. fifo was created for div. fifo = %d\n", fifo);
    }
    if ((fifo = mkfifo(fpath_sub, 0777)) < 0) {
        printf("MAIN. cannot create fifo for sub.\n");
    } else {
        printf("MAIN. fifo was created for sub. fifo = %d\n", fifo);
    }
    return 0;
}

int delete_fifo() {
    unlink(fpath_mul); 
    unlink(fpath_sum);
    unlink(fpath_sqrt);
    unlink(fpath_div);
    unlink(fpath_sub);
    return 0;
}

void input_var(double* a, double* b, double* c) {
    printf("input a: ");
    scanf("%lf", a);

    printf("input b: ");
    scanf("%lf", b);

    printf("input c: ");
    scanf("%lf", c);
}


void multiple(pthread_mutex_t* mutex, double *a, double *b, double *res) {
    int fd;
    pthread_mutex_lock(mutex);
    printf("MAIN. mutex locks. \n");
    if((fd = open(fpath_mul, O_WRONLY | O_TRUNC )) < 0) {
        printf("MAIN. cannot open fifo for write (mul). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for write (mul). fd = %d\n", fd);
    }
    write(fd, a, sizeof(*a));
    write(fd, b, sizeof(*b));
    close(fd);
    pthread_mutex_unlock(mutex);
    printf("MAIN. mutex unlocks .\n");

    if((fd = open(fpath_mul, O_RDONLY)) < 0) {
        printf("MAIN. cannot open fifo for read (mul). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for read (mul). fd = %d\n", fd);
    }
    read(fd, res, sizeof(*res));
    close(fd);
}


void subtraction(pthread_mutex_t* mutex, double *a, double *b, double *res) {
    int fd;
    pthread_mutex_lock(mutex);
    printf("MAIN. mutex locks. \n");
    if((fd = open(fpath_sub, O_WRONLY | O_TRUNC )) < 0) {
        printf("MAIN. cannot open fifo for write (sub). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for write (sub). fd = %d\n", fd);
    }
    write(fd, a, sizeof(*a));
    write(fd, b, sizeof(*b));
    close(fd);
    pthread_mutex_unlock(mutex);
    printf("MAIN. mutex unlocks .\n");

    if((fd = open(fpath_sub, O_RDONLY)) < 0) {
        printf("MAIN. cannot open fifo for read (sub). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for read (sub). fd = %d\n", fd);
    }
    read(fd, res, sizeof(*res));
    close(fd);
}

void summ(pthread_mutex_t* mutex, double *a, double *b, double *res) {
    int fd;
    pthread_mutex_lock(mutex);
    printf("MAIN. mutex locks. \n");
    if((fd = open(fpath_sum, O_WRONLY | O_TRUNC )) < 0) {
        printf("MAIN. cannot open fifo for write (sum). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for write (sum). fd = %d\n", fd);
    }
    write(fd, a, sizeof(*a));
    write(fd, b, sizeof(*b));
    close(fd);
    pthread_mutex_unlock(mutex);
    printf("MAIN. mutex unlocks .\n");

    if((fd = open(fpath_sum, O_RDONLY)) < 0) {
        printf("MAIN. cannot open fifo for read (sum). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for read (sum). fd = %d\n", fd);
    }
    read(fd, res, sizeof(*res));
    close(fd);
}

void division(pthread_mutex_t* mutex, double *a, double *b, double *res) {
    int fd;
    pthread_mutex_lock(mutex);
    printf("MAIN. mutex locks. \n");
    if((fd = open(fpath_div, O_WRONLY | O_TRUNC )) < 0) {
        printf("MAIN. cannot open fifo for write (div). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for write (div). fd = %d\n", fd);
    }
    write(fd, a, sizeof(*a));
    write(fd, b, sizeof(*b));
    close(fd);
    pthread_mutex_unlock(mutex);
    printf("MAIN. mutex unlocks .\n");

    if((fd = open(fpath_div, O_RDONLY)) < 0) {
        printf("MAIN. cannot open fifo for read (div). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for read (div). fd = %d\n", fd);
    }
    read(fd, res, sizeof(*res));
    close(fd);
}


void squary_root(pthread_mutex_t* mutex, double *a, double *res) {
    int fd;
    pthread_mutex_lock(mutex);
    printf("MAIN. mutex locks. \n");
    if((fd = open(fpath_sqrt, O_WRONLY | O_TRUNC )) < 0) {
        printf("MAIN. cannot open fifo for write (sqrt). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for write (sqrt). fd = %d\n", fd);
    }
    write(fd, a, sizeof(*a));
    close(fd);
    pthread_mutex_unlock(mutex);
    printf("MAIN. mutex unlocks .\n");

    if((fd = open(fpath_sqrt, O_RDONLY)) < 0) {
        printf("MAIN. cannot open fifo for read (sqrt). fd = %d, errno = %d\n", fd, errno);
    } else {
        printf("MAIN. fifo was opened for read (sqrt). fd = %d\n", fd);
    }
    read(fd, res, sizeof(*res));
    close(fd);
}