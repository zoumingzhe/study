// https://www.nowcoder.com/practice/9a763ed59c7243bd8ab706b2da52b7fd?tpId=37&tqId=21248&tPage=2&rp=&ru=%2Fta%2Fhuawei&qru=%2Fta%2Fhuawei%2Fquestion-ranking
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char  typ;
    int   idx;
    int   val1;
    char* val2;
    struct node* prev;
    struct node* next;
    int   mnum;
    struct node2* mlist;
}node;

typedef struct {
    node* item;
    struct node2* next;
}node2;

int cmp_item(node* item1, node* item2)
{
    if (item1->val1 < item2->val1)
        return -1;
    else if (item1->val1 > item2->val1)
        return 1;
    else
        return 0;
}

int cmp_item2(node* item1, node* item2)
{
    return strcmp(item1->val2, item2->val2);
}

node* make_item(char typ, int val, char *str, int idx)
{
    node* item = (node*)malloc(sizeof(node));
    item->typ = typ;
    item->val1 = val;
    item->val2 = str;
    item->idx = idx;
    item->prev = 0;
    item->next = 0;
    item->mnum = 0;
    item->mlist = 0;
    return item;
}

node2* make_item2(node* val)
{
    node2* item = (node2*)malloc(sizeof(node2));
    item->item = val;
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
        if (cmp_item(item, list) < 0) {
            item->next = list;
            list = item;
        }
        else {
            for (it = list; it != 0; it = it->next) {
                if (it->next == 0 ||
                   (cmp_item(item, it) >= 0 &&
                    cmp_item(item, it->next) < 0))
                {
                    insert_item(it, item);
                    break;
                }
            }
        }
    }
    else {
        list = item;
    }
    return list;
}

node* add_item2(node* list, node* item)
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

node* add_item_u(node* list, node* item)
{
    node* it;

    if (list != 0) {
        if (cmp_item(item, list) < 0) {
            item->next = list;
            list = item;
        }
        else {
            for (it = list; it != 0; it = it->next) {
                if (cmp_item(item, it) == 0) {
                    break;
                } else if (it->next == 0 ||
                   (cmp_item(item, it) > 0 &&
                    cmp_item(item, it->next) < 0))
                {
                    insert_item(it, item);
                    break;
                }
            }
        }
    }
    else {
        list = item;
    }
    return list;
}

node* get_list2(char typ, char* str)
{
    int off = 0;
    int idx = 0;
    int num = 0;
    unsigned int val = 0;
    char* temp = NULL;
    node* list = 0;
    node* item;

    while (str[off] != ' ') {
        num *= 10;
        num += str[off] - '0';
        off++;
    }
    off++;

    while (str[off] != '\0') {
        if (str[off] == ' ') {
            temp = (char*)malloc(100);
            sprintf(temp, "%d", val);
            item = make_item(typ, val, temp, idx++);
            list = add_item2(list, item);
            val = 0;
        }
        else {
            val *= 10;
            val += str[off] - '0';
        }
        off++;
    }
    temp = (char*)malloc(100);
    sprintf(temp, "%d", val);
    item = make_item(typ, val, temp, idx++);
    list = add_item2(list, item);
    return list;
}

node* get_list_u(char typ, char* str)
{
    int off = 0;
    int idx = 0;
    int num = 0;
    unsigned int val = 0;
    char* temp = NULL;
    node* list = 0;
    node* item;

    while (str[off] != ' ') {
        num *= 10;
        num += str[off] - '0';
        off++;
    }
    off++;

    while (str[off] != '\0') {
        if (str[off] == ' ') {
            temp = (char*)malloc(100);
            sprintf(temp, "%d", val);
            item = make_item(typ, val, temp, idx++);
            list = add_item_u(list, item);
            val = 0;
        }
        else {
            val *= 10;
            val += str[off] - '0';
        }
        off++;
    }
    temp = (char*)malloc(100);
    sprintf(temp, "%d", val);
    item = make_item(typ, val, temp, idx++);
    list = add_item_u(list, item);
    return list;
}

extern int substring(char *str1, char *str2);

int matching(node* list, node* item)
{
    int ret = 0;
    node2* listh = 0;
    node2* list1 = 0;
    node2* list2 = 0;
    node* it;
    
    if (item->val1 == 0)
    {
        item->mnum = 0;
        item->mlist = 0;
        return 0;
    }

    for (it = list; it != 0; it = it->next)
    {
        if (substring(it->val2, item->val2))
        {
            list2 = make_item2(it);
            if (!list1)
            {
                list1 = list2;
                listh = list1;
            }
            else
            {
                list1->next = list2;
                list1 =list2;
            }
            
            ret++;
        }
    }
    item->mnum = ret;
    item->mlist = listh;
    return ret;
}

int main(void)
{
    int   sum = 0;
    node* item;
    node* listI;
    node* listR;
    node2* item2;
    char strI[1000];
    char strR[1000];

    gets(strI);
retry:
    gets(strR);

    listI = get_list2(1, strI);
    listR = get_list_u(2, strR);

    for (item = listR; item != 0; item = item->next)
    {
        if (matching(listI, item) > 0)
        {
            sum += ((item->mnum + 1) * 2);
        }
    }

    if (sum > 0)
    {
        printf("%d ", sum);
        for (item = listR; item != 0; item = item->next)
        {
            if (item->mnum > 0)
            {
                printf("%s %d ", item->val2, item->mnum);
                item2 = item->mlist;
                while (item2)
                {
                    printf("%d %s ", item2->item->idx, item2->item->val2);
                    item2 = item2->next;
                }
            }
        }
    }

    if (gets(strI))
    {
        sum = 0;
        printf("\n");
        goto retry;
    }

    // for (item = listI; item != 0; item = item->next)
    // {
    //     printf("%d-%s ", item->idx, item->val2);
    // }

    // for (item = listR; item != 0; item = item->next)
    // {
    //     printf("%d-%s ", item->idx, item->val2);
    // }

    return 0;
}


int substring(char *str1, char *str2)
{
    int idx1 = 0;
    int idx2 = 0;

    while(str1[idx1] != '\0')
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
            return 1;
        }
        idx1++;
    }
    return 0;
}