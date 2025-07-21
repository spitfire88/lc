#include <stdio.h>
#include <stdlib.h>
#include "mailbox_client.h"

void test_send_message() {
    // Test sending a message
    const char *message = "Hello, Host!";
    int result = send_message(message);
    if (result == 0) {
        printf("Message sent successfully: %s\n", message);
    } else {
        printf("Failed to send message: %d\n", result);
    }
}

void test_receive_message() {
    // Test receiving a message
    char buffer[256];
    int result = receive_message(buffer, sizeof(buffer));
    if (result >= 0) {
        printf("Message received: %s\n", buffer);
    } else {
        printf("Failed to receive message: %d\n", result);
    }
}

int main() {
    // Initialize the client
    if (init_client() != 0) {
        fprintf(stderr, "Failed to initialize client\n");
        return EXIT_FAILURE;
    }

    // Run tests
    test_send_message();
    test_receive_message();

    return EXIT_SUCCESS;
}