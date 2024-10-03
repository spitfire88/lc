#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    struct pollfd fds[1];
    int timeout_msecs = 5000; // 5 seconds timeout
    int ret;

    // Open a file descriptor for reading
    int fd = open("example.txt", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    fds[0].fd = fd;
    fds[0].events = POLLIN; // Check for data to read

    printf("Waiting for data...\n");
    ret = poll(fds, 1, timeout_msecs);

    if (ret == -1) {
        perror("poll");
        return 1;
    } else if (ret == 0) {
        printf("Timeout occurred! No data within %d milliseconds.\n", timeout_msecs);
    } else {
        if (fds[0].revents & POLLIN) {
            printf("Data is available to read.\n");
        }
    }

    close(fd);
    return 0;
}