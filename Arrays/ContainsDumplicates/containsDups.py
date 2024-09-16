from typing import List
import time

class Solution:
    def containsDuplicates(self, nums: List[int]) -> bool:
        h = set()
        for n in nums:
            if n in h:
                return True
            h.add(n)
        return False

    def containsDuplicatesAtEnd(self, nums: List[int]) -> bool:
        h = set()
        for n in range(len(nums) - 1, -1, -1):
            if nums[n] in h:
                return True
            h.add(nums[n])
        return False

    def containsDuplicatesInPlace(self, nums: List[int]) -> bool:
        nums.sort()
        for n in range(1, len(nums)):
            if nums[n] == nums[n-1]:
                return True
        return False

s = Solution()
n1 = [1, 2, 3, 1]
n2 = [1, 2, 3, 4]
n3 = list(range(0, 100000))
n3[-3] = 99995
n3.sort()

# Test Cases
# Case 1
assert s.containsDuplicates(n1) == True
# Case 2
assert s.containsDuplicates(n2) == False
# Case 3
tstamp = time.time()
assert s.containsDuplicatesAtEnd(n3) == True
print (time.time() - tstamp)
tstamp
# Case 4
assert s.containsDuplicates(n3) == True
print (time.time() - tstamp)