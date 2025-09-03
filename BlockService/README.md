# BlockService

A C++ shared library for block device management on Ubuntu (BareMetal OS).

## Features
- `GetMountedInfo`: List block devices and their mount status (like `lsblk`).
- `GetDevice(size_t size_bytes, std::string fs_type)`: Find an unmounted `/dev/sd*` device, partition it, create a filesystem, and return the device name.

## Build
```
mkdir build && cd build
cmake ..
make
```

## Usage
Link your test program with `libBlockService.so` and include `BlockService.h`.

**Note:** Requires root privileges for partitioning and formatting.
