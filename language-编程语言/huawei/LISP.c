#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char** base;
    char** top;
}stack2;

void push2(stack2* s, char* e)
{
    *(s->top) = e;
    (s->top)++;
}

char* pop2(stack2* s)
{
    char* e;
    if ((s->base) == (s->top))
        e = NULL;
    else
    {
        e = *(--(s->top));
    }
    return e;
}

char* get2topn(stack2* s, int n)
{
    return *((s->top) - n);
}

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

int gettopn(stack* s, int n)
{
    //printf("gettop %d from stack\n",*((s->top)-1));
    return *((s->top) - n);
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

extern void stringbackward(char* str);
extern int substring2(char* str1, char* str2);

void do_lisp_r(stack* strs)
{
    char* tmp = get2topn(strs, 1);
    stringbackward(tmp);
}

void do_lisp_s(stack* strs)
{
    char* tmp;
    char* tmp1 = get2topn(strs, 2);
    char* tmp2 = get2topn(strs, 1);
    int   off  = substring2(tmp1, tmp2);
    int   len  = 0;
    
    if (off >= 0)
    {
        len = strlen(tmp1) - off + 1;
        tmp = (char*)malloc(len);
        strcpy(tmp, tmp1+ off);
    }
    else
    {
        tmp = (char*)malloc(1);
        tmp[0] = '\0';
    }

    free(pop2(strs));
    free(pop2(strs));
    push2(strs, tmp);
}

void do_lisp_c(stack* strs, int n)
{
    char* tmp;
    int   idx = 0;
    int   len = 0;

    for (idx = 0; idx < n; idx++)
        len += strlen(get2topn(strs, idx + 1));
    len++;
    tmp = (char*)malloc(len);
    tmp[0] = '\0';
    for (idx = 0; idx < n; idx++)
        strcat(tmp, get2topn(strs, n - idx));
    for (idx = 0; idx < n; idx++)
        free(pop2(strs));
    push2(strs, tmp);
}

void do_lisp(stack* ops, stack* strs)
{
    int op;
    int off;
    int num = 1;

    while ((op = gettopn(ops, num)) == '"')
        num++;
    switch(op)
    {
        case 'r': do_lisp_r(strs); break;
        case 's': do_lisp_s(strs); break;
        case 'c': do_lisp_c(strs, num-1); break;    
    }
    for (off = 0; off < num+1; off++)
        pop(ops, &op);
}

void lisp(char* str)
{
    char ch;
    int len;
    int off;
    char* tmp;
    int idx = 0;
    stack ops;
    stack2 strs;

    ops.base = (int*)malloc(100 * sizeof(int));
    ops.top = ops.base;
    strs.base = (char**)malloc(100 * sizeof(char*));
    strs.top = strs.base;

    while ((ch = str[idx]) != '\0')
    {
        switch (ch)
        {
        case '(':
            push(&ops, ch);
            break;
        case ')':
            // 从栈中找到匹配的"("
            do_lisp(&ops, &strs);
            break;
        case 'q':
        case 'r':
        case 's':
        case 'c':
            push(&ops, ch);
            while (str[++idx] != ' ');
            break;
        case '"':
            push(&ops, ch);
            len = 0;
            while (str[++idx] != '"')
                len++;
            tmp = (char*)malloc(len + 1);
            tmp[len] = '\0';
            for (off = 0; off < len; off++)
            {
                tmp[off] = str[idx - len + off];
            }
            push2(&strs, tmp);
            break;
        }
        idx++;
    }

    tmp = pop2(&strs);
    printf("\"%s\"\n", tmp);
    free(tmp);
    free(ops.base);
    free(strs.base);
}

int main(void)
{
    char str1[1000];

    while (gets(str1))
    {
        lisp(str1);
    }

    return 0;
}

void stringbackward(char* str)
{
    char ch;
    int  idx = 0;
    int  len = 0;
    while (str[idx++] != '\0');
    len = idx - 2;
    for (idx = 0; idx < len; )
    {
        ch = str[idx];
        str[idx] = str[len];
        str[len] = ch;
        idx++;
        len--;
    }
}

int substring2(char* str1, char* str2)
{
    int idx1 = 0;
    int idx2 = 0;

    while (str1[idx1] != '\0')
    {
        if (str1[idx1] == str2[idx2])
        {
            idx2++;
        }
        else
        {
            idx1 -= idx2;
            idx2 = 0;
        }

        if (str2[idx2] == '\0')
        {
            return (idx1 - idx2 + 1);
        }
        idx1++;
    }
    return -1;
}