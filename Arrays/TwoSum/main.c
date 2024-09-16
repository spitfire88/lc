/***
Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.

You may assume that each input would have exactly one solution, and you may not use the same element twice.

You can return the answer in any order.

 

 Example 1:

 Input: nums = [2,7,11,15], target = 9
 Output: [0,1]
 Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].
 Example 2:

 Input: nums = [3,2,4], target = 6
 Output: [1,2]
 Example 3:

 Input: nums = [3,3], target = 6
 Output: [0,1]
  

  Constraints:

  2 <= nums.length <= 104
  -109 <= nums[i] <= 109
  -109 <= target <= 109
  Only one valid answer exists.
   

   Follow-up: Can you come up with an algorithm that is less than O(n2) time complexity?
***/

#include <stdio.h>
#include <stdlib.h>


// i -> 0..., 
// for j = i + 1, target - nums[i] == nums[j], return [i, j]
//
int *twoSum(int *nums, int numsSize, int target, int *returnSize) {
    unsigned int i = -1, j = -1;
    int *result = (int *)malloc(2 * sizeof(int));
    //result[0] = i;
    //result[1] = j;
    *returnSize = 2;

    //printf("result[0], [1] = %d %d\n", result[0], result[1]);

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (target - nums[i] == nums[j]) {
                //printf("i=%d, j=%d\n", i, j);
                result[0] = i;
                result[1] = j;
                //printf("result[0], [1] = %d %d\n", result[0], result[1]);
                goto out;
            }
out:
    return result;
}
