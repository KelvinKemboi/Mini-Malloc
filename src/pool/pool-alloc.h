#ifndef POOL_ALLOC_H
#define POOL_ALLOC_H

#include <cstddef>
#include <cstdint>

// Fixed-size block allocator backed by an intrusive singly-linked free list.
// Both allocate() and deallocate() are O(1). blockSize must be at least
// sizeof(void*) so each free block can hold the next-pointer.
struct PoolAllocator {
    PoolAllocator(std::size_t blockSize, std::size_t blockCount);
    ~PoolAllocator();

    PoolAllocator(const PoolAllocator&)            = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    void* allocate();
    void  deallocate(void* ptr);

private:
    std::uint8_t* m_buffer;
    std::size_t   m_blockSize;
    std::size_t   m_blockCount;
    void*         m_freeList; // head of the intrusive free list
};

#endif // POOL_ALLOC_H
