#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define steps 64 * 1024 * 1024

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

int array[2];

void test_loop1()
{
    clock_t start_t = clock();
    for (long i = 0; i < steps; i++)
    {
        array[0]++;
        array[0]++;
    }
    clock_t finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    printf("[test_loop1] use time：%0.3fs\n", total_t);
}

void test_loop2()
{
    clock_t start_t = clock();
    for (long i = 0; i < steps; i++)
    {
        array[0]++;
        array[1]++;
    }
    clock_t finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    printf("[test_loop2] use time：%0.3fs\n", total_t);
}

void main()
{
    int size = sizeof(array);
    memset((void*)array, 0x0, size);
    printf("array sizeof : %d\n", size);

    test_loop1();
    test_loop2();
}
