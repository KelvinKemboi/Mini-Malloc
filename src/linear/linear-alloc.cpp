#include "linear-alloc.h"

LinearAllocator::LinearAllocator(std::size_t size)
    : m_buffer(new std::uint8_t[size]), m_capacity(size), m_offset(0) {}

LinearAllocator::~LinearAllocator() {
    delete[] m_buffer;
}

void* LinearAllocator::allocate(std::size_t size, std::size_t alignment) {
    std::size_t current = reinterpret_cast<std::size_t>(m_buffer + m_offset);
    std::size_t padding = (alignment - (current % alignment)) % alignment;
    if (m_offset + padding + size > m_capacity)
        return nullptr;
    m_offset += padding;
    void* ptr = m_buffer + m_offset;
    m_offset += size;
    return ptr;
}

void LinearAllocator::reset() {
    m_offset = 0;
}
