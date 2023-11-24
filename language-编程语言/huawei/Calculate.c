// https://www.nowcoder.com/practice/9999764a61484d819056f807d2a91f1e?tpId=37&tqId=21273&tPage=3&rp=&ru=/ta/huawei&qru=/ta/huawei/question-ranking
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int* base;
    int* top;
}stack;

void push(stack* s, int e)
{
    *(s->top) = e;
    (s->top)++;
    //printf("push %d into stack\n",e);
}

void pop(stack* s, int* e)
{
    if ((s->base) == (s->top))
        e = NULL;
    else
    {
        *e = *(--(s->top));
    }
    // printf("pop %d from stack\n",*e);
}

int gettop(stack* s)
{
    //printf("gettop %d from stack\n",*((s->top)-1));
    return *((s->top) - 1);
}

//1:is empty 0:no empty
int isempty(stack* s)
{
    return ((s->base) == (s->top));
}

int elemnum(stack* s)
{
    return (abs((s->base) - (s->top)));
}

//c2>c1?
int ishigh(char c1, char c2)
{
    if (c1 == '+' || c1 == '-')
    {
        if (c2 == '+' || c2 == '-')
            return 0;
        else if (c2 == '*' || c2 == '/')
            return 1;
    }
    if (c1 == '*' || c1 == '-')
        return 0;
    if (c1 == '(')
        return 1;
}

int do_calc(int a, int b, int ch)
{
    switch (ch)
    {
    case '+': return (a + b);
    case '-': return (a - b);
    case '*': return (a * b);
    case '/': return (a / b);
    }
}

void calc_low(stack *nums, stack *calcs)
{
    int c;
    int n1, n2;

    while (isempty(calcs) == 0)
    {
        pop(calcs, &c);
        if (c == '(')
            return;
        pop(nums, &n2);
        pop(nums, &n1);
        if (isempty(calcs) == 0 && gettop(calcs) == '-')
        {
            if (c == '+')
            {
                n1 = do_calc(n1, n2, '-');
            }
            else
            {
                n1 = do_calc(n1, n2, '+');
            }
        }
        else
        {
            n1 = do_calc(n1, n2, c);
        }
        push(nums, n1);
    }
}

void calc_high(stack *nums, stack *calcs)
{
    int c;
    int n1, n2;

retryh:
    c = gettop(calcs);
    if (c == '*' || c == '/')
    {
        pop(calcs, &c);
        pop(nums, &n2);
        pop(nums, &n1);
        n1 = do_calc(n1, n2, c);
        push(nums, n1);
        goto retryh;
    }
}

int calc(char* str)
{
    char ch;
    int idx = 0;
    int val = 0;
    int c;
    int n1, n2;
    stack nums, calcs;
    nums.base = (int*)malloc(100 * sizeof(int));
    calcs.base = (int*)malloc(100 * sizeof(int));
    nums.top = nums.base;
    calcs.top = calcs.base;

    while ((ch = str[idx]) != '\0')
    {
        switch (ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // 读取一个数字
            while (str[idx] >= '0'
                && str[idx] <= '9')
            {
                val *= 10;
                val += (str[idx] - '0');
                idx++;
            }
            push(&nums, val);
            val = 0;
            idx--;
            // 计算乘除法
            calc_high(&nums, &calcs);
            break;
        case '{':
        case '[':
        case '(':
            // 入运算栈
            push(&calcs, '(');
            break;
        case '}':
        case ']':
        case ')':
            // 计算加减法
            calc_low(&nums, &calcs);
            // 计算乘除法
            calc_high(&nums, &calcs);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            push(&calcs, ch);
            break;
        }
        idx++;
    }

    // 计算加减法
    calc_low(&nums, &calcs);

    pop(&nums, &n1);
    free(nums.base);
    free(calcs.base);
    return n1;
}

int main(void)
{
    char str[1000];

    while (gets(str))
    {
        printf("%d\n", calc(str));
    }

    return 0;
}