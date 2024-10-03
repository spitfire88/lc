from typing import List

class Solution:
    def bestTimeBuyNSell(self, nums: List[int]) -> int:
        l, r = 0, 1
        diff, profit = 0, 0
        while r < len(nums):
            diff = nums[r] - nums[l]
            if (diff <= 0) or (nums[r] < nums[l]):
                l = r
                r += 1
            else:
                profit = max(profit, diff)
                r += 1
        return profit

s = Solution()
nums = [7, 2, 5, 1, 6, 4]
print(s.bestTimeBuyNSell(nums))