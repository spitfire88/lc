#include <algorithm>
#include <iostream>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
    TreeNode(int v, TreeNode *left, TreeNode *right) : val(0), left(left), right(right) {}
};

class Solution {
public:
    int diameterOfBinaryTree(TreeNode *root) {
        int diameter = 0;
        calculateHeight(root, diameter);
        return diameter;
    }

private:
    int calculateHeight(TreeNode *node, int& diameter) {
        if(node == nullptr)
            return 0;

        // calculate leftHeight and rightHeight recursively
        int leftHeight = calculateHeight(node->left, diameter);
        int rightHeight = calculateHeight(node->right, diameter);

        diameter = std::max(diameter, leftHeight + rightHeight);
        return 1 + std::max(leftHeight, rightHeight);
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(5);

    Solution s;
    int diameter = s.diameterOfBinaryTree(root);
    std::cout << "Diameter of binary tree is: " << diameter << std::endl;
    return 0;
}