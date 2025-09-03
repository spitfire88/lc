#include "BlockService.h"
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace BlockService {

std::vector<DeviceInfo> GetMountedInfo() {
    std::vector<DeviceInfo> devices;
    FILE* fp = popen("lsblk -b -o NAME,MOUNTPOINT,SIZE --noheadings --nodeps /dev/sd* 2>/dev/null", "r");
    if (!fp) throw std::runtime_error("Failed to run lsblk");
    char buf[512];
    while (fgets(buf, sizeof(buf), fp)) {
        std::istringstream iss(buf);
        std::string name, mountpoint, size_str;
        iss >> name >> mountpoint >> size_str;
        DeviceInfo info;
        info.name = "/dev/" + name;
        info.mountpoint = mountpoint;
        info.size_bytes = size_str.empty() ? 0 : std::stoull(size_str);
        info.mounted = !mountpoint.empty() && mountpoint != "-";
        devices.push_back(info);
    }
    pclose(fp);
    return devices;
}

std::string GetDevice(size_t size_bytes, const std::string& fs_type) {
    auto devices = GetMountedInfo();
    auto it = std::find_if(devices.begin(), devices.end(), [](const DeviceInfo& d) {
        return !d.mounted;
    });
    if (it == devices.end()) throw std::runtime_error("No unmounted /dev/sd* device found");
    std::string dev = it->name;
    // Partition the device (use sfdisk for simplicity)
    std::string part = dev + "1";
    std::ostringstream cmd;
    cmd << "echo '," << size_bytes << ",,\n' | sudo sfdisk " << dev;
    if (system(cmd.str().c_str()) != 0) throw std::runtime_error("Partitioning failed");
    // Create filesystem
    std::ostringstream mkfs;
    mkfs << "sudo mkfs." << fs_type << " " << part << " -F";
    if (system(mkfs.str().c_str()) != 0) throw std::runtime_error("mkfs failed");
    return part;
}

} // namespace BlockService
