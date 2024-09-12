#include <math.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ctime>
#include <string>
#include <intrin.h>
#include <windows.h>

#define T 10
#define S 10
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

#define array_remove(a, l) {\
    for (size_t i = 0; i < l; i++) {\
        delete(a[i]);\
    }\
    delete(a);\
}

#define swap(_type, a, b) { \
    _type temp = a; \
    a = b; \
    b = temp; \
}


void get_decomposition(int, int[], int);
void out_decomposition(int, int[], int);
int is_primary_number(int);
void print_array(int*, int);
void print_array2(int**, int);
void bubble_sort(int*, int);
int len_num(int);
int** multiple(int**, int**);
void hist(unsigned __int64*,int , int);
double get_average(unsigned __int64*, int);
double get_variance(unsigned __int64*, int);
long double get_cpu(void);

int main(int argv, char **argc) {
    int i, count_of_iter, iter, count_of_col;
    double r;
    srand(time(NULL));
    unsigned __int64 t1, t2, _time;
    double CPU_Z = get_cpu();
    
    std::cout << "Enter count of iter" << std::endl;
    std::cin >> count_of_iter;

    std::cout << "Enter count of columns" << std::endl;
    std::cin >> count_of_col;

    unsigned __int64 *times = new unsigned __int64[count_of_iter];

    
    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = __rdtsc();
        // start block 1
        int number_1 = (argv > 1) ? atoi(argc[1]) : 909091;
        int len_1 = number_1/2 + 1;
        int *result_1 = new int[len_1]{ };
        get_decomposition(number_1, result_1, len_1);
        //out_decomposition(number_1, result_1, len_1);
        // end block 1

        r = rand() / static_cast<double>(RAND_MAX);

        //std::cout << r << std::endl;
        if (r < 0.5) {

            // start block 2
            int result_2[T] { };
            for(i = 0; i < T; i++) {
                result_2[i] = rand() % (END - START + 1) + START;
            }
            //print_array(result_2, T);
            bubble_sort(result_2, T);
            //print_array(result_2, T);
            // end block2
        } else {
            // start block3
            int **result_3;
            int **a = new int*[S];
            int **b = new int*[S];
            array_init_random(a, S);
            array_init_random(b, S);
            //print_array2(a, S);
            //print_array2(b, S);
            result_3 = multiple(a, b);
            //print_array2(result_3, S);
            // end block3
        }
        
        t2 = __rdtsc();
        _time=t2-t1;   
        //std::cout << _time << std::endl;
        //std::cout << _time / CPU_Z << std::endl;
        times[iter] = _time;
    }
    //print_array(times, count_of_iter);
    std::cout << "1-2-3" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter)/CPU_Z << "   DX = " << get_variance(times, count_of_iter)/CPU_Z << std::endl;

    hist(times, count_of_iter, count_of_col);
    

    //TIME 1

    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = __rdtsc();
        // start block 1
        int number_1 = (argv > 1) ? atoi(argc[1]) : 909091;
        int len_1 = number_1/2 + 1;
        int *result_1 = new int[len_1]{ };
        get_decomposition(number_1, result_1, len_1);
        //out_decomposition(number_1, result_1, len_1);
        // end block 1
        t2 = __rdtsc();
        _time=t2-t1;
        times[iter] = _time;
    }
    std::cout << "BLOCK 1" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter)/CPU_Z << "   DX = " << get_variance(times, count_of_iter)/CPU_Z << std::endl;

    hist(times, count_of_iter, count_of_col);   

    //TIME 2

    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = __rdtsc();
        // start block 2
        int result_2[T] { };
        for(i = 0; i < T; i++) {
            result_2[i] = rand() % (END - START + 1) + START;
        }
        //print_array(result_2, T);
        bubble_sort(result_2, T);
        //print_array(result_2, T);
        // end block2
        t2 = __rdtsc();
        _time=t2-t1;
        times[iter] = _time;
    }
    std::cout << "BLOCK 2" << std::endl;
    std::cout << "MX = " << get_average(times, count_of_iter)/CPU_Z <<  "   DX = " << get_variance(times, count_of_iter)/CPU_Z << std::endl;

    hist(times, count_of_iter, count_of_col); 

    //TIME 3

    for(iter = 0; iter < count_of_iter; iter++) {
        t1 = __rdtsc();
        // start block3
        int **result_3;
        int **a = new int*[S];
        int **b = new int*[S];
        array_init_random(a, S);
        array_init_random(b, S);
        //print_array2(a, S);
        //print_array2(b, S);
        result_3 = multiple(a, b);
        //print_array2(result_3, S);
        // end block3
        t2 = __rdtsc();
        _time=t2-t1;
        times[iter] = _time;
    }
    std::cout << "BLOCK 3" << std::endl;
    std::cout << "MX = " << (double)get_average(times, count_of_iter)/CPU_Z << "   DX = " << get_variance(times, count_of_iter)/CPU_Z << std::endl;

    hist(times, count_of_iter, count_of_col); 
    return 0;
}

long double get_cpu() {
    unsigned __int64 t1, t2;
    t1 = __rdtsc();
    Sleep(500);
    t2 = __rdtsc();
    double long res = (t2 - t1) * 1000 / 500;
    //std::cout << res << " -----";
    return res;
}

double get_average(unsigned __int64 *array, int size) {
    int i;
    double average;
    average = 0;
    for(i = 0; i < size; i++) {
        average += array[i];
        //std::cout << array[i] << std::endl;
    }
    return average/size;
}
double get_variance(unsigned __int64 *array, int size) {
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
void hist(unsigned __int64 *array, int size, int c) {
    unsigned __int64 max, min, kk, k;
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

void out_decomposition(int n, int *result, int size) {
    int i;
    bool flag = 0;
    std::cout << n << " " << "=";
    for(i = 0; i < size; i++) {
        if(result[i]) {
            if(flag) {
                std::cout << "*";
            } else {
                flag = 1;
            }
            if(result[i] > 1) {
                std::cout << " (" << result[i] << " * " << i << ") ";
            } else {
                std::cout << " " << i << " ";
            }
        } 
    } 
    if(!flag) {
        std::cout << " this number is a primary";
    }
    std::cout << std::endl;
}

void print_array2(int **result, int size) {
    int i, j, column_w_cur;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            column_w_cur = len_num(result[i][j]);
            
            while(COLUMN_W - column_w_cur) {
                column_w_cur++;
                std::cout << " ";
            }
            std::cout << result[i][j] ;
            
        } 
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void print_array(int *result, int size) {
    int i;
    std::cout << "[";
    for(i = 0; i < size; i++) {
        std::cout << " " << result[i] << " ";
        if(i != size - 1) {
            std::cout << ",";
        }
    }
    std::cout << "]" << std::endl;
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

int len_num(int num) {
    int c = num > 0? 0 : 1;
    while(num) {
        num /= 10;
        c++;
    }
    return c;
}

int** multiple(int **a, int **b) {
    int i, j, k;
    int **res = new int*[S];
    for (i = 0; i < S; i++) {
        res[i] = new int[S];
        array_init(res[i], S);
        for(j = 0; j < S; j++) {
            for (k = 0; k < S; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        } 
    }
    return res;
}