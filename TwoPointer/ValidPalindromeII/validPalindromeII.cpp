/*
Given a string s, return true if the s can be palindrome after deleting at most one character from it.

 

Example 1:

Input: s = "aba"
Output: true
Example 2:

Input: s = "abca"
Output: true
Explanation: You could delete the character 'c'.
Example 3:

Input: s = "abc"
Output: false
*/

/*
    two pointer method
    abca
    while low < high
    if s.low == s.high : low++, high--
    if s.low != s.high : isValidPalindrome(s, low + 1, high) || isValidPalindrome(s, low, high - 1)
    else return false
    return true

    isValidPalindrome()
    if(s.low != s.high) return false
    else low++, high--
    return true

*/

#include <iostream>
#include <cstdbool>

using namespace std;

class Solution {
public:
    bool validPalindrome(string s) {
        int low = 0, high = s.size() - 1;
        while(low < high) {
            if (s[low] != s[high]) {
                return isPalindrome(s, low + 1, high) || isPalindrome(s, low, high - 1);
            }
            low++;
            high--;
        }
        return true;
    }

    bool isPalindrome(string& s, int low, int high) {
        while(low < high) {
            if(s[low] != s[high]) {
                return false;
            }
            low++;
            high--;
        }
        return true;
    }
};

int main() {
    string s = "cupuufxoohdfpgjdmysgvhmvffcnqxjjxqncffvmhvgsymdjgpfdhooxfuupucu";
    Solution sol;
    cout << "Palindrome of " << s << " skipping 1 char at most: " << sol.validPalindrome(s) << endl;
    return 0;
}