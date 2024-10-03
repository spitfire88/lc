class Solution:
    def charReplacementNC(self, s, k) -> int:
        count = {}
        res = 0
        l = 0
        maxF = 0

        for r in range(len(s)):
            count[s[r]] = 1 + count.get(s[r], 0)
            maxF = max(maxF, count[s[r]])

            if (r - l + 1) - maxF > k:
                count[s[l]] -= 1
                l += 1
            
            res = max(res, r - l + 1)
        
        return res

sol = Solution()
s = "ABBB"
k = 2
print(sol.charReplacementNC(s, k))
s = "AABABBA"
k = 3
print(sol.charReplacementNC(s, k))
s = "AAAA"
k = 2
print(sol.charReplacementNC(s, k))
s = ""
k = 2
print(sol.charReplacementNC(s, k))