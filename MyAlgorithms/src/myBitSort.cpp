#include "../include/myBitSort.hpp"
#include <algorithm>
#include <climits>

void MyBitSort::sort(std::vector<uint64_t>& arr) {
    sortByBitCount(arr, true);
}

void MyBitSort::sortByBitCount(std::vector<uint64_t>& arr, bool ascending) {
    if (arr.size() <= 1) {
        return;
    }
    mergeSort(arr, 0, arr.size() - 1, ascending);
}

int MyBitSort::countOnes(uint64_t n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // Brian Kernighan's algorithm
        count++;
    }
    return count;
}

int MyBitSort::countZeros(uint64_t n) {
    // Count total bits and subtract ones
    int total_bits = sizeof(uint64_t) * 8;
    return total_bits - countOnes(n);
}

void MyBitSort::mergeSort(std::vector<uint64_t>& arr, size_t left, size_t right, bool ascending) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;

        // Recursively sort the two halves
        mergeSort(arr, left, mid, ascending);
        mergeSort(arr, mid + 1, right, ascending);

        // Merge the sorted halves
        merge(arr, left, mid, right, ascending);
    }
}

void MyBitSort::merge(std::vector<uint64_t>& arr, size_t left, size_t mid, size_t right, bool ascending) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    // Create temporary arrays
    std::vector<uint64_t> leftArr(n1);
    std::vector<uint64_t> rightArr(n2);

    // Copy data to temporary arrays
    for (size_t i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (size_t i = 0; i < n2; i++) {
        rightArr[i] = arr[mid + 1 + i];
    }

    // Merge the temporary arrays back into arr
    size_t i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        int leftBits = countOnes(leftArr[i]);
        int rightBits = countOnes(rightArr[j]);
        
        bool should_take_left;
        if (ascending) {
            should_take_left = (leftBits < rightBits) || 
                              (leftBits == rightBits && leftArr[i] <= rightArr[j]);
        } else {
            should_take_left = (leftBits > rightBits) || 
                              (leftBits == rightBits && leftArr[i] <= rightArr[j]);
        }
        
        if (should_take_left) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // Copy remaining elements of leftArr, if any
    while (i < n1) {
        arr[k++] = leftArr[i++];
    }

    // Copy remaining elements of rightArr, if any
    while (j < n2) {
        arr[k++] = rightArr[j++];
    }
}
