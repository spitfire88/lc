#include <iostream>
#include <stack>

using namespace std;

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
    TreeNode(int v, struct TreeNode *l, struct TreeNode *r) : val(v), left(l), right(r) {}
};

class dfs {
public:
    int searchRecursive(TreeNode *root, int val) {
        if(!root) return 0;
        if(root->val == val) return 1;
        int foundLeft = searchRecursive(root->left, val);
        if (foundLeft) return 1;
        return searchRecursive(root->right, val);
    }

    int searchIterative(TreeNode *root, int val) {
        if(!root) return 0;
        stack<TreeNode *> stack;
        stack.push(root);

        while(!stack.empty()) {
            TreeNode *current = stack.top();
            stack.pop();

            if(current->val == val) return 1;
            
            if (current->right) stack.push(current->right);
            if (current->left) stack.push(current->left);
        }
        return 0;
    }
};

int main() {
    TreeNode *root = new TreeNode(0);
    root->left = new TreeNode(1);
    root->right = new TreeNode(2);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(5);

    dfs s;
    s.searchIterative(root, 5);
    s.searchRecursive(root, 3);

    return 0;
}