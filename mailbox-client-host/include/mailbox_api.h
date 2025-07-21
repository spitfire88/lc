#ifndef MAILBOX_API_H
#define MAILBOX_API_H

#ifdef __cplusplus
extern "C" {
#endif

// Function to initialize the mailbox system
int mailbox_init(void);

// Function to send a message through the mailbox
int mailbox_send(const char *message);

// Function to receive a message from the mailbox
int mailbox_receive(char *buffer, size_t buffer_size);

// Function to clean up the mailbox system
void mailbox_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif // MAILBOX_API_H