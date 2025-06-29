#include <iostream>
#include <cstring>
#include <stdexcept>
#include <mutex> // Include mutex for thread safety

class Buffer {
public:
    static const size_t BUFFER_SIZE = 1024;

    Buffer() : head(0), tail(0) {
        std::memset(data, 0, BUFFER_SIZE);
    }

    void write(const char* input, size_t length) {
        if (length > availableSpace()) {
            throw std::overflow_error("Not enough space in buffer");
        }

        std::lock_guard<std::mutex> lock(mutex);
        for (size_t i = 0; i < length; ++i) {
            data[head] = input[i];
            head = (head + 1) % BUFFER_SIZE;
        }
    }

    void read(char* output, size_t length) {
        if (length > usedSpace()) {
            throw std::underflow_error("Not enough data in buffer");
        }
        
        std::lock_guard<std::mutex> lock(mutex);
        for (size_t i = 0; i < length; ++i) {
            output[i] = data[tail];
            tail = (tail + 1) % BUFFER_SIZE;
        }
    }

    size_t usedSpace() const {
        return (head >= tail) ? (head - tail) : (BUFFER_SIZE - tail + head);
    }

    size_t availableSpace() const {
        return BUFFER_SIZE - usedSpace() - 1; // -1 for the circular buffer nature
    }

private:
    char data[BUFFER_SIZE];
    size_t head;
    size_t tail;
    mutable std::mutex mutex; // Mutex for thread safety
};

int main() {
    Buffer buffer;

    const char* message = "Hello, Circular Buffer!";
    buffer.write(message, std::strlen(message));

    char output[50];
    buffer.read(output, std::strlen(message));
    output[std::strlen(message)] = '\0'; // Null-terminate the string

    std::cout << "Read from buffer: " << output << std::endl;

    return 0;
}