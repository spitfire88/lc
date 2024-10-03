#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int binary_search(int *nums, int numSize, int target) {
    int pivot, left = 0, right = numSize - 1;

    while(left < right) {
        pivot = left + ((right - left) >> 1);
        if(nums[pivot] == target)
            return pivot;
        else if(nums[pivot] > target)
            right = pivot - 1;
        else
            left = pivot + 1;
    }
    return -1;
}