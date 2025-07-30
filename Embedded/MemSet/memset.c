// memset in c
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>  // For AVX intrinsics
#include <string.h>     // For standard memset fallback
#include <stdlib.h>     // For aligned_alloc and free
#include <time.h>       // For clock functions
#include "memset_avx.h" // Our header file

void memset64(void *ptr, int len, uint8_t val) {
    if(ptr == NULL) {
        perror("ptr is NULL");
        return;
    }
    
    int nbytes = len / sizeof(int);
    int remaining = len % sizeof(int);
    uint64_t *temp = (uint64_t *)ptr;
    int val64 = val << 56 | val << 48 | val << 40 | val << 32 | val << 24 | val << 16 | val << 8 | val;
    for(int i = 0; i < nbytes; i++) {
        *(temp + i) = val64;
    }
    uint8_t *temp8 = (uint8_t *) (ptr + nbytes);
    if(remaining) {
        for(int i = 0; i < remaining; i++) {
            *(temp8 + i) = val;
        }
    }
    return;
}

void memsetAVX(void *ptr, size_t len, uint8_t val) {
    if (ptr == NULL) {
        perror("ptr is NULL");
        return;
    }
    
    // Check for AVX support (runtime check would be better in production)
    // For this example, we assume AVX is available
    
    uint8_t *dest = (uint8_t *)ptr;
    size_t remaining = len;
    
    // Create 256-bit (32-byte) vector filled with the value
    __m256i avx_val = _mm256_set1_epi8(val);
    
    // Align pointer to 32-byte boundary for optimal AVX performance
    uintptr_t addr = (uintptr_t)dest;
    size_t alignment_offset = (32 - (addr & 31)) & 31;
    
    // Handle unaligned bytes at the beginning
    if (alignment_offset > 0 && remaining > 0) {
        size_t bytes_to_align = (alignment_offset < remaining) ? alignment_offset : remaining;
        memset(dest, val, bytes_to_align);
        dest += bytes_to_align;
        remaining -= bytes_to_align;
    }
    
    // Process 256 bytes (8 x 32-byte chunks) at a time for maximum throughput
    size_t avx_chunks_256 = remaining / 256;
    for (size_t i = 0; i < avx_chunks_256; i++) {
        _mm256_store_si256((__m256i*)(dest + 0), avx_val);
        _mm256_store_si256((__m256i*)(dest + 32), avx_val);
        _mm256_store_si256((__m256i*)(dest + 64), avx_val);
        _mm256_store_si256((__m256i*)(dest + 96), avx_val);
        _mm256_store_si256((__m256i*)(dest + 128), avx_val);
        _mm256_store_si256((__m256i*)(dest + 160), avx_val);
        _mm256_store_si256((__m256i*)(dest + 192), avx_val);
        _mm256_store_si256((__m256i*)(dest + 224), avx_val);
        dest += 256;
        remaining -= 256;
    }
    
    // Process remaining 32-byte aligned chunks
    size_t avx_chunks_32 = remaining / 32;
    for (size_t i = 0; i < avx_chunks_32; i++) {
        _mm256_store_si256((__m256i*)dest, avx_val);
        dest += 32;
        remaining -= 32;
    }
    
    // Handle remaining bytes (less than 32)
    if (remaining > 0) {
        memset(dest, val, remaining);
    }
}

// Alternative AVX implementation using streaming stores for very large memory
void memsetAVX_streaming(void *ptr, size_t len, uint8_t val) {
    if (ptr == NULL) {
        perror("ptr is NULL");
        return;
    }
    
    uint8_t *dest = (uint8_t *)ptr;
    size_t remaining = len;
    
    // Create 256-bit vector filled with the value
    __m256i avx_val = _mm256_set1_epi8(val);
    
    // Align pointer to 32-byte boundary
    uintptr_t addr = (uintptr_t)dest;
    size_t alignment_offset = (32 - (addr & 31)) & 31;
    
    // Handle unaligned bytes at the beginning
    if (alignment_offset > 0 && remaining > 0) {
        size_t bytes_to_align = (alignment_offset < remaining) ? alignment_offset : remaining;
        memset(dest, val, bytes_to_align);
        dest += bytes_to_align;
        remaining -= bytes_to_align;
    }
    
    // For very large buffers, use streaming stores to avoid cache pollution
    // Use streaming stores for chunks larger than typical L3 cache size
    const size_t streaming_threshold = 8 * 1024 * 1024; // 8MB threshold
    
    if (remaining >= streaming_threshold) {
        // Process 256 bytes at a time with streaming stores
        size_t streaming_chunks = remaining / 256;
        for (size_t i = 0; i < streaming_chunks; i++) {
            _mm256_stream_si256((__m256i*)(dest + 0), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 32), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 64), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 96), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 128), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 160), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 192), avx_val);
            _mm256_stream_si256((__m256i*)(dest + 224), avx_val);
            dest += 256;
            remaining -= 256;
        }
        
        // Ensure streaming stores are completed
        _mm_sfence();
    }
    
    // Process remaining 32-byte chunks with regular stores
    size_t avx_chunks = remaining / 32;
    for (size_t i = 0; i < avx_chunks; i++) {
        _mm256_store_si256((__m256i*)dest, avx_val);
        dest += 32;
        remaining -= 32;
    }
    
    // Handle remaining bytes
    if (remaining > 0) {
        memset(dest, val, remaining);
    }
}

// CPU feature detection for AVX support
int has_avx_support(void) {
    unsigned int eax, ebx, ecx, edx;
    
    // Check for AVX support using CPUID
    __asm__ ("cpuid"
            : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
            : "a" (1));
    
    return (ecx & (1 << 28)) != 0; // Check AVX bit
}

// Test and benchmark function
void test_memset_implementations(void) {
    const size_t test_sizes[] = {1024, 4096, 64*1024, 1024*1024, 16*1024*1024};
    const size_t num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    const uint8_t test_val = 0xAA;
    
    printf("Testing memset implementations:\n");
    printf("AVX Support: %s\n", has_avx_support() ? "Yes" : "No");
    printf("========================================\n");
    
    for (size_t i = 0; i < num_tests; i++) {
        size_t size = test_sizes[i];
        void *buffer = aligned_alloc(32, size); // 32-byte aligned allocation
        
        if (buffer == NULL) {
            printf("Failed to allocate %zu bytes\n", size);
            continue;
        }
        
        printf("\nBuffer size: %zu bytes (%.2f KB, %.2f MB)\n", 
               size, size/1024.0, size/(1024.0*1024.0));
        
        // Test standard memset
        clock_t start = clock();
        memset(buffer, test_val, size);
        clock_t end = clock();
        double std_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        printf("Standard memset: %.3f ms\n", std_time);
        
        // Test AVX memset
        start = clock();
        memsetAVX(buffer, size, test_val);
        end = clock();
        double avx_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        printf("AVX memset:     %.3f ms (%.2fx %s)\n", 
               avx_time, 
               std_time / avx_time,
               avx_time < std_time ? "faster" : "slower");
        
        // Test AVX streaming memset for large buffers
        if (size >= 1024*1024) {
            start = clock();
            memsetAVX_streaming(buffer, size, test_val);
            end = clock();
            double stream_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("AVX streaming:  %.3f ms (%.2fx %s)\n", 
                   stream_time,
                   std_time / stream_time,
                   stream_time < std_time ? "faster" : "slower");
        }
        
        free(buffer);
    }
}

int main(void) {
    test_memset_implementations();
    return 0;
}