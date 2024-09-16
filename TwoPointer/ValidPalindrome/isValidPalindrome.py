class Solution:
    def isValidPalindrome(self, s: str) -> bool:
        filteredStr = ""
        for c in s:
            if c.isalnum():
                filteredStr += c
        
        if len(filteredStr) == 0:
            return True
        else:
            for i in range(0, len(filteredStr) // 2):
                if filteredStr[i].lower() != filteredStr[len(filteredStr) - i - 1].lower():
                    return False
            return True
        
s = Solution()
print(s.isValidPalindrome("Was it a car or a cat I saw?")) # True
print(s.isValidPalindrome("tab a cat")) # False
print(s.isValidPalindrome("a")) # True
print(s.isValidPalindrome("a.")) # True
print(s.isValidPalindrome("")) # True