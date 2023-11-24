// https://www.nowcoder.com/practice/d9162298cb5a437aad722fccccaae8a7?tpId=37&tqId=21227&tPage=1&rp=&ru=/ta/huawei&qru=/ta/huawei/question-ranking
#include <stdio.h>

void stringcut(char *str)
{
    int idx = 0;
    int off = 0;
    
    while (str[idx] != '\0')
    {
        if (off < 8)
        {
            printf("%c", str[idx]);
            off++;
        }
        else
        {
            printf("\n%c", str[idx]);
            off = 1;
        }
        idx++;
    }
    
    if (off > 0)
    {
        while (off < 8)
        {
            printf("0");
            off++;
        }
        printf("\n");
    }
}

int main(void)
{
    char str[1000];
    
    while (gets(str))
    {
        stringcut(str);
    }
    return 0;
}