from typing import List

class Solution:
    def encodeint(self, strs: List[str]) -> str:
        res = ""
        for s in strs:
            res += str(len(s)) + "#" + s
        return res
    
    def decodeint(self, str) -> List[str]:
        res, i = [], 0

        while i < len(str):
            j = i
            while (str[j] != '#'): # 3#abc
                j += 1
            # i=0, j=2
            length = int(str[i:j])
            res.append(str[j + 1 : j + 1 + length])
            i = j + 1 + length
        
        return res

    def encodehex(self, strs: List[str]) -> str:
        res = ""
        for s in strs:
            res += format(len(s), 'x') + "#" + s
        return res
    
    def decodehex(self, s: str) -> List[str]:
        res, i = [], 0
        while i < len(s):
            j = i
            while s[j] != "#":
                j += 1
            length = int(s[i:j], 16)
            res.append(s[j + 1 : j + 1 + length])
            i = j + 1 + length
        return res

s = Solution()      
# Test case 1: Empty list
strs = []
print(s.encodeint(strs))  # Output: ""
print(s.decodeint(""))  # Output: []
print(s.encodehex(strs))  # Output: ""
print(s.decodehex(""))  # Output: []

# Test case 2: List with one empty string
strs = [""]
print(s.encodeint(strs))  # Output: "0#"
print(s.decodeint("0#"))  # Output: [""]
print(s.encodehex(strs))  # Output: "0#"
print(s.decodehex("0#"))  # Output: [""]

# Test case 3: List with multiple empty strings
strs = ["", "", ""]
print(s.encodeint(strs))  # Output: "0#0#0#"
print(s.decodeint("0#0#0#"))  # Output: ["", "", ""]
print(s.encodehex(strs))  # Output: "0#0#0#"
print(s.decodehex("0#0#0#"))  # Output: ["", "", ""]

# Test case 4: List with strings of different lengths
strs = ["abc", "defg", "hijklm"]
print(s.encodeint(strs))  # Output: "3#abc4#defg6#hijklm"
print(s.decodeint("3#abc4#defg6#hijklm"))  # Output: ["abc", "defg", "hijklm"]
print(s.encodehex(strs))  # Output: "3#abc4#defg6#hijklm"
print(s.decodehex("3#abc4#defg6#hijklm"))  # Output: ["abc", "defg", "hijklm"]

# Test case 5: List with special characters
strs = ["!@#$%^&*()", "1234567890", "abcdefghijklmnopqrstuvwxyz"]
print(s.encodeint(strs))  # Output: "10#!@#$%^&*()10#123456789026#abcdefghijklmnopqrstuvwxyz"
print(s.decodeint("10#!@#$%^&*()10#123456789026#abcdefghijklmnopqrstuvwxyz"))  # Output: ["!@#$%^&*()", "1234567890", "abcdefghijklmnopqrstuvwxyz"]
print(s.encodehex(strs))  # Output: "a#!@#$%^&*()a#123456789019#abcdefghijklmnopqrstuvwxyz"
print(s.decodehex("a#!@#$%^&*()a#12345678901a#abcdefghijklmnopqrstuvwxyz"))  # Output: ["!@#$%^&*()", "1234567890", "abcdefghijklmnopqrstuvwxyz"]

            