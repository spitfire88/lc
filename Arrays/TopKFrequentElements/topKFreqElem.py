from typing import List
import heapq
import time

class Solution:
    # Using Heap with time complexity O(nlogk)
    # and space complexity O(n)
    def topKFrequentElemHeap(self, nums: List[int], k: int) -> List[int]:
        count = {}
        for num in nums:
            if num not in count:
                count[num] = 0
            count[num] += 1
        heap = []
        for key, value in count.items():
            heapq.heappush(heap, (-value, key))
        res = []
        for _ in range(k):
            res.append(heapq.heappop(heap)[1])
        return res
    
    # Bucket sort with time complexity O(n)
    # and space complexity O(n)
    def topKFrequentElemBucket(self, nums: List[int], k: int) -> List[int]:
        count = {}
        for num in nums:
            if num not in count:
                count[num] = 0
            count[num] += 1
        bucket = [None] * (len(nums) + 1)
        for key, value in count.items():
            if bucket[value] is None:
                bucket[value] = []
            bucket[value].append(key)
        res = []
        for i in range(len(bucket) - 1, -1, -1):
            if bucket[i] is not None:
                res.extend(bucket[i])
            if len(res) >= k:
                break
        return res[:k]
    
sol = Solution()
# Test cases
# Case 1
nums = [1] * 20 + [2] * 15 + [3] * 10 + [4] * 8 + [5] * 7 + [6] * 6 + [7] * 5 + [8] * 4 + [9] * 3 + [10] * 2 + [11] + [12] + [13] + [14] + [15] + [16] + [17] + [18] + [19] + [20]
k = 2
timeStamp = time.time()
assert sol.topKFrequentElemBucket(nums, k) == [1, 2]
print("Time taken by Bucket: ", time.time() - timeStamp)
timeStamp = time.time()
assert sol.topKFrequentElemHeap(nums, k) == [1, 2]
print("Time taken by Heap: ", time.time() - timeStamp)


# Case 2
nums = [1]
k = 1
assert sol.topKFrequentElemHeap(nums, k) == [1]
assert sol.topKFrequentElemBucket(nums, k) == [1]