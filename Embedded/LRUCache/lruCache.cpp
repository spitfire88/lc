/*
    1. add key/value pair at the end of the list
    2. if key is present return value and move the key/value to the end of the list
    3. if the cache is full remove from the front of the list
    unordered_map cache; struct node to maintain head (Least recently used) and tail (most recently used)
    4. Does it need to be thread safe for multithreaded usecase?
    5. any performance optimizations?
*/
#include <unordered_map>
#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>

class Node {
public:
    int key;
    int value;
    Node *prev;
    Node *next;

    Node(int k, int v) : key(k), value(v) {
        prev = nullptr;
        next = nullptr;
    }
};

class lruCache {
private:
    int capacity;
    // reader-writer lock
    mutable std::shared_mutex rw_lock;
    std::unordered_map<int, Node *> cache;
    Node *head;
    Node *tail;

    Node *remove_in_place(Node *node) {
        Node *next = node->next;
        Node *prev = node->prev;
        prev->next = next;
        next->prev = prev;
        return node;
    }

    void insert_at_end(Node *node) {
        Node *next = tail;
        Node *prev = tail->prev;
        prev->next = node;
        node->prev = prev;
        node->next = next;
        next->prev = node;
    }

public:
    lruCache(int cap) : capacity(cap) {
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        // Shared lock for readers
        std::shared_lock<std::shared_mutex> lock(rw_lock); 
        if(cache.find(key) != cache.end()) {
            Node *node = cache[key];
            // Unlock shared lock before modifying
            lock.unlock();
            // Upgrade to exclusive lock
            std::unique_lock<std::shared_mutex> write_lock(rw_lock);
            remove_in_place(node);
            insert_at_end(node);
            return node->value;
        }
        return -1;
    }

    void put(int key, int value) {
        // Exclusive lock for writers
        std::unique_lock<std::shared_mutex> lock(rw_lock);
        if(cache.find(key) != cache.end()) {
            std::cout << "key already present! \n";
            return;
        }
        Node *node = new Node(key, value);
        cache[key] = node;
        insert_at_end(node);
        if(cache.size() > capacity) {
            Node *node = remove_in_place(head->next);
            cache.erase(node->key);
            delete node;
        }
    }

    void dump() {
        std::shared_lock<std::shared_mutex> lock(rw_lock);
        Node *curr = head->next;
        while (curr != nullptr && curr != tail) {
            std::cout << "[" << curr->key << " : " << curr->value << "] ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }

    ~lruCache() {
        Node *curr = head;
        while (curr != nullptr) {
            Node *next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

void test_put(lruCache &cache, int key, int value) {
    cache.put(key, value);
    std::cout << "Put key: " << key << ": " << value << std::endl;
    cache.dump();
}

void test_get(lruCache &cache, int key) {
    int value = cache.get(key);
    std::cout << "Get key: " << key << ": " << value << std::endl;
    cache.dump();
}

void test_dump(lruCache &cache) {
    cache.dump();
}

int main() {
    lruCache l1(3);
    
    // Create threads for testing
    std::vector<std::thread> threads;

    // Add threads for put/get operations
    threads.emplace_back(test_put, std::ref(l1), 1, 10);
    threads.emplace_back(test_get,std::ref(l1), 1);
    threads.emplace_back(test_put, std::ref(l1), 2, 20);
    threads.emplace_back(test_put, std::ref(l1), 3, 30);
    threads.emplace_back(test_get,std::ref(l1), 1);
    threads.emplace_back(test_get,std::ref(l1), 2);
    threads.emplace_back(test_get,std::ref(l1), 3);
    threads.emplace_back(test_dump, std::ref(l1));
    threads.emplace_back(test_put, std::ref(l1), 4, 40);
    threads.emplace_back(test_put, std::ref(l1), 5, 50);
    threads.emplace_back(test_get,std::ref(l1), 1);
    

    for (auto &thread : threads) {
        thread.join();
    }

    l1.dump();

    return 0;
}