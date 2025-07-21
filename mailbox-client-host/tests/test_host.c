#include <stdio.h>
#include <stdlib.h>
#include "mailbox_host.h"
#include "mailbox_common.h"

void test_init_host() {
    // Test host initialization
    if (init_host() != 0) {
        printf("Host initialization failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Host initialized successfully\n");
}

void test_handle_message() {
    // Test handling a message
    struct mailbox_message msg;
    msg.type = MESSAGE_TYPE_TEST;
    msg.data = "Test message";

    if (handle_message(&msg) != 0) {
        printf("Failed to handle message\n");
        exit(EXIT_FAILURE);
    }
    printf("Message handled successfully\n");
}

int main() {
    test_init_host();
    test_handle_message();
    return 0;
}