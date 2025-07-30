CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -IGPIO_I2C
TARGET = gpio_i2c_test
SOURCES = test.c GPIO_I2C/gpio_i2c.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

test: $(TARGET)
	sudo ./$(TARGET)

.PHONY: all clean install test
