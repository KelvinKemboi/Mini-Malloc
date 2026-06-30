#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H

#include <cstddef>
#include <cstdint>

// LIFO bump-pointer allocator. Like LinearAllocator but supports saving a
// Marker and rolling back to it, enabling nested temporary scratch regions.
struct StackAllocator {
    using Marker = std::size_t;

    StackAllocator(std::size_t size);
    ~StackAllocator();

    StackAllocator(const StackAllocator&)            = delete;
    StackAllocator& operator=(const StackAllocator&) = delete;

    void*  allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
    Marker getMarker() const;
    void   freeToMarker(Marker marker);
    void   reset();

private:
    std::uint8_t* m_buffer;
    std::size_t   m_capacity;
    std::size_t   m_offset;
};

#endif // STACK_ALLOC_H
