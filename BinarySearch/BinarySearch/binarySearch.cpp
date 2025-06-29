// Binary search a sorted array for a target
#include <iostream>
#include <vector>
#include <cassert>

class BinarySearch {
public:
    int search(std::vector<int>& nums, int target) {
        int low = 0;
        int high = nums.size() - 1;
        
        while(low <= high) {
            auto mid = low + (high - low) / 2;
            if(nums[mid] == target)
                return mid;
            if(nums[mid] > target)
                high = mid - 1;
            else 
                low = mid + 1;
        }

        return -1;
    }
};

int main() {
    std::vector<int> numbers = {-1, 4, 6, 9, 11};
    BinarySearch bsearch;
    int target = 6;
    if(target > numbers[0] && target < numbers[numbers.size() - 1]) {
        int result = bsearch.search(numbers, target);
        std::cout << "Target is at: " << result << std::endl;
    } else {
        std::cout << "Target is not part of numbers\n";
    }

    return 0;
}