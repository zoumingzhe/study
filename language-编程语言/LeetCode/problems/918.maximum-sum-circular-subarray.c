#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

int maxSubarraySumCircular(int* A, int ASize){
	int sum = A[0];
	int min_end_here = A[0];
	int min_sub_nums = A[0];
	int max_end_here = A[0];
	int max_sub_nums = A[0];
	for (int i = 1; i < ASize; i++)
	{
		sum += A[i];
		min_end_here = min(min_end_here+A[i], A[i]);
		min_sub_nums = min(min_sub_nums, min_end_here);
		max_end_here = max(max_end_here+A[i], A[i]);
		max_sub_nums = max(max_sub_nums, max_end_here);
	}
	int tmp = sum-min_sub_nums;
	return tmp == 0 ? max_sub_nums : max(tmp, max_sub_nums);
}
