/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* dailyTemperatures(int* T, int TSize, int* returnSize){
    int* ret = (int*)malloc(sizeof(int)*TSize);
    int* tmp = (int*)malloc(sizeof(int)*TSize);
    int  len = 0;
    int  day = 0;

    for (int i = 0; i < TSize; i++)
    {
        ret[i] = 0;
        for (int j = len - 1; j >= 0; j--)
        {
            day = tmp[j];
            if (T[i] > T[day])
            {
                ret[day] = i - day;
                len--;
            }
            else
            {
                break;
            }
        }
        tmp[len++] = i;
    }

    *returnSize = TSize;
    return ret;
}
