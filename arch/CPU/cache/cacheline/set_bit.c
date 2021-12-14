#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define thdnum 2 // max 64
#define bitnum 8 * 1024
#define opsnum bitnum * 1024
unsigned char bits[bitnum*1024];
int thread_id = 0;

/* bitops */
#if 1
static inline void set_bit(int nr, volatile void * addr)
{
    // nr *= 8 * 64;
    volatile unsigned int *a = addr;
    int mask = 1 << (nr & 0x1f);
    a += nr >> 5;
    *a |= mask;
}
#else
static inline void set_bit(int nr, void * addr)
{
    unsigned int *a = addr;
    int mask = 1 << (nr & 0x1f);
    a += nr >> 5;
    *a |= mask;
}
#endif

void test_set_bits()
{
    clock_t start_t = clock();
    for (int i = 0; i < opsnum; i++)
    {
        int bitn = i / 1024;
        set_bit(bitn, bits);
    }
    clock_t finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    printf("[test_set_bits] use time：%0.3fs\n", total_t);
}

void* test_set_bits_thd(void* thd)
{
    int tid = __sync_add_and_fetch(&thread_id, 1) - 1;
    clock_t start_t = clock();
    for (int i = 0; i < opsnum / thdnum; i++)
    {
        int bitn = i / 1024;
        bitn *= thdnum;
        bitn += tid;
        set_bit(bitn, bits);
    }
    clock_t finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    printf("[test_set_bits_thd:%02d] use time：%0.3fs\n", tid, total_t);
}


void main()
{
    int size = sizeof(bits);
    memset((void*)bits, 0x0, size);
    printf("bits sizeof : %d\n", size);
    test_set_bits();

    pthread_t threads[thdnum];
    for (int i = 0; i < thdnum; i++)
    {
        pthread_create(threads+i, NULL, test_set_bits_thd, NULL);
    }
    pthread_exit(NULL);
}
