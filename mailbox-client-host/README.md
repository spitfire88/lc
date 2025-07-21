# Mailbox Client-Host Communication Framework

This project implements a client-host communication framework based on the Common Mailbox Framework in the Linux Kernel. It allows for efficient message passing between a client and a host using a defined protocol.

## Project Structure

- **src/**: Contains the source code for the client, host, common definitions, and utility functions.
  - **client/**: Implements the client-side functionality.
    - `mailbox_client.c`: Client implementation.
    - `mailbox_client.h`: Client header file.
  - **host/**: Implements the host-side functionality.
    - `mailbox_host.c`: Host implementation.
    - `mailbox_host.h`: Host header file.
  - **common/**: Contains common definitions and protocol specifications.
    - `mailbox_common.h`: Common data structures and definitions.
    - `protocol.h`: Communication protocol definitions.
  - **utils/**: Utility functions for debugging.
    - `debug.c`: Debugging utility implementation.
    - `debug.h`: Debugging utility header file.
  
- **drivers/**: Contains the mailbox driver that interfaces with the Linux kernel.
  - `mailbox_driver.c`: Mailbox driver implementation.

- **include/**: Public API for the mailbox system.
  - `mailbox_api.h`: API declarations for mailbox operations.

- **tests/**: Contains test cases for both client and host functionalities.
  - `test_client.c`: Tests for client functionality.
  - `test_host.c`: Tests for host functionality.

- **Makefile**: Build instructions for compiling the project.

- **Kconfig**: Configuration options for the kernel module.

## Setup Instructions

1. **Clone the repository**:
   ```
   git clone <repository-url>
   cd mailbox-client-host
   ```

2. **Build the project**:
   ```
   make
   ```

3. **Load the mailbox driver**:
   ```
   sudo insmod mailbox_driver.ko
   ```

4. **Run the client and host applications**:
   ```
   ./src/client/mailbox_client
   ./src/host/mailbox_host
   ```

## Usage

The client can send messages to the host, and the host can respond back. The communication is handled through the mailbox driver, which utilizes the Linux Kernel's Common Mailbox Framework.

## Overview

This framework is designed to facilitate inter-process communication in a Linux environment, leveraging the efficiency and reliability of the kernel's mailbox system. The client and host communicate using a defined protocol, ensuring that messages are correctly formatted and processed.