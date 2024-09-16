from typing import List

class Solution:
    def containerWithMostWater(self, height: List[int]) -> int:
        l, r = 0, len(height) - 1
        area, currArea = 0, 0

        while (l < r):
            currArea = min(height[l], height[r]) * (r - l)
            area = max(currArea, area)
            if (height[l] < height[r]):
                l += 1
            else:
                r -= 1

        return area
    
s = Solution()
height = [1,8,6,2,5,4,8,3,7]
assert s.containerWithMostWater(height) == 49
height = [1, 1]
assert s.containerWithMostWater(height) == 1