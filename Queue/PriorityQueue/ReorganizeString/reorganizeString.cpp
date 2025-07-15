// Given a string, rearrage it so that there is no repeating character. If not possible return NULL
// str = "abaabbc", return = "abababc"
// clarifying questions: only small letters? no numbers? yes
// brute force: abababc - stack. abbba - ababb fail
// priority queue: 
// 1. Go through the string and keep a count of characters
// 2. Add to priority queue {count, char} - top most will have highest count on top
// 3. pop first[char] from pq in result but if result.back is same, pop second[char] in result
// 4. if first was added, and --first[count] is not zero, push to pq
// 5. else --second[count] is not zero, push to pq
// 6. if first[char] == result.back and there is not second to pop from pq, return NULL

#include <vector>
#include <queue>
#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    string reorganizeString(string s) {
        vector<int> charCount (26, 0);
        for(char c : s) {
            charCount[c - 'a']++;
        }

        priority_queue<vector<int>> pq;
        for(int i; i < 26; i++) {
            if(charCount[i] > 0) {
                // push {count, char}
                pq.push(vector<int>{charCount[i], i + 'a'});
            }
        }

        string result;
        while(!pq.empty()) {
            auto first = pq.top(); // {count, char}
            pq.pop();
            if(result.empty() || result.back() != first[1]) {
                result += (char)first[1];
                if(--first[0] > 0) {
                    pq.push(first);
                }
            } else { // result.back is same as first
                if(pq.empty()) { // there is no second
                    return "";
                }
                auto second = pq.top();
                pq.pop();
                result += (char)second[1];
                if(--second[0] > 0) {
                    pq.push(second);
                }
                pq.push(first);
            }
        }
        return result;
    }
};

int main() {
    string test = "abbabba";
    Solution S;
    string res = S.reorganizeString(test);
    cout << test << " reorganized to " << res << endl;
    return 0;
}