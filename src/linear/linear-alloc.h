#ifndef LINEAR_ALLOC_H
#define LINEAR_ALLOC_H

#include <cstddef>
#include <cstdint>

struct LinearAllocator {
    std::uint8_t* buffer; // points to start of the allocated memory
    std::size_t capacity; // total size of the allocated memory
    std::size_t offset; // current offset in the buffer for the next allocation

    // Constructor to initialize the allocator with a specified size
    LinearAllocator(std::size_t size): capacity(size), offset(0) {
        buffer = new std::uint8_t[size];
    }

    // Destructor to free the allocated memory
    ~LinearAllocator() {
        delete[] buffer;
    }

    // Allocate memory with specified size and alignment
    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
        std::size_t currentAddress = reinterpret_cast<std::size_t>(buffer + offset);
        std::size_t padding = (alignment - (currentAddress % alignment)) % alignment;
        if (offset + padding + size > capacity) {
            return nullptr;
        }
        offset += padding;
        void* ptr = buffer + offset;
        offset += size;
        return ptr;
    }

    // Reset the allocator to reuse the buffer
    void reset() {
        offset = 0;
    }
};

#endif // LINEAR_ALLOC_H
