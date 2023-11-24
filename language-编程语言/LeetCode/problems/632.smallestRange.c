/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#define max(a,b) (a)>(b)?(a):(b)
#define min(a,b) (a)<(b)?(a):(b)

int* smallestRange(int** nums, int numsSize, int* numsColSize, int* returnSize){
    int* ret = (int*)malloc(sizeof(int)*2);
    int  tmp[2];
    *returnSize = 2;
    ret[0] = nums[0][0];
    ret[1] = nums[0][numsColSize[0]-1];

    for (int i = 1; i < numsSize; i++)
    {
        tmp[0] = nums[i][0];
        tmp[1] = nums[i][numsColSize[i]-1];
        ret[0] = max(ret[0], tmp[0]);
        ret[1] = min(ret[1], tmp[1]);
    }
    return ret;
}
