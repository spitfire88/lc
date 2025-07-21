#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mailbox_client.h"
#include "mailbox_common.h"
#include "protocol.h"

static int mailbox_fd;

int init_client(const char *mailbox_name) {
    // Initialize the mailbox client
    mailbox_fd = open(mailbox_name, O_RDWR);
    if (mailbox_fd < 0) {
        perror("Failed to open mailbox");
        return -1;
    }
    return 0;
}

int send_message(const char *message) {
    // Send a message to the mailbox
    ssize_t bytes_written = write(mailbox_fd, message, strlen(message));
    if (bytes_written < 0) {
        perror("Failed to send message");
        return -1;
    }
    return 0;
}

int receive_message(char *buffer, size_t buffer_size) {
    // Receive a message from the mailbox
    ssize_t bytes_read = read(mailbox_fd, buffer, buffer_size - 1);
    if (bytes_read < 0) {
        perror("Failed to receive message");
        return -1;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the received message
    return 0;
}

void close_client() {
    // Close the mailbox client
    if (mailbox_fd >= 0) {
        close(mailbox_fd);
    }
}