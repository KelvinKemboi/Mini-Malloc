#include <cstddef>
#include <cstdint>
#include <cstdio>

struct LinearAllocator {
    std::uint8_t* buffer; // points to start of the allocated memory
    std::size_t capacity; // total size of the allocated memory
    std::size_t offset; // current offset in the buffer for the next allocation

    // Constructor to initialize the allocator with a specified size
    LinearAllocator(std::size_t size):capacity(size), offset(0) {
        buffer = new std::uint8_t[size];
    }

    // Destructor to free the allocated memory
    ~LinearAllocator() {
        delete[] buffer;
    }

    // Allocate memory with specified size and alignment
    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
        std::size_t currentAddress = reinterpret_cast<std::size_t>(buffer + offset); // Get the current address in the buffer
        std::size_t padding = (alignment - (currentAddress % alignment)) % alignment; // Calculate padding needed to align the address
        if (offset + padding + size > capacity) {
            return nullptr; // Not enough memory
        }
        offset += padding; // Add padding to align the address
        void* ptr = buffer + offset; // Get the aligned address
        offset += size; // Move the offset for the next allocation
        return ptr;
    }

    // Reset the allocator to reuse the buffer
    void reset() {
        offset = 0; // reset the offset to reuse the buffer
    }
};

int main() {   
    LinearAllocator allocator(1024); // Create a linear allocator with 1KB of memory

    return 0;
}