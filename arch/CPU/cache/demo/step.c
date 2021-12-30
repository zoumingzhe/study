#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define minstep 1
#define maxstep 1024
#define numelem 128 * 1024 * 1024
unsigned char array[numelem];

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

void test_add_step(long step)
{
    step = max(step, minstep);
    step = min(step, maxstep);
    clock_t start_t = clock();
    for (long i = 0; i < numelem; i++)
    {
        long n = i * step;
        array[n % numelem] *= 3;
    }
    clock_t finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    printf("[test_add_step] step: %4ld use time：%0.3fs\n", step, total_t);
}

void main()
{
    long size = sizeof(array);
    memset((void*)array, 0x1, size);
    printf("array sizeof : %ld\n", size);

    for (long s = minstep; s <= maxstep; s *= 2)
    {
        test_add_step(s);
    }
}
