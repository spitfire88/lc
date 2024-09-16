from typing import List
import random

# Your code here
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        m = {} # {num, index} map
        for i, n in enumerate(nums):
            if (target - n) not in m:
                m[n] = i
            else:
                return [i, m[target - n]]

# Create an instance of the Solution class
solution = Solution()

# Test case 1
#nums = list(range(1, 10000001))
#random.shuffle(nums)
#target = 1
#result = solution.twoSum(nums, target)
#print(result)  # Expected output: [0, 1]

# Test case 2
nums = [3, 2, 4]
target = 6
result = solution.twoSum(nums, target)
print(result)  # Expected output: [1, 2]

# Test case 3
nums = [3, 3]
target = 6
result = solution.twoSum(nums, target)
print(result)  # Expected output: [0, 1]
