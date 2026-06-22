#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H

#include <cstddef>
#include <cstdint>
#include <vector>

struct StackAllocator {
    std::vector<std::uint8_t> buffer; // buffer to hold the allocated memory
    std::size_t offset; // current offset in the buffer for the next allocation

    // Constructor to initialize the allocator with a specified size
    StackAllocator(std::size_t size): buffer(size), offset(0) {}

    // Allocate memory with specified size and alignment
    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
        std::size_t currentAddress = reinterpret_cast<std::size_t>(buffer.data() + offset);
        std::size_t padding = (alignment - (currentAddress % alignment)) % alignment;
        if (offset + padding + size > buffer.size()) {
            return nullptr;
        }
        offset += padding;
        void* ptr = buffer.data() + offset;
        offset += size;
        return ptr;
    }

    // Reset the allocator to reuse the buffer
    void reset() {
        offset = 0;
    }
};

#endif // STACK_ALLOC_H
