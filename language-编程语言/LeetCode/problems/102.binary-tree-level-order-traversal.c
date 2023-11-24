/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

typedef struct QueueLink{
    int  level;
    void *item;
    struct QueueLink *next;
}QueueLink;

QueueLink *push(QueueLink *t, int level, void *item)
{
    QueueLink *ret;
    ret = (QueueLink*)malloc(sizeof(QueueLink));
    ret->level = level;
    ret->item = item;
    ret->next = NULL;
    if (t)
    {
        t->next = ret;
    }
    return ret;
}

struct TreeNode *pop(QueueLink *h)
{
    if (h)
    {
        return (struct TreeNode*)h->item;
    }
    return NULL;
}

int get_columns(QueueLink *h, int level)
{
    int ret = 0;
    QueueLink *n = h;
    while (n && n->level == level)
    {
        n = n->next;
        ret++;
    }
    return ret;
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes){
    int col = 0;
    int level = 0;
    int max_level = 0;
    QueueLink *c = NULL;
    QueueLink *h = NULL;
    QueueLink *t = NULL;
    struct TreeNode *curr;
    int** ret = (int**)malloc(sizeof(int*)*10000);
    for (size_t i = 0; i < 100; i++)
    {
        ret[i] = NULL;
    }

    if (!root)
    {
        *returnSize = 0;
        return NULL;
    }
    

    h = t = push(t, 0, root);
    ret[0] = (int*)malloc(sizeof(int)*1);
    ret[0][0] = root->val;
    returnColumnSizes[0][0] = 1;

    while (curr = pop(h))
    {
        c = h;
        if (c->level > max_level)
        {
            col = get_columns(c, c->level);
            ret[c->level] = (int*)malloc(sizeof(int)*col);
            returnColumnSizes[0][c->level] = col;
            max_level = c->level;
            col = 0;
        }
        ret[c->level][col++] = curr->val;
        if (curr->left)
        {
            t = push(t, c->level+1, curr->left);
        }
        if (curr->right)
        {
            t = push(t, c->level+1, curr->right);
        }
        h = h->next;
        free(c);
    }

    *returnSize = max_level+1;
    return ret;
}
