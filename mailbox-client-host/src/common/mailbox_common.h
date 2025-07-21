#ifndef MAILBOX_COMMON_H
#define MAILBOX_COMMON_H

#define MAX_MESSAGE_SIZE 256
#define MAX_MESSAGES 10

typedef enum {
    MSG_TYPE_TEXT,
    MSG_TYPE_BINARY,
    MSG_TYPE_CONTROL
} message_type_t;

typedef struct {
    message_type_t type;
    char data[MAX_MESSAGE_SIZE];
} message_t;

typedef enum {
    STATUS_OK,
    STATUS_ERROR,
    STATUS_FULL,
    STATUS_EMPTY
} mailbox_status_t;

#endif // MAILBOX_COMMON_H