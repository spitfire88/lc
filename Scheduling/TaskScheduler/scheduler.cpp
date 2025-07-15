/*
    Task Scheduler
    You are given an array of CPU tasks, each labeled with a letter from A to Z, and a number n. 
    Each CPU interval can be idle or allow the completion of one task. 
    Tasks can be completed in any order, but there's a constraint: there has to be a gap of at least 
    n intervals between two tasks with the same label.
    Return the minimum number of CPU intervals required to complete all tasks.

    Input: tasks = ["A","A","A","B","B","B"], n = 2
    Output: 8
    Explanation: A possible sequence is: A -> B -> idle -> A -> B -> idle -> A -> B.

    Approach:
    Priority Queue {Count, Task} = {3, A}, {3, B} 
    Time Queue {Count, Task, FutureTime} = 

    !pq.empty() 
        if (tq.empty())
            pq.first, decrement count, increment time, tq.push({count, time + n})
        else
            pq.push(tq.top())
    
*/

#include <queue>
#include <vector>
#include <iostream>

using namespace std;

class Scheduler {
public:
    int minInterval(vector<char>& tasks, int n) {
        vector<int> countTasks (26, 0);
        for( char c : tasks) {
            // {0, count}, ...
            countTasks[c - 'A']++;
        }

        priority_queue<vector<int>> pq;
        for(int i = 0; i < 26; i++) {
            if(countTasks[i] > 0) {
                // {count, 65}, {count, 66}, ..
                pq.push(vector<int>{countTasks[i], i + 'A'});
            }
        }

        int epoch = 0;
        string result;
        queue<vector<int>> tq;

        while(!pq.empty() || !tq.empty()) {
            epoch++;
            if(pq.empty()) {
                for(; epoch < tq.front()[0]; epoch++) {
                    result += "idle";
                }
            } else {
                auto tup = pq.top();
                pq.pop();
                result += (char)tup[1];
                if(--tup[0] > 0) {
                    tq.emplace(vector<int>{n + epoch, tup[0], tup[1]});
                }
            }

            if(!tq.empty() && tq.front()[0] == epoch) {
                pq.push(vector<int>{tq.front()[1], tq.front()[2]});
                tq.pop();
            }
        }

        cout << "Resulting schedule sequence is " << result << endl;
        return epoch;  
    }
};

int main() {
    vector<char> t = {'A', 'A', 'A', 'B', 'B', 'B'}; 
    int n = 2;

    Scheduler s;
    cout << "scheduler min Interval is " << s.minInterval(t, n);
    return 0;
}