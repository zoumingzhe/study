/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* productExceptSelf(int* nums, int numsSize, int* returnSize){
    int* ret  = (int*)malloc(sizeof(int)*numsSize);
    int* tmp1 = (int*)malloc(sizeof(int)*(numsSize));
    int* tmp2 = (int*)malloc(sizeof(int)*(numsSize));

    tmp1[0] = 1;
    for (int i = 0; i < numsSize - 1; i++)
    {
        tmp1[i+1] = tmp1[i]*nums[i];
    }

    tmp2[numsSize - 1] = 1;
    for (int i = numsSize - 1; i > 0; i--)
    {
        tmp2[i-1] = tmp2[i]*nums[i];
    }

    for (int i = 0; i < numsSize; i++)
    {
        ret[i] = tmp1[i]*tmp2[i];
    }

    *returnSize = numsSize;
    return ret;
}
