// mailbox_client.h - Header file for the mailbox client

#ifndef MAILBOX_CLIENT_H
#define MAILBOX_CLIENT_H

#include "mailbox_common.h"

// Function to initialize the mailbox client
int init_client(const char *client_name);

// Function to send a message to the host
int send_message(const char *message, size_t length);

// Function to receive a message from the host
int receive_message(char *buffer, size_t buffer_length);

#endif // MAILBOX_CLIENT_H