// vector<uint64_t>& arr
// sort: mergesort
// sort based on countOfOnes()
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

class BitSorter {
public:
    // Function to sort integers based on 1s in its binary representation
    void sort(std::vector<uint64_t>& arr) {
        mergeSort(arr, 0, arr.size() - 1);
    }

private:
    // Function to count the number of 1s in the binary representation of a number
    int countOnes(uint64_t n) {
        int count = 0;
        while (n) {
            n &= (n - 1);
            count++;
        }
        return count;
    }

    void mergeSort(std::vector<uint64_t>& arr, std::vector<uint64_t>::size_type left, std::vector<uint64_t>::size_type right) {
        if(left < right) {
            auto mid = left + (right - left) / 2;

            //Recursively sort the two halves
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);

            // Merge the sorted halves
            merge(arr, left, mid, right);
        }
    }

    void merge(std::vector<uint64_t>& arr, std::vector<uint64_t>::size_type left,
                std::vector<uint64_t>::size_type mid, std::vector<uint64_t>::size_type right) {
        auto n1 = mid - left + 1;
        auto n2 = right - mid;

        // Temporary arrays
        std::vector<uint64_t> L(n1), R(n2);

        // Copy data to temporary arrays
        for (std::vector<uint64_t>::size_type i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (std::vector<uint64_t>::size_type i = 0; i < n2; i++)
            R[i] = arr[mid + 1 + i];

        // Merge the temporary arrays back into arr
        std::vector<uint64_t>::size_type i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (countOnes(L[i]) <= countOnes(R[j])) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }

        // Copy remaining elements of L, if any
        while (i < n1) {
            arr[k++] = L[i++];
        }
        // Copy remaining elements of R, if any
        while (j < n2) {
            arr[k++] = R[j++];
        }
    }
};

int main() {
    std::vector<uint64_t> numbers = {0xF, 0x1F, 0x3F, 0x1, 0x3, 0x7, 0x7F, 0xFF, 0x1FF, 0x3FF};

    BitSorter sorter;
    sorter.sort(numbers);

    std::cout << "Sorted numbers based on the number of 1s in binary representation: \n";
    for (uint64_t num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "Reversed vector: \n";
    std::reverse(numbers.begin(), numbers.end());
    for (uint64_t num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}