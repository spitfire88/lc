/*
Implement a round-robin scheduler that:
			□ Accepts a list of processes with burst and arrival times.
			□ Simulates execution using a fixed time quantum.
			□ Tracks and outputs each process's:
			□ Completion time
			□ Turnaround time (completion - arrival)
			□ Waiting time (turnaround - burst)
Expected output quantum 4
Process Arrival Burst   Completion      Turnaround      Waiting
P1      0       10      23              23              13
P2      1       5       17              16              11
P3      2       8       21              19              11

*/

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;


// Create a Process that needs all the values, and write a constructor
struct Process {
    int id;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    
    Process(int _id, int _arrival, int _burst) : 
        id(_id), arrival(_arrival), burst(_burst), 
        remaining(_burst), completion(0), turnaround(0), waiting(0) {}
};

class RoundRobinScheduler {
private:
    // vector of Process that a method adds to
    vector<Process> processes;
    // queue of ready processes that havent completed
    queue<int> readyQueue; // Store process indices
    int currentTime;
    int quantum;
    
public:
    RoundRobinScheduler(int timeQuantum) : currentTime(0), quantum(timeQuantum) {}
    
    void addProcess(int id, int arrival, int burst) {
        processes.emplace_back(id, arrival, burst);
    }
    
    void schedule() {
        // akin to vector<int>countOfProcs (26, 0)
        vector<bool> inQueue(processes.size(), false);
        int completed = 0;
        int n = processes.size();
        
        while (completed < n) {
            // Add newly arrived processes to ready queue
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival <= currentTime && !inQueue[i] && processes[i].remaining > 0) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }
            
            if (!readyQueue.empty()) {
                int currentProcess = readyQueue.front();
                readyQueue.pop();
                inQueue[currentProcess] = false;
                
                // Execute process for quantum time or remaining time (whichever is smaller)
                int executeTime = min(quantum, processes[currentProcess].remaining);
                processes[currentProcess].remaining -= executeTime;
                currentTime += executeTime;
                
                // Check for newly arrived processes during execution
                for (int i = 0; i < n; i++) {
                    if (processes[i].arrival <= currentTime && !inQueue[i] && 
                        processes[i].remaining > 0 && i != currentProcess) {
                        readyQueue.push(i);
                        inQueue[i] = true;
                    }
                }
                
                // If process is completed
                if (processes[currentProcess].remaining == 0) {
                    processes[currentProcess].completion = currentTime;
                    processes[currentProcess].turnaround = processes[currentProcess].completion - processes[currentProcess].arrival;
                    processes[currentProcess].waiting = processes[currentProcess].turnaround - processes[currentProcess].burst;
                    completed++;
                } else {
                    // Add back to queue if not completed
                    readyQueue.push(currentProcess);
                    inQueue[currentProcess] = true;
                }
            } else {
                // No process ready, increment time
                currentTime++;
            }
        }
    }
    
    void printResults() {
        cout << "Process Arrival Burst   Completion      Turnaround      Waiting\n";
        for (const auto& p : processes) {
            cout << "P" << p.id << "      " 
                 << setw(2) << p.arrival << "      "
                 << setw(2) << p.burst << "      "
                 << setw(2) << p.completion << "              "
                 << setw(2) << p.turnaround << "              "
                 << setw(2) << p.waiting << "\n";
        }
    }
};

int main() {
    RoundRobinScheduler scheduler(4); // Quantum = 4
    
    // Add processes: P1(0,10), P2(1,5), P3(2,8)
    scheduler.addProcess(1, 0, 10);
    scheduler.addProcess(2, 1, 5);
    scheduler.addProcess(3, 2, 8);
    
    scheduler.schedule();
    scheduler.printResults();
    
    return 0;
}