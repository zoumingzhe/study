/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

#define max(a,b) (a)>(b)?(a):(b)
#define min(a,b) (a)<(b)?(a):(b)

int high(struct TreeNode* root, int h)
{
    int l, r;
    h++;

    if (!root || (!root->left && !root->right))
    {
        return h;
    }

    if (root->left)
    {
        l = high(root->left, h);
    }
    else
    {
        l = h;
    }
    
    if (root->right)
    {
        r = high(root->right, h);
    }
    else
    {
        r = h;
    }

    return max(l, r);
}

bool isBalancedNode(struct TreeNode* root)
{
    int l, r, h = 0;
    l = high(root->left, h);
    r = high(root->right, h);
    return (max(l, r) - min(l, r) < 2);
}

bool isBalanced(struct TreeNode* root){
    if (root && !isBalancedNode(root))
        return false;

    if (root->left && !isBalanced(root->left))
        return false;

    if (root->right && !isBalanced(root->right))
        return false;

    return true;
}