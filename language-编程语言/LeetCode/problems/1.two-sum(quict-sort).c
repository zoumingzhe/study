void quictsort(int* nums, int head, int tail) {
	int i = head;
	int j = tail;
	int tmp = nums[i];
	while (i < j)
	{
		for (; j > i; j--)
		{
			if (nums[j] < tmp)
			{
				nums[i] = nums[j];
				break;
			}
		}
		for (; i < j; i++)
		{
			if (nums[i] > tmp)
			{
				nums[j] = nums[i];
				break;
			}
		}
	}
	nums[i] = tmp;
	if (i - head > 1)
	{
		quictsort(nums, head, i - 1);
	}
	if (tail - j > 1)
	{
		quictsort(nums, j + 1, tail);
	}
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int i = 0;
    int j = numsSize - 1;
    int *ret = malloc(sizeof(int)*2);
    quictsort(nums, 0, numsSize - 1);
    *returnSize = 2;
    while (1)
    {
        
        return ret;
    }
}