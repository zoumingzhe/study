#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define steps 256 * 1024 * 1024
#define minsize 1
#define maxsize 64 * 1024 * 1024
unsigned char array[maxsize];

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

void test_add_size(long size)
{
    size = max(size, minsize);
    size = min(size, maxsize);
    long mod = size - 1;
    clock_t start_t = clock();
    for (long i = 0; i < steps; i++)
    {
        array[(i * 64) & mod]++;
    }
    clock_t finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    if (size < 1024) {
        printf("[test_step_add] size: %4ldB use time：%0.3fs\n", size, total_t);
    } else if (size < 1024 * 1024) {
        printf("[test_step_add] size: %4ldKB use time：%0.3fs\n", size/1024, total_t);
    } else {
        printf("[test_step_add] size: %4ldMB use time：%0.3fs\n", size/1024/1024, total_t);
    }
}

void main()
{
    long size = sizeof(array);
    memset((void*)array, 0x0, size);
    printf("array sizeof : %ld\n", size);

    for (long s = minsize; s <= maxsize; s *= 2)
    {
        test_add_size(s);
    }
}
