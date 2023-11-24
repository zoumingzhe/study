// 本题为考试多行输入输出规范示例，无需提交，不计分。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int to_num(char* str, int* off)
{
    char ch;
    int val = 0;
    int idx = 0;

again:
    ch = str[idx++] - '0';
    if (ch >= 0 && ch <= 9)
    {
        val *= 10;
        val += ch;
        goto again;
    }
    *off = idx-1;
    return val;
}

int dos(char* str, int n)
{
    int ret = 0;
    int val = 0;
    int idx = 0;
    int off = 0;

retry:
    val = to_num(str + idx, &off);
    ret += val;
    idx += off;
    if (str[idx] != '\0')
    {
        idx++;
        goto retry;
    }

    //printf("%d\n", ret);
    return ret;
}

int main() {
    int n, ans = 0;
    char str[1000];

    while (gets(str))
    {
        n = to_num(str, &ans);
        ans = 0;
        //printf("%d\n", n);
        //scanf("%d\n", &n);
        for (int i = 0; i < n; i++) {
            gets(str);
            ans += dos(str, n);
        }
        printf("%d\n", ans);
    }
    return 0;
}