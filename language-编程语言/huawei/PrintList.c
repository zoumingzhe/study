#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int   idx;
    int   val;
    struct node* prev;
    struct node* next;
}node;

int cmp_item(node* item1, node* item2)
{
    if (item1->val < item2->val)
        return -1;
    else if (item1->val > item2->val)
        return 1;
    else
        return 0;
}

node* make_item(int val, int idx)
{
    node* item = (node*)malloc(sizeof(node));
    item->val = val;
    item->idx = idx;
    item->prev = 0;
    item->next = 0;
    return item;
}

void insert_item(node* list, node* item)
{
    item->next = list->next;
    item->prev = list;
    list->next = item;
}

node* add_item(node* list, node* item)
{
    node* it;

    if (list != 0) {
        it = list;
        while (it->next) {
            it = it->next;
        }
        insert_item(it, item);
    }
    else {
        list = item;
    }
    return list;
}

node* sub_item(node* list, node** item)
{
    node* it;
    node* max = list;
    node* next;
    node* prev;

    for (it = list; it != 0; it = it->next)
    {
        if (cmp_item(max, it) < 0)
        {
            max = it;
        }
    }

    if (max == list)
    {
        *item = list;
        list = (*item)->next;
    }
    else
    {
        *item = max;
        next = max->next;
        prev = max->prev;
        prev->next = next;
        if (next)
        {
            next->prev = prev;
        }

        if (list->next)
        {
            it = list;
            while (it->next)
            {
                it = it->next;
            }
            it->next = list;
            list->prev = it;
            it = list->next;
            list->next = NULL;
            it->prev = NULL;
            list = it;
        }
    }
    return list;
}

void doo2(char* str)
{
    node* list = 0;
    node* item = 0;

    list = add_item(list, make_item(9, 0));
    list = add_item(list, make_item(2, 1));
    list = add_item(list, make_item(9, 2));
    list = add_item(list, make_item(5, 3));
    list = add_item(list, make_item(2, 4));
    list = add_item(list, make_item(2, 5));
    list = add_item(list, make_item(9, 6));
    list = add_item(list, make_item(5, 7));

    while (list)
    {
        list = sub_item(list, &item);
        printf("%d", item->idx);
        if (list)
            printf(", ");
        free(item);
    }
    printf("\n");
}

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
    *off = idx - 1;
    return val;
}

void doo(char* str)
{
    node* list = 0;
    node* item = 0;
    int val = 0;
    int num = 0;
    int idx = 0;
    int off = 0;

retry:
    val = to_num(str + idx, &off);
    list = add_item(list, make_item(val, num++));
    idx += off;
    if (str[idx] != '\0')
    {
        idx++;
        idx++;
        goto retry;
    }

    while (list)
    {
        list = sub_item(list, &item);
        printf("%d", item->idx);
        if (list)
            printf(", ");
        free(item);
    }
    printf("\n");
}

int main() {
    char str[1000];

    while (gets(str))
    {
        doo(str);
    }
    return 0;
}