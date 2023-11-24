// https://www.nowcoder.com/practice/48b3cb4e3c694d9da5526e6255bb73c3?tpId=37&tqId=21236&tPage=1&rp=&ru=%2Fta%2Fhuawei&qru=%2Fta%2Fhuawei%2Fquestion-ranking
#include <stdio.h>

void sentence(char *str)
{
    int  idx = 0;

    while(str[idx] != '\0')
    {
        if (str[idx] == ' ')
        {
            str[idx] = '\0';
        }
        idx++;
    }

    idx--;
    while(idx >= 0)
    {
        if (str[idx] == '\0')
        {
            printf("%s ", str + idx + 1);
        }
        else if (idx == 0)
        {
             printf("%s ", str);
        }
        idx--;
    }
     printf("\n");
}

int main()
{
    int  idx = 0;
    char str[10000];
    
    while(gets(str))
    {
        sentence(str);
    }

    return 0;
}