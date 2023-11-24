/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

typedef struct StackLink{
    void *item;
    struct StackLink *prev;
}StackLink;

StackLink *push(StackLink *s, void *item)
{
    StackLink *ret;
    ret = (StackLink*)malloc(sizeof(StackLink));
    ret->item = item;
    ret->prev = s;
    return ret;
}

StackLink *pop(StackLink *s)
{
	StackLink *ret = NULL;
	if (s)
	{
		ret = s->prev;
		free(s);
	}
	return ret;
}

void *get_top(StackLink *s)
{
	return s ? s->item : NULL;
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    StackLink *stack1 = NULL;
    StackLink *stack2 = NULL;
    struct ListNode* item1 = NULL;
    struct ListNode* item2 = NULL;
    struct ListNode* tmp = NULL;
    struct ListNode* ret = NULL;
    int       add = 0;

    for (item1 = l1; item1 != NULL ; item1 = item1->next)
    {
        stack1 = push(stack1, item1);
    }

    for (item2 = l2; item2 != NULL ; item2 = item2->next)
    {
        stack2 = push(stack2, item2);
    }

    // 相加
    while (1)
    {
        item1 = (struct ListNode*)get_top(stack1);
        item2 = (struct ListNode*)get_top(stack2);
        stack1 = pop(stack1);
        stack2 = pop(stack2);

        if (item1 != NULL && item2 != NULL)
        {
            add += (item1->val + item2->val);
        }
        else if (item1 != NULL)
        {
            add += (item1->val);
        }
        else if (item2 != NULL)
        {
            add += (item2->val);
        }
        else
        {
            if (add > 0)
            {
                tmp = (struct ListNode*)malloc(sizeof(struct ListNode));
                tmp->val = add;
                tmp->next = ret;
                ret = tmp;
            }
            return ret;
        }
        
        tmp = (struct ListNode*)malloc(sizeof(struct ListNode));
        tmp->val = add % 10;
        tmp->next = ret;
        ret = tmp;
        add /= 10;
    }
}
