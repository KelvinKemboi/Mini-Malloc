#include "stack-alloc.h"

StackAllocator::StackAllocator(std::size_t size)
    : m_buffer(new std::uint8_t[size]), m_capacity(size), m_offset(0) {}

StackAllocator::~StackAllocator() {
    delete[] m_buffer;
}

void* StackAllocator::allocate(std::size_t size, std::size_t alignment) {
    std::size_t current = reinterpret_cast<std::size_t>(m_buffer + m_offset);
    std::size_t padding = (alignment - (current % alignment)) % alignment;
    if (m_offset + padding + size > m_capacity)
        return nullptr;
    m_offset += padding;
    void* ptr = m_buffer + m_offset;
    m_offset += size;
    return ptr;
}

StackAllocator::Marker StackAllocator::getMarker() const {
    return m_offset;
}

void StackAllocator::freeToMarker(Marker marker) {
    if (marker <= m_offset)
        m_offset = marker;
}

void StackAllocator::reset() {
    m_offset = 0;
}
