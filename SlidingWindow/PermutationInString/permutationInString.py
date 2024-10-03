class Solution:
    def checkInclusion(self, s1, s2) -> bool:
        s1Count, s2Count = [0] * 26, [0] *26
        matches = 0

        if len(s1) > len(s2):
            return False

        for i in range(len(s1)):
            s1Count[ord(s1[i]) - ord('a')] += 1
            s2Count[ord(s2[i]) - ord('a')] += 1
        
        for i in range(26):
            matches += (1 if s1Count[i] == s2Count[i] else 0)

        l = 0
        for r in range(len(s1), len(s2)):
            if matches == 26:
                return True
            
            index = ord(s2[r]) - ord('a')
            s2Count[index] += 1
            if s1Count[index] == s2Count[index]:
                matches += 1
            elif s1Count[index] + 1 == s2Count[index]:
                matches -= 1

            index = ord(s2[l]) - ord('a')
            s2Count[index] -= 1
            if s1Count[index] == s2Count[index]:
                matches += 1
            elif s1Count[index] - 1 == s2Count[index]:
                matches -= 1
            l += 1
        return matches == 26

sol = Solution()
s1 = "ab"
s2 = "eidbaooo"
print(sol.checkInclusion(s1, s2))
s1 = "adc"
s2 = "dcda"
print(sol.checkInclusion(s1, s2))