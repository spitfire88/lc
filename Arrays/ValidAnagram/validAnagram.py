from typing import List
from collections import defaultdict

class Solution:
    def validAnagram(self, s: str, t: str) -> bool:
        sh, th = {}, {}
        if len(s) == len(t) and s == t:
            return True
        elif (len(s) == len(t)) and (s != t):
            for i in range(len(s)):
                if s[i] not in sh:
                    sh[s[i]] = 0
                else:
                    sh[s[i]] += 1  
                if t[i] not in th:
                    th[t[i]] = 0
                else:
                    th[t[i]] += 1
            return sh == th
        return False

    def validAnagramDefaultdict(self, s: str, t: str) -> bool:
        sdict, tdict = defaultdict(int), defaultdict(int)
        if len(s) == len(t) and s == t:
            return True
        elif len(s) == len(t):
            for i in range(len(s)):
                sdict[s[i]] += 1
                tdict[t[i]] += 1
            return sdict == tdict
        return False
    
sol = Solution()
# Test cases
# Case 1
s = "anagram"
t = "nagaram"
assert sol.validAnagramDefaultdict(s, t) == True
# Case 2
s = "rat"
t = "car"
assert sol.validAnagram(s, t) == False
# Case 3
s = "a"
t = "ab"
assert sol.validAnagram(s, t) == False
# Case 4
s = "a"
t = "a"
assert sol.validAnagram(s, t) == True