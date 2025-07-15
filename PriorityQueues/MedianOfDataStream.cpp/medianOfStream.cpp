/*
The median is the middle value in an ordered integer list. If the size of the list is even, there is no middle value, and the median is the mean of the two middle values.

For example, for arr = [2,3,4], the median is 3.
For example, for arr = [2,3], the median is (2 + 3) / 2 = 2.5.
Implement the MedianFinder class:

MedianFinder() initializes the MedianFinder object.
void addNum(int num) adds the integer num from the data stream to the data structure.
double findMedian() returns the median of all elements so far. Answers within 10-5 of the actual answer will be accepted.
 

Example 1:

Input
["MedianFinder", "addNum", "addNum", "findMedian", "addNum", "findMedian"]
[[], [1], [2], [], [3], []]
Output
[null, null, null, 1.5, null, 2.0]

Explanation
MedianFinder medianFinder = new MedianFinder();
medianFinder.addNum(1);    // arr = [1]
medianFinder.addNum(2);    // arr = [1, 2]
medianFinder.findMedian(); // return 1.5 (i.e., (1 + 2) / 2)
medianFinder.addNum(3);    // arr[1, 2, 3]
medianFinder.findMedian(); // return 2.0
 

Constraints:

-105 <= num <= 105
There will be at least one element in the data structure before calling findMedian.
At most 5 * 104 calls will be made to addNum and findMedian.
*/

#include <queue>
#include <vector>
#include <iostream>

using namespace std;

class MedianFinder {
private:
    priority_queue<int> maxHeap; // low half
    priority_queue<int, vector<int>, greater<int>> minHeap; // upper half

public:
    MedianFinder() {}
    void addNum(int num) {
        // add to MaxHeap first
        if(maxHeap.empty() || maxHeap.top() > num) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        // Balance the heaps: maxHeap(elements) <= minHeap(elements)
        if(maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size() + 1) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        if(maxHeap.size() == minHeap.size()) {
            if(maxHeap.empty()) {
                return -1;
            } else {
                double avg = (minHeap.top() + maxHeap.top()) >> 1;
                return avg;
            }
        } else {
            return maxHeap.size() > minHeap.size() ? maxHeap.top() : minHeap.top();
        }
    }
};

int main() {
    string input;
    MedianFinder* med = new MedianFinder();

    while(1) {
        cout << "Enter a positive number or M to get median: ";
        cin >> input;

        if(input == "M") {
            cout << "Median is: " << med->findMedian() << endl;
        } else {
            try {
                int val = stoi(input);
                med->addNum(val);
                cout << "Added: " << val << endl;
            } catch(invalid_argument&) {
                cout << "Invalid input!" << endl;
            }
        }
    }
    return 0;
}