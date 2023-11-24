#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

int kadance_min(int* nums, int numsSize, int* sum) {
	sum = nums[0];
	int min_end_here = nums[0];
	int min_sub_nums = nums[0];
	for (int i = 1; i < numsSize; i++)
	{
		sum += nums[i];
		min_end_here = min(min_end_here+nums[i], nums[i]);
		min_sub_nums = min(min_sub_nums, min_end_here);
	}
	return min_sub_nums;
}

int kadance_max(int* nums, int numsSize, int* sum) {
	sum = nums[0];
	int max_end_here = nums[0];
	int max_sub_nums = nums[0];
	for (int i = 1; i < numsSize; i++)
	{
		sum += nums[i];
		max_end_here = max(max_end_here+nums[i], nums[i]);
		max_sub_nums = max(max_sub_nums, max_end_here);
	}
	return max_sub_nums;
}

int kadance_loop(int* nums, int numsSize) {
	int sum = nums[0];
	int min_end_here = nums[0];
	int min_sub_nums = nums[0];
	int max_end_here = nums[0];
	int max_sub_nums = nums[0];
	for (int i = 1; i < numsSize; i++)
	{
		sum += nums[i];
		min_end_here = min(min_end_here+nums[i], nums[i]);
		min_sub_nums = min(min_sub_nums, min_end_here);
		max_end_here = max(max_end_here+nums[i], nums[i]);
		max_sub_nums = max(max_sub_nums, max_end_here);
	}
	int tmp = sum-min_sub_nums;
	return tmp == 0 ? max_sub_nums : max(tmp, max_sub_nums);
}
