#define max(a,b) (a)>(b)?(a):(b)
#define min(a,b) (a)<(b)?(a):(b)

typedef struct Stack{
    char val;
    struct Stack *prev;
}Stack;

Stack *push(Stack *s, char val)
{
    Stack *ret;
    ret = (Stack*)malloc(sizeof(Stack));
    ret->val = val;
    ret->prev = s;
    return ret;
}

Stack *pop(Stack *s, char *val)
{
    Stack *ret = NULL;
    if (s)
    {
        *val = s->val;
        ret = s->prev;
        free(s);
    }
    return ret;
}

Stack *mul(char * num, int idx)
{
    Stack *ret = NULL;
    int  carry = 0;
    int  off = 0;
    int  bit;

    while (num[off] != '\0')
    {
        bit = num[off] - '0';
        bit *= idx;
        bit += carry;
        ret = push(ret, bit % 10);
        carry = bit / 10;
        off++;
    }
    return ret;
}

void add(char * num, int len, Stack *p, int off)
{
    int  carry = 0;
    char bit;
    char val;

    while (p)
    {
        bit = num[off] - '0';
        p = pop(p, &val);
        bit += val;
        bit += carry;
        num[off] = (bit % 10) + '0';
        carry = bit / 10;
        off++;
    }

    len = max(len, off);
}

char * multiply(char * num1, char * num2){
    Stack sp[10] = { 0 };
    char tmp[220] = { '0' };
    int  carry = 0;
    int  off1 = 0;
    int  off2 = 0;
    int  len1 = 0;
    int  len2 = 0;
    int  len = 0;

    while (num1[off1++] != '\0');
    while (num2[off2++] != '\0');
    len1 = off1;
    len2 = off2;
    off1 = 0;
    off2 = 0;

    while (off1 < len1)
    {
        int idx = num1[off1] - '0';
        Stack *p = sp + idx;
        if (!p)
        {
            //乘积计算1
            p = mul(num2, idx);
        }
        //累加计算2
        add(tmp, len, p, off1);
        off1++;
    }

    char *ret = (char*)malloc(sizeof(char)*(len1+len2));
    return ret;
}
