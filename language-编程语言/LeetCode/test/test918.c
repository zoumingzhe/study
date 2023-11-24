#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

typedef struct List{
    int value;
    struct List *prev;
    struct List *next;
}List;

List *make_list_item(int value)
{
    List *ret;
    ret = (List*)malloc(sizeof(List));
    ret->value = value;
    ret->prev = NULL;
    ret->next = NULL;
    return ret;
}

int* maxNumber(int* nums1, int nums1Size, int* nums2, int nums2Size, int k, int* returnSize){
    int ret_num = 0;
    int offset1 = 0;
    int offset2 = 0;
    int value2 = 0;

    returnSize = (int*)malloc(sizeof(int)*k);
    for (offset2 = 0; offset2 < nums2Size; offset2++)
    {
        value2 = nums2[offset2];
        for (i = offset1; i < nums1Size; i++)
        {
        }
    }
    
    return returnSize;
}

void test_case(void)
{
    int nums[5] = { 0 };
    int nums1[4] = { 3, 4, 6 ,5 };
    int nums2[6] = { 9, 1, 2, 5, 8, 3 };

    maxNumber(nums1, 4, nums2, 6, 5, nums);
    // 正确结果[9, 8, 6, 5, 3]
    printf("[%d, %d, %d, %d, %d]\n", nums[0],
        nums[1], nums[2], nums[3], nums[4]);
}

int main (void)
{
    test_case();
    return 0;
}
