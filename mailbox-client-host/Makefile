CC = gcc
CFLAGS = -Iinclude -Isrc/common -Isrc/utils -Wall -Wextra
SRC = $(wildcard src/client/*.c) $(wildcard src/host/*.c) $(wildcard src/utils/*.c) $(wildcard drivers/*.c)
OBJ = $(SRC:.c=.o)
TARGET = mailbox_client_host

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean