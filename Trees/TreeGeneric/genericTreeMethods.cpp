#include <iostream>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(-1), left(nullptr), right(nullptr) {}
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
    TreeNode(int v, TreeNode *l, TreeNode *r) : val(v), left(l), right(r) {}
};

class DFS {
public:
    int searchRecursive(TreeNode *root, int val) {
        if(!root) 
            return -1;

        if(root->val == val) 
            return 0;

        int foundLeft = searchRecursive(root->left, val);
        if (foundLeft == 0) return 0;
        return searchRecursive(root->right, val);
    }

    int depthRecursive(TreeNode *root) {
        if(!root) 
            return 0;

        int leftDepth = depthRecursive(root->left);
        int rightDepth = depthRecursive(root->right);

        return max(leftDepth, rightDepth) + 1;
    }

    int calculateDiameter(TreeNode *root, int& diameter) {
        if(!root)
            return 0;

        int leftDepth = calculateDiameter(root->left, diameter);
        int rightDepth = calculateDiameter(root->right, diameter);

        diameter = max(diameter, leftDepth + rightDepth);
        return max(leftDepth, rightDepth) + 1;
    }

    int diameterRecursive(TreeNode *root) {
        int diameter = 0;
        calculateDiameter(root, diameter);
        return diameter;
    }
};

int main() {
    TreeNode *tree = new TreeNode(8);
    tree->left = new TreeNode(6);
    tree->right = new TreeNode(7);
    tree->left->left = new TreeNode(1);
    tree->left->right = new TreeNode(2);
    tree->right->left = new TreeNode(4);
    tree->right->right = new TreeNode(5);
    
    DFS d;
    cout << "is 2 present in tree? " << (d.searchRecursive(tree, 2) == 0 ? true : false) << endl;
    cout << "Depth of the tree is: " << d.depthRecursive(tree) << endl;
    cout << "Diameter of the tree is: " << d.diameterRecursive(tree) << endl;
    return 0;
}