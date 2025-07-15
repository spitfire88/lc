#ifndef MY_BIT_SORT_HPP
#define MY_BIT_SORT_HPP

#include <vector>
#include <cstdint>

/**
 * @brief Custom bit-based sorting implementation
 * 
 * This class provides sorting algorithms based on the number of 1s
 * in the binary representation of numbers.
 */
class MyBitSort {
public:
    /**
     * @brief Sort vector based on number of 1s in binary representation
     * @param arr Vector to sort
     */
    void sort(std::vector<uint64_t>& arr);

    /**
     * @brief Sort vector with custom comparator based on bit patterns
     * @param arr Vector to sort
     * @param ascending Sort in ascending order if true, descending if false
     */
    void sortByBitCount(std::vector<uint64_t>& arr, bool ascending = true);

    /**
     * @brief Count number of 1s in binary representation
     * @param n Number to count bits for
     * @return Number of 1s in binary representation
     */
    static int countOnes(uint64_t n);

    /**
     * @brief Count number of 0s in binary representation
     * @param n Number to count bits for
     * @return Number of 0s in binary representation
     */
    static int countZeros(uint64_t n);

private:
    /**
     * @brief Merge sort implementation for bit-based sorting
     * @param arr Vector to sort
     * @param left Left index
     * @param right Right index
     * @param ascending Sort order
     */
    void mergeSort(std::vector<uint64_t>& arr, size_t left, size_t right, bool ascending);

    /**
     * @brief Merge function for merge sort
     * @param arr Vector being sorted
     * @param left Left index
     * @param mid Middle index
     * @param right Right index
     * @param ascending Sort order
     */
    void merge(std::vector<uint64_t>& arr, size_t left, size_t mid, size_t right, bool ascending);
};

#endif // MY_BIT_SORT_HPP
