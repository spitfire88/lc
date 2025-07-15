#include "../include/myLRUCache.hpp"
#include <iostream>

template<typename Key, typename Value>
MyLRUCache<Key, Value>::MyLRUCache(size_t cap) : capacity(cap) {
    // Create dummy head and tail nodes
    head = std::make_shared<Node>(Key{}, Value{});
    tail = std::make_shared<Node>(Key{}, Value{});
    head->next = tail;
    tail->prev = head;
}

template<typename Key, typename Value>
void MyLRUCache<Key, Value>::removeNode(NodePtr node) {
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
}

template<typename Key, typename Value>
void MyLRUCache<Key, Value>::insertAtEnd(NodePtr node) {
    NodePtr prev_node = tail->prev;
    prev_node->next = node;
    node->prev = prev_node;
    node->next = tail;
    tail->prev = node;
}

template<typename Key, typename Value>
void MyLRUCache<Key, Value>::moveToEnd(NodePtr node) {
    removeNode(node);
    insertAtEnd(node);
}

template<typename Key, typename Value>
Value MyLRUCache<Key, Value>::get(const Key& key) {
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    
    auto it = cache.find(key);
    if (it != cache.end()) {
        NodePtr node = it->second;
        lock.unlock();
        
        // Upgrade to exclusive lock for modification
        std::unique_lock<std::shared_mutex> write_lock(rw_lock);
        moveToEnd(node);
        return node->value;
    }
    
    return Value{}; // Return default value if not found
}

template<typename Key, typename Value>
bool MyLRUCache<Key, Value>::get(const Key& key, Value& value) {
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    
    auto it = cache.find(key);
    if (it != cache.end()) {
        NodePtr node = it->second;
        value = node->value;
        lock.unlock();
        
        // Upgrade to exclusive lock for modification
        std::unique_lock<std::shared_mutex> write_lock(rw_lock);
        moveToEnd(node);
        return true;
    }
    
    return false;
}

template<typename Key, typename Value>
void MyLRUCache<Key, Value>::put(const Key& key, const Value& value) {
    std::unique_lock<std::shared_mutex> lock(rw_lock);
    
    auto it = cache.find(key);
    if (it != cache.end()) {
        // Key exists, update value and move to end
        NodePtr node = it->second;
        node->value = value;
        moveToEnd(node);
    } else {
        // New key
        NodePtr new_node = std::make_shared<Node>(key, value);
        cache[key] = new_node;
        insertAtEnd(new_node);
        
        // Check capacity and evict if necessary
        if (cache.size() > capacity) {
            NodePtr lru_node = head->next;
            removeNode(lru_node);
            cache.erase(lru_node->key);
        }
    }
}

template<typename Key, typename Value>
bool MyLRUCache<Key, Value>::remove(const Key& key) {
    std::unique_lock<std::shared_mutex> lock(rw_lock);
    
    auto it = cache.find(key);
    if (it != cache.end()) {
        NodePtr node = it->second;
        removeNode(node);
        cache.erase(it);
        return true;
    }
    
    return false;
}

template<typename Key, typename Value>
size_t MyLRUCache<Key, Value>::size() const {
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    return cache.size();
}

template<typename Key, typename Value>
bool MyLRUCache<Key, Value>::empty() const {
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    return cache.empty();
}

template<typename Key, typename Value>
void MyLRUCache<Key, Value>::clear() {
    std::unique_lock<std::shared_mutex> lock(rw_lock);
    cache.clear();
    head->next = tail;
    tail->prev = head;
}

template<typename Key, typename Value>
size_t MyLRUCache<Key, Value>::getCapacity() const {
    return capacity;
}

template<typename Key, typename Value>
void MyLRUCache<Key, Value>::dump() const {
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    
    std::cout << "Cache contents (LRU to MRU): ";
    NodePtr current = head->next;
    while (current && current != tail) {
        std::cout << "[" << current->key << ":" << current->value << "] ";
        current = current->next;
    }
    std::cout << std::endl;
}
