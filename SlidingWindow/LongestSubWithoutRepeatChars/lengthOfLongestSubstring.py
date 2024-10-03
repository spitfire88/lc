
class Solution:
    def lengthOfLongestSubstringNC(self, s: str) -> int:
        x = set()
        l, r, maxlen = 0, 1, 0
        while l < len(s):
            x.add(s[l])
            maxlen = max(maxlen, 1)
            while r < len(s):
                if s[r] not in x:
                    x.add(s[r])
                    maxlen = max(maxlen, r - l + 1)
                    r += 1
                else:
                    x.discard(s[l])
                    l += 1
            return maxlen
        return maxlen

sol = Solution()
s = "dvdf"
print(sol.lengthOfLongestSubstringNC(s))
s = "pwwkew"
print(sol.lengthOfLongestSubstringNC(s))
s = "bbbbb"
print(sol.lengthOfLongestSubstringNC(s))
s = "abcabcbb"
print(sol.lengthOfLongestSubstringNC(s))
s = " "
print(sol.lengthOfLongestSubstringNC(s))
s = ""
print(sol.lengthOfLongestSubstringNC(s))
