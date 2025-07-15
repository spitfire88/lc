#include <atomic>

class mySpinlock {
private:
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT; // Atomic flag for the spinlock

public:
    void lock() {
        while (lock_flag.test_and_set(std::memory_order_acquire)) {
            // Spin until the lock is acquired
        }
    }

    void unlock() {
        lock_flag.clear(std::memory_order_release); // Release the lock
    }
};