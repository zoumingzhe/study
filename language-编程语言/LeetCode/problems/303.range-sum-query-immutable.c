typedef struct {
    int *nums;
    int numsSize;
} NumArray;


NumArray* numArrayCreate(int* nums, int numsSize) {
    NumArray *obj = (NumArray*)malloc(sizeof(NumArray));
    obj->nums = nums;
    obj->numsSize = numsSize;
    return obj;
}

int numArraySumRange(NumArray* obj, int i, int j) {
    int sum = 0;
    for (int k = i; k <= j; k++)
        sum += obj->nums[k];
    return sum;
}

void numArrayFree(NumArray* obj) {
    free(obj);
}

/**
 * Your NumArray struct will be instantiated and called as such:
 * NumArray* obj = numArrayCreate(nums, numsSize);
 * int param_1 = numArraySumRange(obj, i, j);
 
 * numArrayFree(obj);
*/