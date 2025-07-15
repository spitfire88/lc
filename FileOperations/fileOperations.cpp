/*
    Write to multiple shared resources.
    1. file on disk
    2. shared memory

    Use memory barriers to maintain ordering.
    Fork thread that writes to the same shared resource in parallel.
*/
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "Buffer/DoubleBuffering/doubleCircularBuffer.cpp"
#include <cstring>
#include <thread>

#define DEVICE "temporary.txt"
#define BUFFER_SIZE 10

using namespace std;

int main() {

    Buffer<string> buffer(BUFFER_SIZE);

    int fd = open(DEVICE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        return 1;
    }

    size_t fileSize = sb.st_size;

    // Map the file into memory
    void *mapped = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mapped == nullptr) {
        perror("mmap");
        close(fd);
        return 1;
    }

    char *data = static_cast<char *>(mapped);
    cout << "File contents: \n";
    cout.write(data, fileSize);
    cout << endl;

    thread workerThread([&]() {
        cout << "Worker thread started \n";

        string fileContent(data, fileSize);
        buffer.push(fileContent);
        this_thread::sleep_for(chrono::milliseconds(1000));

        string result;
        buffer.pop(result);
        cout << "Worker thread finished with result: " << result << endl;
    });

    cout << "Main thread doing work\n";
    this_thread::sleep_for(chrono::milliseconds(100));
    string mainData = "This data is from main thread";
    string result;
    buffer.pop(result);
    cout << "Main thread popped with result: " << result << endl;
    buffer.push(mainData);
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Main thread finished with result: " << result << endl;

    workerThread.join();

    munmap(mapped, fileSize);
    close(fd);

    return 0;
}