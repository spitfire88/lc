#include <vector>
#include <iostream>
#include <queue>
using namespace std;

struct Tnode {
    int val;
    Tnode *left;
    Tnode *right;
    Tnode() : val(0), left(nullptr), right(nullptr) {}
    Tnode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class BFS {
private:
    vector<vector<int>> levels;

public:
    void helper(Tnode *root, int level) {
        if(levels.size() == level) levels.push_back(vector<int>());
        levels[level].push_back(root->val);
        if(root->left != nullptr) helper(root->left, level + 1);
        if(root->right != nullptr) helper(root->right, level + 1);
    }

    vector<vector<int>> levelOrder(Tnode *root) {
        if(root == nullptr) return levels;

        helper(root, 0); // pass the level
        return levels;
    }

    vector<vector<int>> levelOrderIterative(Tnode *root) {
        if(root == nullptr) return levels;
        deque<Tnode *> queue;
        queue.push_back(root);
        int level = 0;

        while(!queue.empty()) {
            levels.push_back(vector<int>());
            int level_length = queue.size();
            for(int i = 0; i < level_length; ++i) {
                Tnode *node = queue.front();
                queue.pop_front();
                levels[level].push_back(node->val);
                if(node->left != nullptr) queue.push_back(node->left);
                if(node->right != nullptr) queue.push_back(node->right);
            }
            level++;
        }
        return levels;
    }
};

int main() {
    Tnode *root = new Tnode(0);
    root->left = new Tnode(1);
    root->right = new Tnode(2);
    root->left->left = new Tnode(3);
    root->left->right = new Tnode(4);
    root->right->left = new Tnode(5);
    root->right->right = new Tnode(6);

    BFS b;
    vector<vector<int>> result = b.levelOrder(root);

    for(auto i : result) {
        for(auto j : i) {
            cout << j << " ";
        }
        cout << endl;
    }
    return 0;
}