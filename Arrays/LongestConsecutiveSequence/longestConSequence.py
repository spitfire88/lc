from typing import List

class Solution:
    def longestConsecutiveSeqSort(self, nums: List[int]) -> int: # O(nlogn)
        nums.sort()
        max_length = 1
        current_length = 1

        for i in range(1, len(nums)):
            if nums[i] == nums[i-1] + 1:
                current_length += 1
            elif nums[i] != nums[i-1]:
                current_length = 1
            max_length = max(max_length, current_length)

        return max_length
    
    def longestConsecutiveSeq(self, nums: List[int]) -> int: # O(n)
        nums = set(nums)
        res = 0
        for n in nums:
            if (n - 1) not in nums:
                count = 1
                while (n + 1) in nums:
                    count += 1
                    n += 1
                res = max(res, count)
        return res

s = Solution()
print(s.longestConsecutiveSeq([100, 4, 200, 1, 3, 2]))  # 4
print(s.longestConsecutiveSeq([0,3,7,2,5,8,4,6,0,1]))  # 9
print(s.longestConsecutiveSeq([1,2,0,1]))  # 3
print(s.longestConsecutiveSeq([1,2,0,1,2]))  # 3
print(s.longestConsecutiveSeq([1,2,0,1,2,3]))  # 4
print(s.longestConsecutiveSeq([1,2,0,1,2,3,4]))  # 5
print(s.longestConsecutiveSeq([1,2,0,1,2,3,4,5]))  # 6