from typing import List
class Solution:
    def twoSumII(self, nums: List[int], target: int) -> List[int]:
        l, r = 0, len(nums) - 1

        while l < r:
            if (target - nums[l]) < nums[r]:
                r -= 1
            elif (target - nums[l]) > nums[r]:
                l += 1
            else:
                return [l + 1, r + 1]

s = Solution()
nums = [-10,-8,-2,1,2,5,6]
target = 0
assert s.twoSumII(nums, target) == [3, 5]
nums = [12,13,23,28,43,44,59,60,61,68,70,86,88,92,124,125,136,168,173,173,180,199,212,221,227,230,277,282,306,314,316,321,325,328,336,337,363,365,368,370,370,371,375,384,387,394,400,404,414,422,422,427,430,435,457,493,506,527,531,538,541,546,568,583,585]
target = 542
assert s.twoSumII(nums, target) == [24, 32]
nums = [2, 7, 11, 15]
target = 9
assert s.twoSumII(nums, target) == [1, 2]
nums = [-1, 0]
target = -1
assert s.twoSumII(nums, target) == [1, 2]
nums = [2, 3, 4]
target = 6
assert s.twoSumII(nums, target) == [1, 3]