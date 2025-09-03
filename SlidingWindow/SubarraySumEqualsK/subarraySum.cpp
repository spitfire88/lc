#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// Positive and negative integers. Keep complement
int subarraySumEqualsK(vector<int>& nums, int k) {
    int count = 0;
    int curr_prefix_sum = 0;
    // {complement_sum, freq}
    unordered_map<int, int> prefix_sum_to_freq;
    prefix_sum_to_freq[0] = 1; // the case where the whole array adds to k

    for(int num : nums) {
        curr_prefix_sum += num;
        if(prefix_sum_to_freq.find(curr_prefix_sum - k) != prefix_sum_to_freq.end()) {
            count += prefix_sum_to_freq[curr_prefix_sum - k];
        }
        prefix_sum_to_freq[curr_prefix_sum]++;
    }
}

// Positive and negative integers but return true on first occurrence
bool subarraySumEqualsKTrueOrFalse(vector<int>& nums, int k) {
    int curr_prefix_sum = 0;
    unordered_set<int> prefix_sums;
    prefix_sums.insert(0);

    for(int num : nums) {
        curr_prefix_sum += num;
        if(prefix_sums.find(curr_prefix_sum - k) != prefix_sums.end()) {
            return true;
        }
        prefix_sums.insert(curr_prefix_sum);
    }
    return false;
}

// Only positive integers
bool subarraySumPositiveIntegers(vector<int>& nums, int k) {
    int left = 0;
    int sum = 0;
    for(int right = 0; right < nums.size(); right++) {
        sum += nums[right];
        while(sum > k) {
            sum -= nums[left];
            left++;
        }
        if(sum == k) {
            return true;
        }
    }
    return false;
}