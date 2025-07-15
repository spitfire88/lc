#include "../include/myBinarySearch.hpp"
#include <climits>

template<typename T>
int MyBinarySearch<T>::search(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

template<typename T>
int MyBinarySearch<T>::search(const std::vector<T>& arr, const T& target, 
                             std::function<bool(const T&, const T&)> comp) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (comp(arr[mid], target)) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

template<typename T>
int MyBinarySearch<T>::findFirst(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            result = mid;
            right = mid - 1; // Continue searching in left half
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}

template<typename T>
int MyBinarySearch<T>::findLast(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            result = mid;
            left = mid + 1; // Continue searching in right half
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}

template<typename T>
int MyBinarySearch<T>::findInsertionPoint(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size());
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return left;
}

template<typename T>
int MyBinarySearch<T>::findFloor(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] <= target) {
            result = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}

template<typename T>
int MyBinarySearch<T>::findCeil(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] >= target) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    return result;
}

template<typename T>
std::pair<int, int> MyBinarySearch<T>::findRange(const std::vector<T>& arr, const T& target) {
    int first = findFirst(arr, target);
    if (first == -1) {
        return {-1, -1};
    }
    
    int last = findLast(arr, target);
    return {first, last};
}

template<typename T>
int MyBinarySearch<T>::searchRotated(const std::vector<T>& arr, const T& target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        // Check which half is sorted
        if (arr[left] <= arr[mid]) {
            // Left half is sorted
            if (target >= arr[left] && target < arr[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {
            // Right half is sorted
            if (target > arr[mid] && target <= arr[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    return -1;
}

template<typename T>
int MyBinarySearch<T>::findPeak(const std::vector<T>& arr) {
    if (arr.empty()) {
        return -1;
    }
    
    if (arr.size() == 1) {
        return 0;
    }
    
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] > arr[mid + 1]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    
    return left;
}

template<typename T>
int MyBinarySearch<T>::binarySearchGeneric(const std::vector<T>& arr, int left, int right,
                                          std::function<bool(int)> predicate) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (predicate(mid)) {
            return mid;
        }
        
        // This is a simplified version - actual implementation would need
        // more context about how to navigate based on predicate result
        left = mid + 1;
    }
    
    return -1;
}
