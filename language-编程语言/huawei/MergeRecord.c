// https://www.nowcoder.com/practice/de044e89123f4a7482bd2b214a685201?tpId=37&tqId=21231&tPage=1&rp=&ru=%2Fta%2Fhuawei&qru=%2Fta%2Fhuawei%2Fquestion-ranking
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int   key;
    int   val;
    struct node* next;
}node;

int main(void)
{
    int num;
    int key;
    int val;
    
    node* list = NULL;
    node* item = NULL;
    node* prev = NULL;
    node* index = NULL;
    
    scanf("%d", &num);
    while(num--)
    {
        scanf("%d %d", &key, &val);
       
        if (list != NULL)
        {
            for (index = list; index != NULL; index = index->next)
            {
                if (index->key == key)
                {
                    index->val += val;
                    break;
                }
            }
            
            if (!index)
            {
                prev = NULL;
                item = (node*)malloc(sizeof(node));
                item->key = key;
                item->val = val;
                item->next = NULL;
            for (index = list; index != NULL; index = index->next)
            {
                if (index->key > key)
                {
                    item->next = index;
                    if (prev)
                    {
                        prev->next = item;
                    }
                    else
                    {
                        list = item;
                    }
                    break;
                }
                prev = index;
            }
                if (!index)
                {
                    prev->next = item;
                }
            }
        }
        else
        {
            item = (node*)malloc(sizeof(node));
            item->key = key;
            item->val = val;
            item->next = NULL;
            list = item;
        }
    }
    
    for (index = list; index != NULL; index = index->next)
    {
        printf("%d %d \n", index->key, index->val);
    }
    
    return 0;
}