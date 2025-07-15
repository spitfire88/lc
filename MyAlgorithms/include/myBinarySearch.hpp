#ifndef MY_BINARY_SEARCH_HPP
#define MY_BINARY_SEARCH_HPP

#include <vector>
#include <functional>

/**
 * @brief Generic binary search implementation with multiple search strategies
 * 
 * This class provides various binary search algorithms for different use cases
 * including standard search, range queries, and custom predicates.
 */
template<typename T>
class MyBinarySearch {
public:
    /**
     * @brief Standard binary search for exact match
     * @param arr Sorted array to search
     * @param target Value to find
     * @return Index of target if found, -1 otherwise
     */
    static int search(const std::vector<T>& arr, const T& target);

    /**
     * @brief Binary search with custom comparator
     * @param arr Sorted array to search
     * @param target Value to find
     * @param comp Custom comparator function
     * @return Index of target if found, -1 otherwise
     */
    static int search(const std::vector<T>& arr, const T& target, 
                     std::function<bool(const T&, const T&)> comp);

    /**
     * @brief Find first occurrence of target
     * @param arr Sorted array (may contain duplicates)
     * @param target Value to find
     * @return Index of first occurrence, -1 if not found
     */
    static int findFirst(const std::vector<T>& arr, const T& target);

    /**
     * @brief Find last occurrence of target
     * @param arr Sorted array (may contain duplicates)
     * @param target Value to find
     * @return Index of last occurrence, -1 if not found
     */
    static int findLast(const std::vector<T>& arr, const T& target);

    /**
     * @brief Find insertion point for target to maintain sorted order
     * @param arr Sorted array
     * @param target Value to insert
     * @return Index where target should be inserted
     */
    static int findInsertionPoint(const std::vector<T>& arr, const T& target);

    /**
     * @brief Find largest element less than or equal to target
     * @param arr Sorted array
     * @param target Upper bound value
     * @return Index of largest element <= target, -1 if none exists
     */
    static int findFloor(const std::vector<T>& arr, const T& target);

    /**
     * @brief Find smallest element greater than or equal to target
     * @param arr Sorted array
     * @param target Lower bound value
     * @return Index of smallest element >= target, -1 if none exists
     */
    static int findCeil(const std::vector<T>& arr, const T& target);

    /**
     * @brief Find range of indices containing target value
     * @param arr Sorted array (may contain duplicates)
     * @param target Value to find range for
     * @return Pair of (first_index, last_index), (-1, -1) if not found
     */
    static std::pair<int, int> findRange(const std::vector<T>& arr, const T& target);

    /**
     * @brief Search in rotated sorted array
     * @param arr Rotated sorted array
     * @param target Value to find
     * @return Index of target if found, -1 otherwise
     */
    static int searchRotated(const std::vector<T>& arr, const T& target);

    /**
     * @brief Find peak element in array
     * @param arr Array where arr[i] != arr[i+1]
     * @return Index of any peak element
     */
    static int findPeak(const std::vector<T>& arr);

private:
    /**
     * @brief Generic binary search template
     * @param arr Array to search
     * @param left Left boundary
     * @param right Right boundary
     * @param predicate Search predicate
     * @return Index found by predicate
     */
    static int binarySearchGeneric(const std::vector<T>& arr, int left, int right,
                                  std::function<bool(int)> predicate);
};

// Include implementation for template class
#include "../src/myBinarySearch.tpp"

#endif // MY_BINARY_SEARCH_HPP
