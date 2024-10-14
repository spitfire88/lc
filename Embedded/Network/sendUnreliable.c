/*
    Write a copilot program that uses the following network APIs:
    struct network_ops {
        Network APIs
        int (*send_unreliable)(int con_id, const char *buf, size_t len);

        Events
        void (*on_receive)(int con_id, const char *buf, size_t len);
        int (*on_open)(int con_id);
        int (*on_close)(int con_id, int reason);
    };

    Create a message packet that contains a header, sequence number
    Receive the 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 10
#define PACKET_SIZE 32
int sequence_number = 0;

typedef struct message_t {
    char header[HEADER_SIZE];
    char data[PACKET_SIZE];
    struct message_t *next;
} message_t;

struct network_ops {
    int (*send_unreliable)(int con_id, const char *buf, size_t len);
    void (*on_receive)(int con_id, const char *buf, size_t len);
    int (*on_open)(int con_id);
    int (*on_close)(int con_id, int reason);
};

void on_receive(int con_id, const char *buf, size_t len) {
    printf("Received: %s\n", buf);
}

int on_open(int con_id) {
    printf("Connection opened\n");
    return 0;
}

int on_close(int con_id, int reason) {
    printf("Connection closed\n");
    return 0;
}

message_t *create_message(message_t *msg, const char *buf, size_t len) {
    message_t *new_msg = (message_t *)malloc(sizeof(message_t));
    if (new_msg == NULL) {
        return NULL;
    }

    snprintf(new_msg->header, HEADER_SIZE, "HDR%04d", sequence_number++);
    strncpy(new_msg->data, buf, PACKET_SIZE);
    new_msg->next = NULL;

    return new_msg;
}

int send_unreliable(int con_id, const char *buf, size_t len) {
    message_t *msg, *temp;

    msg = create_message(msg, "HEAD", 4);
    for (int i = 0; (i*PACKET_SIZE) < len; i++) {
        temp = create_message(msg, buf + (i*PACKET_SIZE), PACKET_SIZE);
        if (msg == NULL) {
            return -1;
        }
        msg->next = temp;
        msg = msg->next;
        
    }
    return 0;
}

int main() {
    struct network_ops ops = {
        .send_unreliable = send_unreliable,
        .on_receive = on_receive,
        .on_open = on_open,
        .on_close = on_close
    };

    ops.on_open(1);
    ops.send_unreliable(1, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ", 5*26);
    ops.on_receive(1, "World", 5);
    ops.on_close(1, 0);

    return 0;
}