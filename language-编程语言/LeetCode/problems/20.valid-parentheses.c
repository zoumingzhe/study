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
    else
    {
        *val = 0;
    }
    return ret;
}

bool isValid(char * s){
    bool  ret = true;
    Stack *p = NULL;
    char  ch;

    while (*s != '\0')
    {
        switch (*s)
        {
        case '(':
        case '[':
        case '{':
            p = push(p, *s);
            break;
        case ')':
        case ']':
        case '}':
            p = pop(p, &ch);
            if ((*s == ')' && ch == '(') ||
                (*s == ']' && ch == '[') ||
                (*s == '}' && ch == '{'))
            {
                ret = true;
            }
            else
            {
                ret = false;
                goto exit;
            }
            break;
        default:
            ret = false;
            goto exit;
            break;
        }
        s++;
    }

exit:
    if (p)
    {
        while(p = pop(p, &ch));
        return false;
    }
    return ret;
}
