#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>

class Memcpy {
public:
    void *memcpy(void *dest, void *src, size_t n) {
        // Cast the source and destination to char* for byte-wise copying
        char *d = static_cast<char *>(dest);
        const char *s = static_cast<const char *>(src);
        
        // Perform the copy
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }

        return dest;
    }
};

int main() {
    Memcpy myMemcpy;

    const char src[] = "Hello, World!";
    char dest[50] = {0};

    myMemcpy.memcpy(dest, (void *) src, strlen(src) + 1);

    std::cout << "Copied string: " << dest << std::endl;

    return 0;
}