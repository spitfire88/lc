#include<vector>
#include<iostream>
using namespace std;

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        // since nums1 can fit both, lets start iterating backwards and find the final position of integers from both arrays
        int i = m - 1;
        int j = n - 1;
        int k = nums1.size() - 1;

        while(i >= 0 && j >= 0) {
            if(nums1[i] < nums2[j]) {
                nums1[k] = nums2[j];
                k--;
                j--;
            } else {
                nums1[k] = nums1[i];
                k--;
                i--;
            }
        }
        while(i >= 0) {
            nums1[k] = nums1[i];
            k--;
            i--;
        }

        while(j >= 0) {
            nums1[k] = nums2[j];
            k--;
            j--;
        }
    }
};

int main() {
    vector<int> num1 = {0, 1, 4, 8, 0, 0, 0, 0};
    vector<int> num2 = {0, 1, 4, 8};

    Solution s;
    s.merge(num1, 4, num2, num2.size());

    for(auto i : num1) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}