#ifndef GPIO_I2C_H
#define GPIO_I2C_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// GPIO pin definitions
#define SDA_PIN 2  // GPIO2 for data line
#define SCL_PIN 3  // GPIO3 for clock line

// I2C timing (microseconds)
#define I2C_DELAY_US 5  // 100kHz clock

// I2C protocol defines
#define I2C_ACK 0
#define I2C_NACK 1
#define I2C_READ 1
#define I2C_WRITE 0

// Function prototypes
int gpio_init(void);
void gpio_cleanup(void);
void gpio_set_pin(int pin, int value);
int gpio_get_pin(int pin);
void gpio_set_pin_direction(int pin, const char* direction);

// I2C protocol functions
void i2c_start(void);
void i2c_stop(void);
void i2c_send_bit(int bit);
int i2c_read_bit(void);
int i2c_send_byte(unsigned char byte);
unsigned char i2c_read_byte(int ack);
int i2c_write(unsigned char slave_addr, unsigned char* data, int len);
int i2c_read(unsigned char slave_addr, unsigned char* data, int len);

#endif
