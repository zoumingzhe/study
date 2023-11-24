#define max(a,b) (a) > (b) ? (a) : (b)

int rob(int* nums, int numsSize){
    int i;
    int off = 0;
    int max = nums[0];
    int sum = 0;

    for (i = 1; i < numsSize; i++)
    {
        if (nums[i] > max)
        {
            off = i;
            max = nums[i];
        }
    }

    for (i = off - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            sum += nums[i];
        }
        else if (nums[i])
        {
            
        }
    }

    for (i = off + 1; i < numsSize; i++)
    {
        /* code */
    }

    return sum+max;
}
