CC = gcc
CFLAGS = -Wall -Wextra -O3 -mavx -mavx2 -std=c99
TARGET = memset_test
SOURCE = memset.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

test: $(TARGET)
	./$(TARGET)

debug: $(SOURCE)
	$(CC) $(CFLAGS) -g -DDEBUG -o $(TARGET)_debug $(SOURCE)

clean:
	rm -f $(TARGET) $(TARGET)_debug *.o

benchmark: $(TARGET)
	@echo "Running memset benchmark..."
	./$(TARGET)

# Check CPU features
cpuinfo:
	@echo "CPU Information:"
	@grep -E "(model name|flags)" /proc/cpuinfo | head -2
	@echo ""
	@echo "AVX Support:"
	@grep -o "avx[^ ]*" /proc/cpuinfo | sort | uniq || echo "No AVX support found"

.PHONY: all test debug clean benchmark cpuinfo
