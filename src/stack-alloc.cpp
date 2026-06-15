#include <cstdio>
#include <vector>

struct StackAllocator {
    std::vector<std::uint8_t> buffer; // buffer to hold the allocated memory
    std::size_t offset; // current offset in the buffer for the next allocation

    // Constructor to initialize the allocator with a specified size
    StackAllocator(std::size_t size): buffer(size), offset(0) {}

    // Allocate memory with specified size and alignment
    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
        std::size_t currentAddress = reinterpret_cast<std::size_t>(buffer.data() + offset); // Get the current address in the buffer
        std::size_t padding = (alignment - (currentAddress % alignment)) % alignment; // Calculate padding needed to align the address
        if (offset + padding + size > buffer.size()) {
            return nullptr; // Not enough memory
        }
        offset += padding; // Add padding to align the address
        void* ptr = buffer.data() + offset; // Get the aligned address
        offset += size; // Move the offset for the next allocation
        return ptr;
    }

    // Reset the allocator to reuse the buffer
    void reset() {
        offset = 0; // reset the offset to reuse the buffer
    }
};

int main() {   
    StackAllocator allocator(1024); // Create a stack allocator with 1KB of memory

    // Allocate some memory
    void* ptr1 = allocator.allocate(256); // Allocate 256 bytes
    if (ptr1) {
        printf("Allocated 256 bytes at address: %p\n", ptr1);
    } else {
        printf("Failed to allocate memory\n");
    }
    
    void* ptr2 = allocator.allocate(128); // Allocate 128 bytes
    if (ptr2) {
        printf("Allocated 128 bytes at address: %p\n", ptr2);
    } else {
        printf("Failed to allocate memory\n");
    }

    void* ptr3 = allocator.allocate(64); // Allocate 64 bytes
    if (ptr3) {
        printf("Allocated 64 bytes at address: %p\n", ptr3);
    } else {
        printf("Failed to allocate memory\n");
    }

    allocator.reset(); // Reset the allocator to reuse the buffer

    void* ptr4 = allocator.allocate(512); // Allocate 512 bytes after reset
    if (ptr4) {
        printf("Allocated 512 bytes at address: %p\n", ptr4);
    } else {
        printf("Failed to allocate memory\n");
    }

    return 0;
}