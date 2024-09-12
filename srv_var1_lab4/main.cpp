#include <math.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ctime>
#include <string>

// компиляция gcc main.c -o main -lrt
//int clock_gettime(clockid_t clock_id, struct timespec *t_time);

// 1) сортировка пузырьком случайного массива из 1000 элементов
// 2) сортировка вставками случайного массива из 1000 элементов
// 3) разложение на сомножители числа 909091 брутфорсом

// использовать аппаратный таймер

#define T 1000 // размеры массивов менять тут
#define END 50
#define START -50
#define COLUMN_W 7
#define MAX_HIST 50
#define NUM_W_HIST 4

#define array_init_random(aa, ll) {\
    for(size_t i = 0; i < ll; i++) {\
        aa[i] = new int[ll]; \
        for(size_t j = 0; j < ll; j++) {\
            aa[i][j] = rand() % (END - START + 1) + START; \
        } \
} }

#define array_init(aa,ll) { \
    for(size_t ii = 0; ii < ll; ii++) { \
        aa[ii] = 0; \
} } 

#define swap(_type, a, b) { \
    _type temp = a; \
    a = b; \
    b = temp; \
}

void bubble_sort(int*, int);
void insertion_sort(int*, int);
int len_num(int);
void hist(unsigned long*, int, int);
double get_average(unsigned long*, int);
double get_variance(unsigned long*, int);
void get_decomposition(int, int[], int);
long int A(void);
long int B(void);
long int C(void);

int main() {
    int i, count_of_iter, iter, count_of_col;
    struct timespec mt1, mt2;
    long int t1,t2,t3,t;  
    unsigned long *times = new unsigned long[count_of_iter];

    std::cout << "Enter count of iter" << std::endl;
    std::cin >> count_of_iter;

    std::cout << "Enter count of columns" << std::endl;
    std::cin >> count_of_col;

    for(iter = 0; iter < count_of_iter; iter++) {
        clock_gettime (CLOCK_REALTIME, &mt1);
        t1 = A();
        t2 = B();
        t3 = C();    
        clock_gettime (CLOCK_REALTIME, &mt2);
        t = 1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec);
        times[iter] = t;
    }

    std::cout << "---  1-2-3  ---" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter) 
            << "   DX = " 
            << get_variance(times, count_of_iter) << std::endl;
    hist(times, count_of_iter, count_of_col);

    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = A();  
        times[iter] = t1;
    }

    std::cout << "---  1  ---" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter) 
            << "   DX = " 
            << get_variance(times, count_of_iter) << std::endl;
    hist(times, count_of_iter, count_of_col);

    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = B();  
        times[iter] = t1;
    }

    std::cout << "---  2  ---" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter) 
            << "   DX = " 
            << get_variance(times, count_of_iter) << std::endl;
    hist(times, count_of_iter, count_of_col);

    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = C();  
        times[iter] = t1;
    }
    
    std::cout << "---  3  ---" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter) 
            << "   DX = " 
            << get_variance(times, count_of_iter) << std::endl;
    hist(times, count_of_iter, count_of_col);

    return 0;
}

long int A() {
    int i;
    struct timespec mt1, mt2;
    long int t1;
    // START BLOCK 1
    clock_gettime (CLOCK_REALTIME, &mt1);
    //
    int result_1[T] { };
    for(i = 0; i < T; i++) {
        result_1[i] = rand() % (END - START + 1) + START;
    }
    bubble_sort(result_1, T);
    //
    clock_gettime (CLOCK_REALTIME, &mt2);
    // END BLOCK 1

    t1=1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec);
    return t1;
}

long int B() {
    int i;
    struct timespec mt1, mt2;
    long int t2;
    // START BLOCK 2
    clock_gettime (CLOCK_REALTIME, &mt1);
    //
    int result_2[T] { };
    for(i = 0; i < T; i++) {
        result_2[i] = rand() % (END - START + 1) + START;
    }
    insertion_sort(result_2, T);
    //
    clock_gettime (CLOCK_REALTIME, &mt2);
    // END BLOCK 2

    t2=1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec);
    return t2;
}

long int C() {
    int i;
    struct timespec mt1, mt2;
    long int t3;
    // START BLOCK 3
    clock_gettime (CLOCK_REALTIME, &mt1);
    //
    int number_3 = 909091;
    int len_3 = number_3/2;
    int *result_3 = new int[len_3]{ };
    get_decomposition(number_3, result_3, len_3);
    //
    clock_gettime (CLOCK_REALTIME, &mt2);
    // END BLOCK 3
    t3=1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec);
    return t3;
}
double get_average(unsigned long *array, int size) {
    int i;
    double average;
    average = 0;
    for(i = 0; i < size; i++) {
        average += array[i];
        //std::cout << array[i] << std::endl;
    }
    return average/size;
}

double get_variance(unsigned long *array, int size) {
    int i;
    double variance;
    double average;
    variance = 0;
    average = get_average(array, size);
    for(i = 0; i < size; i++) {
        variance += pow((array[i] - average), 2);
    }
    return variance/ (size*(size - 1));
}

void hist(unsigned long *array, int size, int c) {
    unsigned long max, min, kk, k;
    int i, j;
    double m , maxc;
    double *cc = new double[c];
    array_init(cc, c);
    max = min = array[0];
    //std::cout << "0: " << array[0] << std::endl;
    for(i = 1; i < size; i++) {
        if(array[i] < min) min = array[i];
        else if(array[i] > max) max = array[i];
        //std::cout << i << ": " << array[i] << std::endl;
    }
    maxc = 0;
    k = (max - min) / c + 1;
    //std::cout << "k: " <<k << std::endl;
    for(i = 0; i < size; i++) {
        for(kk = min, j = 0; (kk < max) && (array[i] >= kk); kk+=k, j++) {
        }
        cc[j-1] = cc[j-1] + 1;
        if(cc[j-1] > maxc)  maxc = cc[j-1];
    }

    m = (MAX_HIST / maxc);
    //std::cout << "m: " << m << std::endl; 
    for(i = 0; i < c; i++) {
        //std::cout << cc[i] << std::endl; 
        cc[i] *= m;
        //std::cout << cc[i] << std::endl; 
        std::cout << i;
        for(j = 0; j < (NUM_W_HIST - len_num(i)); j++) {
            std::cout << " ";
        }
        std::cout << "|";
        for(j = 0; j < cc[i]; j++) {
            std::cout << "-";
        }
        std::cout << std::endl;
    }
}

int len_num(int num) {
    int c = num > 0? 0 : 1;
    while(num) {
        num /= 10;
        c++;
    }
    return c;
}

void bubble_sort(int *result, int size) {
    int i, j;
    for(i = 0; i < size - 1; i++) {
        for(j = 0; j < size-i-1; j++) {
            if(result[j] > result[j + 1]) {
                swap(int, result[j], result[j+1]);
            }
        }
    }
}
void insertion_sort(int* arr, int n){
	int counter=0;
	for(int i=1;i<n;i++){
		for(int j=i; j>0 && arr[j-1]>arr[j];j--){
			counter++;
			int tmp=arr[j-1];
			arr[j-1]=arr[j];
			arr[j]=tmp;
		}
	}
}

void get_decomposition(int number, int *result, int size) {
    int i;
    for (i = 2; i < size; i++) {
        while(number % i == 0) {
            number /= i;
            result[i] += 1;
        }
    }
    if(number != 1 && number < size) {
        result[number] += 1;
    }
}