from typing import List
import time

class Solution:
    def maxProductSubArray(self, nums: List[int]) -> int:
        start = time.time_ns()
        res = max(nums)
        curMin, curMax = 1, 1

        for n in nums:
            if n == 0:
                curMin, curMax = 1, 1
                continue

            tmp = curMax * n
            curMax = max(tmp, n * curMin, n)
            curMin = min(tmp, n * curMin, n)
            res = max(res, curMax)
        
        print(time.time_ns() - start)
        return res
    
solution = Solution()
nums = [1, 4, -1, 0, -3, -2]
#nums = [-1] * 1000
print(solution.maxProductSubArray(nums))