#include "gpio_i2c.h"

static int gpio_fd_sda = -1;
static int gpio_fd_scl = -1;
static int gpio_export_fd = -1;

// GPIO sysfs paths
static char sda_value_path[64];
static char scl_value_path[64];
static char sda_direction_path[64];
static char scl_direction_path[64];

int gpio_init(void) {
    char buffer[64];
    
    // Export GPIO pins
    gpio_export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if (gpio_export_fd == -1) {
        perror("Failed to open GPIO export");
        return -1;
    }
    
    // Export SDA pin
    snprintf(buffer, sizeof(buffer), "%d", SDA_PIN);
    write(gpio_export_fd, buffer, strlen(buffer));
    
    // Export SCL pin
    snprintf(buffer, sizeof(buffer), "%d", SCL_PIN);
    write(gpio_export_fd, buffer, strlen(buffer));
    
    close(gpio_export_fd);
    
    // Setup file paths
    snprintf(sda_value_path, sizeof(sda_value_path), "/sys/class/gpio/gpio%d/value", SDA_PIN);
    snprintf(scl_value_path, sizeof(scl_value_path), "/sys/class/gpio/gpio%d/value", SCL_PIN);
    snprintf(sda_direction_path, sizeof(sda_direction_path), "/sys/class/gpio/gpio%d/direction", SDA_PIN);
    snprintf(scl_direction_path, sizeof(scl_direction_path), "/sys/class/gpio/gpio%d/direction", SCL_PIN);
    
    // Small delay for sysfs to create files
    usleep(100000);
    
    // Set pin directions (SCL is output, SDA can be input/output)
    gpio_set_pin_direction(SCL_PIN, "out");
    gpio_set_pin_direction(SDA_PIN, "out");
    
    // Initialize both pins to high (idle state)
    gpio_set_pin(SCL_PIN, 1);
    gpio_set_pin(SDA_PIN, 1);
    
    printf("GPIO I2C initialized - SDA: GPIO%d, SCL: GPIO%d\n", SDA_PIN, SCL_PIN);
    return 0;
}

void gpio_cleanup(void) {
    char buffer[64];
    int unexport_fd;
    
    // Unexport GPIO pins
    unexport_fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (unexport_fd != -1) {
        snprintf(buffer, sizeof(buffer), "%d", SDA_PIN);
        write(unexport_fd, buffer, strlen(buffer));
        
        snprintf(buffer, sizeof(buffer), "%d", SCL_PIN);
        write(unexport_fd, buffer, strlen(buffer));
        
        close(unexport_fd);
    }
    
    printf("GPIO I2C cleanup complete\n");
}

void gpio_set_pin_direction(int pin, const char* direction) {
    char path[64];
    int fd;
    
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd != -1) {
        write(fd, direction, strlen(direction));
        close(fd);
    }
}

void gpio_set_pin(int pin, int value) {
    char path[64];
    char val_str[2];
    int fd;
    
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (fd != -1) {
        snprintf(val_str, sizeof(val_str), "%d", value ? 1 : 0);
        write(fd, val_str, 1);
        close(fd);
    }
}

int gpio_get_pin(int pin) {
    char path[64];
    char value;
    int fd;
    
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (fd != -1) {
        read(fd, &value, 1);
        close(fd);
        return (value == '1') ? 1 : 0;
    }
    return 0;
}

// I2C Start condition: SDA goes low while SCL is high
void i2c_start(void) {
    gpio_set_pin_direction(SDA_PIN, "out");
    gpio_set_pin(SDA_PIN, 1);
    gpio_set_pin(SCL_PIN, 1);
    usleep(I2C_DELAY_US);
    
    gpio_set_pin(SDA_PIN, 0);  // SDA low while SCL high = START
    usleep(I2C_DELAY_US);
    gpio_set_pin(SCL_PIN, 0);
    usleep(I2C_DELAY_US);
}

// I2C Stop condition: SDA goes high while SCL is high
void i2c_stop(void) {
    gpio_set_pin_direction(SDA_PIN, "out");
    gpio_set_pin(SDA_PIN, 0);
    gpio_set_pin(SCL_PIN, 0);
    usleep(I2C_DELAY_US);
    
    gpio_set_pin(SCL_PIN, 1);
    usleep(I2C_DELAY_US);
    gpio_set_pin(SDA_PIN, 1);  // SDA high while SCL high = STOP
    usleep(I2C_DELAY_US);
}

void i2c_send_bit(int bit) {
    gpio_set_pin_direction(SDA_PIN, "out");
    gpio_set_pin(SDA_PIN, bit);
    usleep(I2C_DELAY_US);
    
    gpio_set_pin(SCL_PIN, 1);  // Clock high
    usleep(I2C_DELAY_US);
    gpio_set_pin(SCL_PIN, 0);  // Clock low
    usleep(I2C_DELAY_US);
}

int i2c_read_bit(void) {
    int bit;
    
    gpio_set_pin_direction(SDA_PIN, "in");  // Set SDA as input
    usleep(I2C_DELAY_US);
    
    gpio_set_pin(SCL_PIN, 1);  // Clock high
    usleep(I2C_DELAY_US);
    bit = gpio_get_pin(SDA_PIN);  // Read SDA
    gpio_set_pin(SCL_PIN, 0);  // Clock low
    usleep(I2C_DELAY_US);
    
    return bit;
}

int i2c_send_byte(unsigned char byte) {
    int i, ack;
    
    // Send 8 bits, MSB first
    for (i = 7; i >= 0; i--) {
        i2c_send_bit((byte >> i) & 1);
    }
    
    // Read ACK bit
    ack = i2c_read_bit();
    return ack;
}

unsigned char i2c_read_byte(int ack) {
    unsigned char byte = 0;
    int i;
    
    // Read 8 bits, MSB first
    for (i = 7; i >= 0; i--) {
        if (i2c_read_bit()) {
            byte |= (1 << i);
        }
    }
    
    // Send ACK/NACK
    i2c_send_bit(ack);
    
    return byte;
}

int i2c_write(unsigned char slave_addr, unsigned char* data, int len) {
    int i;
    
    i2c_start();
    
    // Send slave address with write bit (0)
    if (i2c_send_byte((slave_addr << 1) | I2C_WRITE) != I2C_ACK) {
        printf("Slave address NACK\n");
        i2c_stop();
        return -1;
    }
    
    // Send data bytes
    for (i = 0; i < len; i++) {
        if (i2c_send_byte(data[i]) != I2C_ACK) {
            printf("Data byte %d NACK\n", i);
            i2c_stop();
            return -1;
        }
    }
    
    i2c_stop();
    return 0;
}

int i2c_read(unsigned char slave_addr, unsigned char* data, int len) {
    int i;
    
    i2c_start();
    
    // Send slave address with read bit (1)
    if (i2c_send_byte((slave_addr << 1) | I2C_READ) != I2C_ACK) {
        printf("Slave address NACK\n");
        i2c_stop();
        return -1;
    }
    
    // Read data bytes
    for (i = 0; i < len; i++) {
        if (i == len - 1) {
            data[i] = i2c_read_byte(I2C_NACK);  // NACK on last byte
        } else {
            data[i] = i2c_read_byte(I2C_ACK);   // ACK on other bytes
        }
    }
    
    i2c_stop();
    return 0;
}
