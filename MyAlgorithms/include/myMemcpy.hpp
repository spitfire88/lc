#ifndef MY_MEMCPY_HPP
#define MY_MEMCPY_HPP

#include <cstddef>
#include <cstdint>

/**
 * @brief Custom memory copy implementation with optimizations
 * 
 * This class provides various memory copy implementations including
 * byte-wise copy, word-aligned copy, and SIMD optimizations.
 */
class MyMemcpy {
public:
    /**
     * @brief Basic byte-wise memory copy
     * @param dest Destination memory address
     * @param src Source memory address
     * @param n Number of bytes to copy
     * @return Pointer to destination
     */
    static void* basicCopy(void* dest, const void* src, size_t n);

    /**
     * @brief Word-aligned memory copy for better performance
     * @param dest Destination memory address (must be word-aligned)
     * @param src Source memory address (must be word-aligned)
     * @param n Number of bytes to copy
     * @return Pointer to destination
     */
    static void* wordAlignedCopy(void* dest, const void* src, size_t n);

    /**
     * @brief Optimized memory copy that chooses the best strategy
     * @param dest Destination memory address
     * @param src Source memory address
     * @param n Number of bytes to copy
     * @return Pointer to destination
     */
    static void* optimizedCopy(void* dest, const void* src, size_t n);

    /**
     * @brief Check if pointer is word-aligned
     * @param ptr Pointer to check
     * @return true if word-aligned, false otherwise
     */
    static bool isWordAligned(const void* ptr);

private:
    static const size_t WORD_SIZE = sizeof(uint64_t);
};

#endif // MY_MEMCPY_HPP
