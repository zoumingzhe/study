int reverse(int x){
    int ret = 0;
    int tmp  = 0;
    int len1 = 0;
    int len2 = 0;
    int nums[100] = { 0 };

    if (0x80000000 == (unsigned int)x) return 0;

    tmp = x > 0 ? x : -x;
    while (tmp > 0)
    {
        nums[len1++] = tmp % 10;
        tmp /= 10;
    }

    while (len2 < len1)
    {
        ret *= 10;
        ret += nums[len2++];
        if (len2 == 9 && len1 >= 10 && ret > 0x7fffffff/10)
        {
            return 0;
        }
    }

    return (x > 0 ? ret : -ret);
}
