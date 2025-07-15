#include "../include/myMemcpy.hpp"
#include <cstring>

void* MyMemcpy::basicCopy(void* dest, const void* src, size_t n) {
    if (dest == nullptr || src == nullptr || n == 0) {
        return dest;
    }

    char* d = static_cast<char*>(dest);
    const char* s = static_cast<const char*>(src);

    // Handle overlapping memory regions
    if (d < s) {
        // Copy forward
        for (size_t i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    } else if (d > s) {
        // Copy backward to handle overlap
        for (size_t i = n; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    }
    // If d == s, no copy needed

    return dest;
}

void* MyMemcpy::wordAlignedCopy(void* dest, const void* src, size_t n) {
    if (dest == nullptr || src == nullptr || n == 0) {
        return dest;
    }

    // Ensure both pointers are word-aligned
    if (!isWordAligned(dest) || !isWordAligned(src)) {
        return basicCopy(dest, src, n);
    }

    uint64_t* d64 = static_cast<uint64_t*>(dest);
    const uint64_t* s64 = static_cast<const uint64_t*>(src);
    
    size_t chunks = n / WORD_SIZE;
    size_t remaining = n % WORD_SIZE;

    // Copy in 64-bit chunks
    for (size_t i = 0; i < chunks; ++i) {
        d64[i] = s64[i];
    }

    // Copy remaining bytes
    if (remaining > 0) {
        char* d = reinterpret_cast<char*>(d64 + chunks);
        const char* s = reinterpret_cast<const char*>(s64 + chunks);
        for (size_t i = 0; i < remaining; ++i) {
            d[i] = s[i];
        }
    }

    return dest;
}

void* MyMemcpy::optimizedCopy(void* dest, const void* src, size_t n) {
    if (dest == nullptr || src == nullptr || n == 0) {
        return dest;
    }

    // For small copies, use basic copy
    if (n < 32) {
        return basicCopy(dest, src, n);
    }

    // Check for word alignment
    if (isWordAligned(dest) && isWordAligned(src)) {
        return wordAlignedCopy(dest, src, n);
    }

    // For unaligned but large copies, try to align
    char* d = static_cast<char*>(dest);
    const char* s = static_cast<const char*>(src);

    // Copy bytes until dest is aligned
    size_t align_offset = 0;
    uintptr_t dest_addr = reinterpret_cast<uintptr_t>(dest);
    while ((dest_addr + align_offset) % WORD_SIZE != 0 && align_offset < n) {
        d[align_offset] = s[align_offset];
        ++align_offset;
    }

    // Now use word-aligned copy for the middle section
    size_t remaining = n - align_offset;
    if (remaining >= WORD_SIZE) {
        void* aligned_dest = d + align_offset;
        const void* aligned_src = s + align_offset;
        
        if (isWordAligned(aligned_src)) {
            wordAlignedCopy(aligned_dest, aligned_src, remaining);
        } else {
            basicCopy(aligned_dest, aligned_src, remaining);
        }
    } else {
        // Copy remaining bytes
        for (size_t i = align_offset; i < n; ++i) {
            d[i] = s[i];
        }
    }

    return dest;
}

bool MyMemcpy::isWordAligned(const void* ptr) {
    return (reinterpret_cast<uintptr_t>(ptr) % WORD_SIZE) == 0;
}
