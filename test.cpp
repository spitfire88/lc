#include<vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode *next;

    TreeNode(int _v) : val(_v), left(nullptr), right(nullptr), next(nullptr) {}
};

vector<int, TreeNode *> map;

int depthFirstLinkNodes(TreeNode *root) {
    if(root == nullptr) {
        return 0;
    }

    int leftDepth = depthFirstLinkNodes(root->left);
    if(leftDepth > 0) 
        map.emplace(leftDepth, root->left);
    int rightDepth = depthFirstLinkNodes(root->right);
    if(rightDepth > 0) 
        map.emplace(rightDepth, root->right);
    return max(leftDepth, rightDepth) + 1;
}

int main() {
    TreeNode *t = new TreeNode(0);
    t->left = new TreeNode(1);
    t->right = new TreeNode(2);
    t->left->left = new TreeNode(3);
    t->left->right = new TreeNode(4);
    t->right->left = new TreeNode(5);
    t->right->right = new TreeNode(6);

    depthFirstLinkNodes(t);

    return 0;
}