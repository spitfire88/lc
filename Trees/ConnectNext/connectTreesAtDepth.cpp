#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode *next;

    TreeNode(int _v) : val(_v), left(nullptr), right(nullptr), next(nullptr) {}
};

void connectBFS(TreeNode* root) {
    if (!root) return;
    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        TreeNode* prev = nullptr;
        for (int i = 0; i < size; ++i) {
            TreeNode* node = q.front(); q.pop();
            if (prev) prev->next = node;
            prev = node;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        prev->next = nullptr; // Last node in the level
    }
}

TreeNode* getNext(TreeNode* node) {
    while (node) {
        if (node->left) return node->left;
        if (node->right) return node->right;
        node = node->next;
    }
    return nullptr;
}

void connectDFS(TreeNode* root) {
    if (!root) return;
    if (root->left) {
        if (root->right)
            root->left->next = root->right;
        else
            root->left->next = getNext(root->next);
    }
    if (root->right) {
        root->right->next = getNext(root->next);
    }
    // Connect right child first to ensure next pointers are set up
    connectDFS(root->right);
    connectDFS(root->left);
}

int main() {
    TreeNode *root = new TreeNode(0);
    root->left = new TreeNode(1);
    root->right = new TreeNode(2);
    root->left->left = new TreeNode(3);
    //root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(5);
    root->right->right = new TreeNode(6);

    connectDFS(root);

    return 0;
}