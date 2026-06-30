#ifndef LINEAR_ALLOC_H
#define LINEAR_ALLOC_H

#include <cstddef>
#include <cstdint>

// Bump-pointer allocator. Allocations are O(1); deallocation resets the
// entire buffer at once. Individual frees are not supported.
struct LinearAllocator {
    LinearAllocator(std::size_t size);
    ~LinearAllocator();

    // Deleted to prevent accidental copies that would double-free the buffer.
    LinearAllocator(const LinearAllocator&)            = delete;
    LinearAllocator& operator=(const LinearAllocator&) = delete;

    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
    void  reset();

private:
    std::uint8_t* m_buffer;
    std::size_t   m_capacity;
    std::size_t   m_offset;
};

#endif // LINEAR_ALLOC_H
