#include "pool-alloc.h"
#include <cassert>

// Each free block repurposes its first bytes as a next-pointer.
struct FreeNode {
    FreeNode* next;
};

PoolAllocator::PoolAllocator(std::size_t blockSize, std::size_t blockCount)
    : m_buffer(nullptr), m_blockSize(blockSize), m_blockCount(blockCount), m_freeList(nullptr)
{
    assert(blockSize >= sizeof(FreeNode) && "blockSize too small to hold free-list pointer");
    m_buffer = new std::uint8_t[blockSize * blockCount];

    // Chain all blocks into the free list.
    for (std::size_t i = 0; i < blockCount; ++i) {
        auto* node  = reinterpret_cast<FreeNode*>(m_buffer + i * blockSize);
        node->next  = static_cast<FreeNode*>(m_freeList);
        m_freeList  = node;
    }
}

PoolAllocator::~PoolAllocator() {
    delete[] m_buffer;
}

void* PoolAllocator::allocate() {
    if (!m_freeList)
        return nullptr;
    auto* node = static_cast<FreeNode*>(m_freeList);
    m_freeList = node->next;
    return node;
}

void PoolAllocator::deallocate(void* ptr) {
    if (!ptr) return;

    // Guard against out-of-range or misaligned pointers.
    auto addr = reinterpret_cast<std::uintptr_t>(ptr);
    auto base = reinterpret_cast<std::uintptr_t>(m_buffer);
    assert(addr >= base && addr < base + m_blockSize * m_blockCount && "ptr out of pool range");
    assert((addr - base) % m_blockSize == 0 && "ptr is not block-aligned");

    auto* node = static_cast<FreeNode*>(ptr);
    node->next = static_cast<FreeNode*>(m_freeList);
    m_freeList = node;
}
