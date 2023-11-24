#include <stdio.h>

#define max(a,b) (a>b?a:b)

int max_val(int *arr, int num)
{
    int sum = arr[0];
    int max = arr[0];
    for (int i = 1; i < num; i++)
    {
        sum += arr[i];
        max = max(max, sum);
    }
    return max;
}

int max_sub(int *arr, int num)
{
    int val = arr[0];
    int max = arr[0];
    for (int i = 0; i < num; i++)
    {
        val = max_val(arr+i, num-i);
        max = max(max, val);
    }
    return max;
}

int main(void)
{
    int arr[] = {6, -1, 3, -4, -6, 9, 2, -2, 5};
    printf("%d\n", max_sub(arr, sizeof(arr)/sizeof(int)));
    return 0;
}