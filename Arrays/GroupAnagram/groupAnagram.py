from collections import defaultdict
from typing import List

class Solution:
    def groupAnagram(self, strs: List[str]) -> List[List[str]]:
        res = defaultdict(list) # mapping charCount to list of Anagrams

        for s in strs:
            count = [0] * 26

            for c in s:
                count[ord(c) - ord("a")] += 1

            res[tuple(count)].append(s)
        
        return res.values()

def sorted_list_of_lists(lists):
    return sorted([sorted(inner_list) for inner_list in lists])

sol = Solution()
#testcases
#Case 1
strs = ["ate", "tea", "eat", "ab", "bab", "ba"]
assert sorted_list_of_lists(sol.groupAnagram(strs)) == sorted_list_of_lists([['ate', 'tea', 'eat'], ['ab', 'ba'], ['bab']])
#Case 2
strs = ["a"]
assert sorted_list_of_lists(sol.groupAnagram(strs)) == sorted_list_of_lists([['a']])
#Case 3
strs = ["a", "b", "c"]
assert sorted_list_of_lists(sol.groupAnagram(strs)) == sorted_list_of_lists([['a'], ['b'], ['c']])
