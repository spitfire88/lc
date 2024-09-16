from typing import List
from collections import defaultdict

class Solution:
    def twoSum(self, n: List[int], target: int) -> List[List[int]]:
        l, r = 0, len(n) - 1
        p = []
        while l < r:
            if (target - n[l] == n[r]):
                p.append([n[l], n[r]])
                l += 1
                r -= 1
            elif (n[l] + n[r] > target or n[r] > target):
                r -= 1
            else:
                l += 1
        return p             

    def threeSum(self, nums: List[int]) -> List[List[int]]:
        nums.sort()
        res = []
        i, j, k = 0, 0, 0

        for i in range(len(nums) - 2):
            j, k = i + 1, len(nums)
            x = self.twoSum(nums[j:k], -nums[i])
            if x is not None:
                for y in x:
                    if [nums[i], y[0], y[1]] not in res:
                        res.append([nums[i], y[0], y[1]])
            i += 1

        return res
    
s = Solution()
print(s.threeSum([-4,-2,-2,-2,0,1,2,2,2,3,3,4,4,6,6])) # [-4,-2,6],[-4,0,4],[-4,1,3],[-4,2,2],[-2,-2,4],[-2,0,2]
print(s.threeSum([-1,0,1,2,-1,-4,-2,-3,3,0,4])) # [[-4,0,4],[-4,1,3],[-3,-1,4],[-3,0,3],[-3,1,2],[-2,-1,3],[-2,0,2],[-1,-1,2],[-1,0,1]]