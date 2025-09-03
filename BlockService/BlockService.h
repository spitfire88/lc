#pragma once
#include <vector>
#include <string>

namespace BlockService {
    struct DeviceInfo {
        std::string name;
        std::string mountpoint;
        size_t size_bytes;
        bool mounted;
    };

    // Returns a list of block devices and their mount status
    std::vector<DeviceInfo> GetMountedInfo();

    // Finds an unmounted /dev/sd* device, partitions it for the given size, creates a filesystem, and returns the device name
    // Throws std::runtime_error on failure
    std::string GetDevice(size_t size_bytes, const std::string& fs_type = "ext4");
}
