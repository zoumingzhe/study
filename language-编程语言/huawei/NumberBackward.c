// https://www.nowcoder.com/practice/ae809795fca34687a48b172186e3dafe?tpId=37&tqId=21234&tPage=1&rp=&ru=%2Fta%2Fhuawei&qru=%2Fta%2Fhuawei%2Fquestion-ranking
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

void tostring(long num, char *str)
{
    sprintf(str, "%lld", num);
}

int main()
{
    long num;
    char tmp[100];

    scanf("%lld", &num);
    tostring(num, tmp);
    backward(tmp);

    return 0;
}