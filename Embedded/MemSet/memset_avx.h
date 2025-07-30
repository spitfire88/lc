#ifndef MEMSET_AVX_H
#define MEMSET_AVX_H

#include <stdint.h>
#include <stddef.h>

// Function prototypes
void memset64(void *ptr, int len, uint8_t val);
void memsetAVX(void *ptr, size_t len, uint8_t val);
void memsetAVX_streaming(void *ptr, size_t len, uint8_t val);
int has_avx_support(void);
void test_memset_implementations(void);

// Performance optimization hints
#define CACHE_LINE_SIZE 64
#define AVX_REGISTER_SIZE 32
#define STREAMING_THRESHOLD (8 * 1024 * 1024)  // 8MB

// Utility macros
#define ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define IS_ALIGNED(ptr, align) (((uintptr_t)(ptr) & ((align) - 1)) == 0)

#endif // MEMSET_AVX_H
