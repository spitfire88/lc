#ifndef MY_LRU_CACHE_HPP
#define MY_LRU_CACHE_HPP

#include <unordered_map>
#include <shared_mutex>
#include <memory>

/**
 * @brief Thread-safe LRU (Least Recently Used) Cache implementation
 * 
 * This class provides a generic LRU cache with configurable capacity
 * and thread-safe operations using reader-writer locks.
 */
template<typename Key, typename Value>
class MyLRUCache {
private:
    /**
     * @brief Node structure for doubly linked list
     */
    struct Node {
        Key key;
        Value value;
        std::shared_ptr<Node> prev;
        std::shared_ptr<Node> next;

        Node(const Key& k, const Value& v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    using NodePtr = std::shared_ptr<Node>;

    size_t capacity;
    mutable std::shared_mutex rw_lock;
    std::unordered_map<Key, NodePtr> cache;
    NodePtr head;
    NodePtr tail;

    /**
     * @brief Remove node from its current position
     * @param node Node to remove
     */
    void removeNode(NodePtr node);

    /**
     * @brief Insert node at the end (most recently used position)
     * @param node Node to insert
     */
    void insertAtEnd(NodePtr node);

    /**
     * @brief Move node to end (mark as most recently used)
     * @param node Node to move
     */
    void moveToEnd(NodePtr node);

public:
    /**
     * @brief Constructor
     * @param cap Maximum capacity of the cache
     */
    explicit MyLRUCache(size_t cap);

    /**
     * @brief Destructor
     */
    ~MyLRUCache() = default;

    /**
     * @brief Get value by key
     * @param key Key to search for
     * @return Value if found, default value otherwise
     */
    Value get(const Key& key);

    /**
     * @brief Get value by key with existence check
     * @param key Key to search for
     * @param value Reference to store the value
     * @return true if key exists, false otherwise
     */
    bool get(const Key& key, Value& value);

    /**
     * @brief Put key-value pair into cache
     * @param key Key to insert/update
     * @param value Value to store
     */
    void put(const Key& key, const Value& value);

    /**
     * @brief Remove key from cache
     * @param key Key to remove
     * @return true if key was found and removed, false otherwise
     */
    bool remove(const Key& key);

    /**
     * @brief Get current size of cache
     * @return Number of elements in cache
     */
    size_t size() const;

    /**
     * @brief Check if cache is empty
     * @return true if empty, false otherwise
     */
    bool empty() const;

    /**
     * @brief Clear all elements from cache
     */
    void clear();

    /**
     * @brief Get cache capacity
     * @return Maximum capacity
     */
    size_t getCapacity() const;

    /**
     * @brief Print cache contents (for debugging)
     */
    void dump() const;
};

// Include implementation for template class
#include "../src/myLRUCache.tpp"

#endif // MY_LRU_CACHE_HPP
