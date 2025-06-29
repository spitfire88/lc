#include <unordered_map>
#include <iostream>

/*
    1. Store a key/val pair at the end of the list
    2. Update a key/val pair 
    3. Given a key, if key does not exist return -1, if it does, return the value
    4. When key/val pair is added, create a node and add it to the back of the list
    5. When key/val pair is accessed, move it to the back of the list
    6. When a new key/val pair is added but the list is at capacity, remove the first node and add the new node to the end of the list

    1-3 can be achieved using a hashmap
    4-6 needs a doubly linked list to move nodes around in constant time
*/

class Node {
public:
    int key;
    int value;
    Node* next;
    Node* prev;

    Node(int k, int v) : key(k), value(v), next(nullptr), prev(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    std::unordered_map<int, Node *> map;
    Node *head; // least used
    Node *tail; // most used

    void remove(Node *node) {
        Node *prev = node->prev;
        Node *next = node->next;
        prev->next = next;
        next->prev = prev;
    }

    void insert(Node *node) {
        Node *prev = tail->prev;
        Node *next = tail;
        prev->next = next->prev = node;
        node->next = next;
        node->prev = prev;
    }

public:
    LRUCache(int cap) {
        capacity = cap;
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (map.find(key) != map.end()) { // found the key
            Node *node = map[key];
            remove(node);
            insert(node);
            return node->value;
        }
        return -1; // no key found
    }

    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            remove(map[key]); // removes it since its not at the end of the list
        }
        Node *newNode = new Node(key, value);
        map[key] = newNode;
        insert(newNode); // adds it to the end of the list

        if (map.size() > capacity) {
            Node* lru = head->next;
            remove(lru);
            map.erase(lru->key);
            delete(lru);
        }
    }

    void dump() {
        Node *curr = head->next;
        while (curr != nullptr && curr != tail) {
            std::cout << "[" << curr->key << " : " << curr->value << "] ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }

    ~LRUCache() {
        Node *curr = head;
        while (curr != nullptr) {
            Node *next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

int main() {
    LRUCache lru(3);

    lru.put(1, 10);
    lru.put(2, 20);
    lru.put(3, 30);
    lru.dump();

    lru.get(1);
    lru.dump();

    lru.put(4, 40);
    lru.dump();

    return 0;
}