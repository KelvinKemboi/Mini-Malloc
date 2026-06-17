#include <cstdio>
#include <vector>
#include <cstdint>

struct PoolAllocator {
    std::vector<std::uint8_t> buffer; // buffer to hold the allocated memory
    std::size_t blockSize; // size of each block in the pool
    std::size_t blockCount; // total number of blocks in the pool
    std::vector<bool> freeBlocks; // vector to track free blocks

    // Constructor to initialize the allocator with a specified block size and count
    PoolAllocator(std::size_t blockSize, std::size_t blockCount)
        : buffer(blockSize * blockCount), blockSize(blockSize), blockCount(blockCount), freeBlocks(blockCount, true) {}

    // Allocate a block of memory from the pool
    void* allocate() {
        for (std::size_t i = 0; i < blockCount; ++i) {
            if (freeBlocks[i]) { // Check if the block is free
                freeBlocks[i] = false; // Mark the block as used
                return buffer.data() + i * blockSize; // Return the address of the allocated block
            }
        }
        return nullptr; // No free blocks available
    }
    // Deallocate a block of memory back to the pool
    void deallocate(void* ptr) {
        std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr);
        std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(buffer.data());
        if (address >= baseAddress && address < baseAddress + buffer.size()) {
            std::size_t index = (address - baseAddress) / blockSize; // Calculate the index of the block
            freeBlocks[index] = true; // Mark the block as free
        }
    }
};

int main() {
    PoolAllocator allocator(64, 16); // Create a pool allocator with block size of 64 bytes and 16 blocks

    // Allocate some blocks
    void* ptr1 = allocator.allocate(); // Allocate first block
    if (ptr1) {
        printf("Allocated block at address: %p\n", ptr1);
    } else {
        printf("Failed to allocate block\n");
    }

    void* ptr2 = allocator.allocate(); // Allocate second block
    if (ptr2) {
        printf("Allocated block at address: %p\n", ptr2);
    } else {
        printf("Failed to allocate block\n");
    }

    allocator.deallocate(ptr1); // Deallocate the first block
    printf("Deallocated block at address: %p\n", ptr1);

    void* ptr3 = allocator.allocate(); // Allocate another block (should reuse the first block)
    if (ptr3) {
        printf("Allocated block at address: %p\n", ptr3);
    } else {
        printf("Failed to allocate block\n");
    }

    return 0;
}