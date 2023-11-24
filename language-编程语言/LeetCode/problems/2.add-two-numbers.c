/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    int a = 0;
    int b = 0;
    struct ListNode* l = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* ret = l;

    if (!l1 && !l2)
    {
        return NULL;
    }

    b = (l1 ? l1->val : 0) + (l2 ? l2->val : 0);
    l1 = l1 ? l1->next : NULL;
    l2 = l2 ? l2->next : NULL;
    l->val = b % 10;
    l->next = NULL;
    a = b / 10;

    while (l1 || l2)
    {
        b = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + a;
        l1 = l1 ? l1->next : NULL;
        l2 = l2 ? l2->next : NULL;
        l->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        l = l->next;
        l->val = b % 10;
        l->next = NULL;
        a = b / 10;
    }

    if (a > 0)
    {
        l->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        l = l->next;
        l->val = a;
        l->next = NULL;
    }

    return ret;
}
