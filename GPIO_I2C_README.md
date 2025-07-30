# GPIO I2C Bitbanging Implementation

This project implements a GPIO bitbanging I2C master interface for Linux systems using the sysfs GPIO interface.

## Features

- Master-only I2C implementation
- Bitbanging using two GPIO pins (SDA and SCL)
- Standard I2C protocol with START/STOP conditions
- ACK/NACK detection and handling
- Configurable timing for different I2C speeds
- Error handling and diagnostics

## Hardware Setup

### GPIO Pins
- **SDA (Data)**: GPIO2 (configurable in `gpio_i2c.h`)
- **SCL (Clock)**: GPIO3 (configurable in `gpio_i2c.h`)

### Connections
```
Master (Linux System)    I2C Slave Device
GPIO2 (SDA) ------------ SDA
GPIO3 (SCL) ------------ SCL
GND --------------------- GND
VCC --------------------- VCC (3.3V or 5V)
```

### Pull-up Resistors
Add 4.7kΩ pull-up resistors on both SDA and SCL lines to VCC.

## Building

```bash
# Compile the project
make

# Clean build files
make clean

# Install to system (optional)
make install
```

## Usage

```bash
# Run the test (requires root permissions for GPIO access)
sudo ./gpio_i2c_test

# Or use the make target
make test
```

## Configuration

Edit `GPIO_I2C/gpio_i2c.h` to configure:

- **GPIO pins**: Change `SDA_PIN` and `SCL_PIN` values
- **I2C timing**: Adjust `I2C_DELAY_US` for different speeds
  - 5µs = ~100kHz (standard mode)
  - 1.25µs = ~400kHz (fast mode)
- **Slave address**: Modify `SLAVE_ADDR` in `test.c`

## API Functions

### Initialization
- `int gpio_init(void)` - Initialize GPIO pins and I2C interface
- `void gpio_cleanup(void)` - Clean up GPIO resources

### I2C Operations
- `int i2c_write(unsigned char slave_addr, unsigned char* data, int len)` - Write data to slave
- `int i2c_read(unsigned char slave_addr, unsigned char* data, int len)` - Read data from slave

### Low-level Functions
- `void i2c_start(void)` - Generate I2C START condition
- `void i2c_stop(void)` - Generate I2C STOP condition
- `int i2c_send_byte(unsigned char byte)` - Send one byte and read ACK
- `unsigned char i2c_read_byte(int ack)` - Read one byte and send ACK/NACK

## Example Usage

```c
#include "GPIO_I2C/gpio_i2c.h"

int main() {
    unsigned char data[] = {0x10, 0x20, 0x30};
    
    if (gpio_init() != 0) {
        printf("GPIO init failed\n");
        return -1;
    }
    
    // Write to I2C slave at address 0x48
    if (i2c_write(0x48, data, sizeof(data)) == 0) {
        printf("Write successful\n");
    }
    
    gpio_cleanup();
    return 0;
}
```

## Troubleshooting

### Permission Denied
- Run with `sudo` - GPIO access requires root permissions
- Check if GPIO pins are already exported by another process

### Communication Failures
- Verify hardware connections and pull-up resistors
- Check slave device address (use `i2cdetect` if available)
- Ensure correct voltage levels (3.3V vs 5V)
- Try slower timing by increasing `I2C_DELAY_US`

### GPIO Export Errors
- Some GPIO pins may not be available on your system
- Change `SDA_PIN` and `SCL_PIN` to available GPIO numbers
- Check `/sys/class/gpio/` for available GPIOs

## Compatible Devices

This implementation should work with any I2C slave device, including:
- Temperature sensors (DS18B20, LM75, etc.)
- EEPROMs (24C02, 24C64, etc.)
- Real-time clocks (DS1307, PCF8563, etc.)
- GPIO expanders (PCF8574, MCP23017, etc.)

## License

This project is provided as-is for educational and development purposes.
