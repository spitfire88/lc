# rotated sorted array
from typing import List

# l.. m ..r
# 4 5 1 2 3

class Solution:
    def minInRotatedSortedArr(self, nums: List[int]) -> int:
        res = nums[0]
        l, r = 0, len(nums) - 1
        while l <= r:
            if nums[l] < nums[r]:
                res = min(res, nums[l])
                break
            
            m = (l + r) // 2
            res = min(res, nums[m])
            if nums[m] >= nums[l]:
                l = m + 1
            else:
                r = m - 1

        return res
    
s = Solution()
nums = list(range(1, 6))
rotate_by = 2
nums = nums[-rotate_by:] + nums[:len(nums) - rotate_by]
print(s.minRotatedSortedArr(nums))