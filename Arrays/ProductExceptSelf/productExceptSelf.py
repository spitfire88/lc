from typing import List
import random

class Solution:
    def productOfArrExceptSelf(self, nums: List[int]) -> List[int]:
        res = [1] * len(nums)
        prefix = 1
        for i in range(len(nums)):
            res[i] = prefix
            prefix *= nums[i]
        postfix = 1
        for i in range(len(nums) - 1, -1 , -1):
            res[i] *= postfix
            postfix *= nums[i]
        return res

solution = Solution()
nums = list(range(2,10))
random.shuffle(nums)
print(nums)
print(solution.productOfArrExceptSelf(nums))