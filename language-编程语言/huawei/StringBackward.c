// https://www.nowcoder.com/practice/e45e078701ab4e4cb49393ae30f1bb04?tpId=37&tqId=21235&tPage=1&rp=&ru=/ta/huawei&qru=/ta/huawei/question-ranking
#include <stdio.h>

void backward(char *str)
{
    int  idx = 0;
    while(str[idx++] != '\0');
    idx--;
    idx--;
    while(idx >= 0)
    {
        printf("%c", str[idx--]);
    }
    printf("\n");
}

int main()
{
    char str[10000];
    
    while(gets(str))
    {
        backward(str);
    }

    return 0;
}