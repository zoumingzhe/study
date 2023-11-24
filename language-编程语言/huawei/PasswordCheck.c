// https://www.nowcoder.com/practice/184edec193864f0985ad2684fbc86841?tpId=37&tqId=21243&tPage=1&rp=&ru=%2Fta%2Fhuawei&qru=%2Fta%2Fhuawei%2Fquestion-ranking
#include <stdio.h>

int main()
{
    int  idx;
    int  off;
    int  len;
    int  sum;
    int  num[4];
    char ch;
    char password[1000];

retry:
while(gets(password))
{
    idx = 0;
    len = 0;
    sum = 0;
    num[0] = 0;
    num[1] = 0;
    num[2] = 0;
    num[3] = 0;
    while((ch = password[idx++]) != '\0')
    {
        if (ch >= 'A' && ch <= 'Z') {
            num[0]++;
        } else if (ch >= 'a' && ch <= 'z')  {
            num[1]++;
        } else if (ch >= '0' && ch <= '9') {
            num[2]++;
        } else {
            num[3]++;
        }
    }

    len = idx - 1;
    for (idx = 0; idx < 4; idx++)
    {
        if (num[idx] > 0) {
            sum++;
        }
    }

    if (len > 8 && sum >= 3) {
        for (idx = 0; idx < len-1; idx++) {
            sum = 0;
            for (off = idx+1; off < len; off++) {
                if (password[idx+sum] == password[off]) {
                    if (sum++ > 1) {
                        printf("NG\n");
                        goto retry;
                    }
                } else {
                    sum = 0;
                }
            }
        }
        printf("OK\n");
    }
    else {
        printf("NG\n");
    }
}

    return 0;
}