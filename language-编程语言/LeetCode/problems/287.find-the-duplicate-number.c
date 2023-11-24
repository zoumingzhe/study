int findDuplicate(int* nums, int numsSize){
    char *tmp = (char*)malloc(numsSize);
    memset(tmp, 0x0, numsSize);
    for (int i = 0; i < numsSize; i++)
    {
        if (tmp[nums[i]])
        {
            return nums[i];
        }
        else
        {
            tmp[nums[i]] = 1;
        }
    }
    return 0;
}
