#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    // Calculate the number of bytes for approximately 8GB of RAM
    uint64_t bytesToAllocate = (uint64_t)20 * 1024 * 1024 * 1024; // 8GB in bytes

    // Allocate a large block of memory
    char* largeMemoryBlock = (char*)malloc(bytesToAllocate);

    if (largeMemoryBlock == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize the allocated memory (optional)
    for (uint64_t i = 0; i < bytesToAllocate; i++) {
        largeMemoryBlock[i] = 0;
    }

    // Simulate using the allocated memory (e.g., reading/writing)
    // ...

    // Free the allocated memory when done
    free(largeMemoryBlock);

    return 0;
}
