int subarraySum(int* nums, int numsSize, int k){
    int ret = 0;
    int tmp = 0;
    int sum = 0;

    for (int i = 0; i < numsSize; i++)
    {
        tmp += nums[i];
        if ((sum = tmp) == k)
        {
            ret++;
        }

        for (int j = i + 1; j < numsSize; j++)
        {
            sum -= nums[j - i - 1];
            sum += nums[j];
            if (sum == k)
            {
                ret++;
            }
        }
    }

    return ret;
}
