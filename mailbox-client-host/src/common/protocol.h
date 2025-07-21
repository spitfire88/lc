#ifndef PROTOCOL_H
#define PROTOCOL_H

// Define message types
#define MESSAGE_TYPE_TEXT 1
#define MESSAGE_TYPE_BINARY 2

// Define command identifiers
#define CMD_SEND_MESSAGE 100
#define CMD_RECEIVE_MESSAGE 101

// Define a structure for messages
struct mailbox_message {
    int type;          // Type of the message (text, binary, etc.)
    int length;       // Length of the message data
    char *data;       // Pointer to the message data
};

// Function prototypes for protocol handling
void init_protocol();
void send_message(struct mailbox_message *msg);
struct mailbox_message *receive_message();

#endif // PROTOCOL_H