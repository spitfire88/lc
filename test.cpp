// LRU

/*
    1. add key/value pair at the end of the list
    2. if key is present return value and move the key/value to the end of the list
    3. if the cache is full remove from the front of the list
    unordered_map cache; struct node to maintain head (Least recently used) and tail (most recently used)
    4. Does it need to be thread safe for multithreaded usecase?
    5. any performance optimizations?
*/

// struct Node {
//      int key;
//      int val; 
//      Node *next;
//      Node *prev;
// }
// 
// unordered_map<int, Node*> lookup is O(1)
// collision? update the val? if not have TreeNode val
// LRU get, remove and move it to the front
// LRU put, move it to the front

#include <unordered_map>
using namespace std;

struct Node {
public:
    int key;
    int val;
    Node *prev;
    Node *next;

    Node(int _k, int _v) : key(_k), val(_v), prev(nullptr), next(nullptr) {}
};

class LRU {
private:
    unordered_map<int, Node *> cache;
    Node *head; //LRU
    Node *tail; //MRU
    int capacity;

    void insertMRU(Node *temp) {
        Node *prev = nullptr;
        Node *curr = tail;
        if(curr != nullptr) {
            temp->next = curr;
            curr->prev = temp;
            prev = temp->prev;
            prev->next = temp;
        } else { // no head
            head = temp;
        }
    }

    void deleteNode(Node *temp) {
        Node *prev = temp->prev;
        Node *next = temp->next;
        prev->next = next;
        next->prev = prev;
    }


public:
    LRU(int _cap) : capacity(_cap) {
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if(cache.find(key) != cache.end()) {
            Node *temp = cache[key];
            deleteNode(temp);
            insertMRU(temp);
            return temp->val;
        }
        return -1;
    }

    int put(int key, int val) {
        if(cache.find(key) != cache.end()) {
            Node *temp = cache[key];
            temp->val = val;
            deleteNode(temp);
            insertMRU(temp);
            return 0;
        }
        Node *temp = new Node(key, val);
        insertMRU(temp);
        cache.emplace(key, temp);

        if(cache.size() > capacity) {
            deleteNode(tail->prev);
        }
    }
};