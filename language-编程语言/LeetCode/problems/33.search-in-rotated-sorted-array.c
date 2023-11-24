#define mid(i,j) ((i + j) / 2)
#define is_order(i,j) (i < j ? 1 : 0)
#define where(i,j,k) (((k >= i) && (k <= j))? 1 : 0)

int search(int* nums, int numsSize, int target){
    int i = 0;
    int j = numsSize - 1;
    int k;
    for (;i <= j;)
    {
        k = mid(i, j);
        if (nums[k] == target)
        {
            return k;
        }
        else if (nums[i] == target)
        {
            return i;
        }
        else if (nums[j] == target)
        {
            return j;
        }
        
        if (is_order(nums[i], nums[k]))
        {   // 前半部有序
            if (where(nums[i], nums[k], target))
            {
                j = k - 1;
            }
            else
            {
                i = k + 1;
            }
        }
        else
        {   // 后半部有序
            if (where(nums[k], nums[j], target))
            {
                i = k + 1;
            }
            else
            {
                j = k - 1;
            }
        }
    }

    return -1;
}
