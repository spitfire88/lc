#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void *my_memcpy(void *dest, void *src, size_t n) {
    char *d = (char *) dest;
    char *s = (char *) src;

    for(size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

int is_word_aligned(void *ptr) {
    size_t word_size = 8;
    return ((uintptr_t)ptr % word_size == 0);
}

void *my_memcpy_word_aligned(void *dest, void *src, size_t n) {
    uint64_t *d64 = (uint64_t *) dest;
    uint64_t *s64 = (uint64_t *) src;
    size_t chunks = n / sizeof(uint64_t);
    size_t remaining = n % sizeof(uint64_t);

    for (size_t i = 0; i < chunks; i++) {
        d64[i] = s64[i];
    }

    char *d = (char *) (d64 + chunks);
    char *s = (char *) (s64 + chunks);
    for (size_t i = 0; i < remaining; i++) {
        d[i] = s[i];
    }
    return dest;
}

int main() {
    char *str1 = "I am happy!";
    char *str2 = malloc(strlen(str1) + 1);

    printf("strlens str1=%d str2=%d\n", strlen(str1), strlen(str2));

    //if(is_word_aligned(str1) && is_word_aligned(str2)) {
        my_memcpy_word_aligned(str2, str1, strlen(str1) + 1);
    //} else {
        my_memcpy(str2, str1, strlen(str1) + 1);
    }

    printf("str2 %s \n", str2);

    return 0;
}