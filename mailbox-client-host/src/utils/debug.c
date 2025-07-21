#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

void log_message(const char *message) {
    FILE *log_file = fopen("mailbox_log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }
    fprintf(log_file, "%s\n", message);
    fclose(log_file);
}

void log_error(const char *error_message) {
    fprintf(stderr, "Error: %s\n", error_message);
    log_message(error_message);
}