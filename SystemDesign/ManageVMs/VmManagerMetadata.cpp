/*
You are designing a system service in the Android Framework to manage virtual machines (VMs) launched from Android. 
This registry will be part of a virtualization-aware Android system and must:

1. Register a new VM with metadata:
			VM ID (string)
			Owner UID (int)
			VM state (enum: RUNNING, STOPPED, PAUSED)
			Creation timestamp

2. Query the status of a VM by user ID and VM ID.

3. List all VMs for a given Android user.

4. Remove a VM by ID.

5. Track VM creation order using a doubly linked list per user.
			This should allow efficient traversal in creation order.
			It should support future use cases like LRU eviction or timeout-based cleanup.

6. Ensure thread safety for concurrent access.

7. Be extensible for future features like:
			Resource usage tracking (CPU, memory)
			IPC routing to/from VMs
            Persistent storage of VM metadata
*/
#include <iostream>
#include <chrono>
#include <unordered_map>
using namespace std;

enum class VmState {
    RUNNING,
    STOPPED,
    PAUSED,
    UNDEFINED,
};

class VmMetadata {
private:
    string vm_id;
    int own_uid;
    VmState state;
    time_t start_timestamp;
    VmMetadata *next;
    VmMetadata *prev;

public:
    VmMetadata(string name, int uid) : vm_id(name), own_uid(uid), state(VmState::UNDEFINED), start_timestamp(0), next(nullptr), prev(nullptr) {}

    void VmStart(int uid) {
        if(uid != own_uid ) 
            perror("UIDs do not match!");
        
        auto now = chrono::system_clock::now();
        start_timestamp = chrono::system_clock::to_time_t(now);
        VmStateUpdate(uid, VmState::RUNNING);
    }

    void VmStateUpdate(int uid, VmState s) {
        if(uid == own_uid) {
            state = s;
        }
    }
};

class VmManager {
private:
    unordered_map<int, VmMetadata *> map;
    VmMetadata *head; // most recently added
    VmMetadata *tail; // least recently added

    void add_to_front(VmMetadata *node) {}
    void delete_node(VmMetadata *node) {}

public:
    
};
