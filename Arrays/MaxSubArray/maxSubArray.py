from typing import List
import random

class Solution:
    def maxSubArray(self, nums: List[int]) -> int:
        curSum = 0
        maxSub = nums[0]

        for n in nums:
            if curSum < 0:
                curSum = 0
            curSum += n
            maxSub = max(maxSub, curSum)
        
        return maxSub


solution = Solution()
nums = list(range(-2, 3))
nums.extend([random.choice(nums) for _ in range(len(nums)//2)])
nums[::-4]
random.shuffle(nums)
print(nums)
print(solution.maxSubArray(nums))