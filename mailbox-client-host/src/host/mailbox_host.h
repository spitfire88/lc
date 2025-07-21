#ifndef MAILBOX_HOST_H
#define MAILBOX_HOST_H

#include "../common/mailbox_common.h"

// Function to initialize the host
void init_host(void);

// Function to handle incoming messages
void handle_message(const struct mailbox_message *msg);

// Function to start the host and begin processing messages
void start_host(void);

#endif // MAILBOX_HOST_H