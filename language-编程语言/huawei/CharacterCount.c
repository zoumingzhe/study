// https://www.nowcoder.com/practice/eb94f6a5b2ba49c6ac72d40b5ce95f50?tpId=37&tqId=21233&tPage=1&rp=&ru=/ta/huawei&qru=/ta/huawei/question-ranking
#include <stdio.h>

int main(void)
{
    char ch;
    char arry[10000];
    int count[256] = {0};
    int num = 0;
    int idx = 0;
    
    gets(arry);
    while((ch = arry[idx++]) != '\0')
    {
        count[ch]++;
    }
    
    count['\n']=0;
    for(idx=0;idx<128;idx++)
    {
        if(count[idx]>0)
        {
            num++;
        }
    }
    printf("%d", num);
    return 0;
}