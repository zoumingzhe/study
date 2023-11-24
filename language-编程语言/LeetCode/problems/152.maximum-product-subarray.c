#define max(a,b) (a) > (b) ? (a) : (b)
#define min(a,b) (a) < (b) ? (a) : (b)

int maxProduct(int* nums, int numsSize){
    int tmp = 0;
    int flag = nums[0] < 0 ? 1 : 0;
    int min_end_here = nums[0];
    int max_end_here = nums[0];
    int max_sub_nums = nums[0];

    for (int i = 1; i < numsSize; i++)
    {
        tmp = nums[i];
        if (tmp == 0)
        {
            flag = 0;
            min_end_here = 0;
            max_end_here = 0;
        }
        else if (tmp < 0)
        {
            if (flag)
            {// 负数结束
                flag = 0;
                max_end_here  = max(min_end_here*tmp, max_end_here);
            }
            else
            {// 负数开始
                flag = 1;
                min_end_here  = min(max_end_here*tmp, tmp);
                max_end_here *= tmp;
            }
        }
        else if (tmp > 0)
        {
            min_end_here *= tmp;
            max_end_here  = max(max_end_here*tmp, tmp);
        }
        
        max_sub_nums = max(max_sub_nums, max_end_here);
    }

    return max_sub_nums;
}
