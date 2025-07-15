#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
using namespace std;
template<typename T>
class Buffer {
private:
    vector<T> primary_buffer;
    vector<T> secondary_buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    bool using_primary;

    mutable mutex buffer_mutex;
    mutable condition_variable write_complete_cv;

public:
    explicit Buffer(size_t size) 
        : primary_buffer(size), secondary_buffer(size), capacity(size),
          head(0), tail(0), count(0), using_primary(true) {
        if(size == 0) {
            throw invalid_argument("Buffer size ust be greater than 0");
        }
    }

    // Get reference to current active buffer
    vector<T>& get_active_buffer() {
        lock_guard<mutex> lock(buffer_mutex);
        return using_primary ? primary_buffer : secondary_buffer;
    }

    // Get reference to inactive buffer
    vector<T>& get_inactive_buffer() {
        lock_guard<mutex> lock(buffer_mutex);
        return using_primary ? secondary_buffer : primary_buffer;
    }

    // Swap active and inactive buffers
    void swap_buffers() {
        lock_guard<mutex> lock(buffer_mutex);
        using_primary = !using_primary;
        // reset circular buffer state
        head = 0;
        tail = 0;
        count = 0;
        // Notify waiting threads that write is complete
        write_complete_cv.notify_all();
    }

    bool push(const T& item) {
        lock_guard<mutex> lock(buffer_mutex);
        auto& active = using_primary ? primary_buffer : secondary_buffer;

        if(count == capacity) {
            // Buffer is full, overwrite oldest element
            head = (head + 1) % capacity;
        } else {
            count++;
        }

        active[tail] = item;
        tail = (tail + 1) % capacity;

        write_complete_cv.notify_all();
        return true;
    }

    bool pop(T& item) {
        lock_guard<mutex> lock(buffer_mutex);
        if (count == 0) {
            return false;
        }

        auto& active = using_primary ? primary_buffer : secondary_buffer;
        item = active[head];
        head = (head + 1) % capacity;
        count--;

        // Notify waiting threads that write is complete
        write_complete_cv.notify_all();
        return true;
    }
    // Peek at front element without removing
    bool front(T& item) const {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        if (count == 0) {
            return false;
        }
        
        const auto& active = using_primary ? primary_buffer : secondary_buffer;
        item = active[head];
        return true;
    }

    // Check if buffer is empty
    bool empty() const {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        return count == 0;
    }

    // Check if buffer is full
    bool full() const {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        return count == capacity;
    }

    // Get current size
    size_t size() const {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        return count;
    }

    // Get capacity (no lock needed - immutable)
    size_t get_capacity() const {
        return capacity;
    }

    // Clear the active buffer
    void clear() {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        head = 0;
        tail = 0;
        count = 0;
        // Notify waiting threads that write is complete
        write_complete_cv.notify_all();
    }

    // Copy data from inactive to active buffer
    void copy_from_inactive() {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        auto& active = using_primary ? primary_buffer : secondary_buffer;
        auto& inactive = using_primary ? secondary_buffer : primary_buffer;
        active = inactive;
        // Notify waiting threads that write is complete
        write_complete_cv.notify_all();
    }

    // Get which buffer is currently active
    bool is_using_primary() const {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        return using_primary;
    }

    // Wait for any ongoing writes to complete
    void wait_for_write_complete() const {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        // This will immediately return since we're using simple mutex
        // but provides interface for waiting if needed
        write_complete_cv.wait_for(lock, std::chrono::milliseconds(0));
    }

    // Notify all waiting threads (useful for external coordination)
    void notify_write_complete() {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        write_complete_cv.notify_all();
    }
};

/*
// Example usage
int main() {
    try {
        Buffer<int> buffer(5);
        
        std::cout << "Adding elements to buffer..." << std::endl;
        for (int i = 1; i <= 7; ++i) {
            buffer.push(i);
            std::cout << "Added: " << i << ", Size: " << buffer.size() << std::endl;
        }
        
        std::cout << "\nReading elements from active buffer:" << std::endl;
        int value;
        while (buffer.pop(value)) {
            std::cout << "Popped: " << value << std::endl;
        }
        
        std::cout << "\nSwapping buffers and adding new data..." << std::endl;
        buffer.swap_buffers();
        
        for (int i = 10; i <= 13; ++i) {
            buffer.push(i);
            std::cout << "Added to new buffer: " << i << std::endl;
        }
        
        std::cout << "\nActive buffer is " << (buffer.is_using_primary() ? "primary" : "secondary") << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
    */