// https://www.nowcoder.com/practice/196534628ca6490ebce2e336b47b3607?tpId=37&tqId=21229&tPage=1&rp=&ru=/ta/huawei&qru=/ta/huawei/question-ranking
#include <stdio.h>

int next_prime(int now)
{
    int i;
    
    while (1)
    {
        now++;
        for (i = 2; i*i <= now; i++)
        {
            if ((now % i) == 0)
                break;
        }

        if (i*i > now)
        {
           return now;
        }
    }
}

int main(void)
{
    int  input;
    int  output = 2;
    
    scanf("%d", &input);
    while (input > 1)
    {
        while (((input % output) == 0))
        {
            printf("%d ", output);
            input /= output;
        }
        
        // 下一个质数
        output = next_prime(output);
    }

    return 0;
}

int main2(void)
{
    int now = 1;
    while (now < 100)
    {
        now = next_prime(now);
        printf("%d ", now);
    }
    return 0;
}