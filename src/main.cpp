#include <cstdio>
#include "linear/linear-alloc.h"
#include "stack/stack-alloc.h"
#include "pool/pool-alloc.h"

int main() {
    // --- Linear Allocator ---
    printf("=== LinearAllocator ===\n");
    LinearAllocator linear(1024);
    void* l1 = linear.allocate(256);
    void* l2 = linear.allocate(128);
    void* l3 = linear.allocate(64);
    printf("l1=%p  l2=%p  l3=%p\n", l1, l2, l3);
    linear.reset();
    void* l4 = linear.allocate(512);
    printf("after reset: l4=%p (reuses l1: %s)\n\n", l4, l4 == l1 ? "yes" : "no");

    // --- Stack Allocator ---
    printf("=== StackAllocator ===\n");
    StackAllocator stack(1024);
    void* s1 = stack.allocate(64);
    StackAllocator::Marker mark = stack.getMarker();
    void* s2 = stack.allocate(128);
    void* s3 = stack.allocate(64);
    printf("s1=%p  s2=%p  s3=%p\n", s1, s2, s3);
    stack.freeToMarker(mark);         // rolls back s2 and s3, keeps s1
    void* s4 = stack.allocate(128);
    printf("after freeToMarker: s4=%p (reuses s2: %s)\n\n", s4, s4 == s2 ? "yes" : "no");

    // --- Pool Allocator ---
    printf("=== PoolAllocator ===\n");
    PoolAllocator pool(64, 16);       // 16 blocks of 64 bytes
    void* p1 = pool.allocate();
    void* p2 = pool.allocate();
    void* p3 = pool.allocate();
    printf("p1=%p  p2=%p  p3=%p\n", p1, p2, p3);
    pool.deallocate(p2);
    void* p4 = pool.allocate();
    printf("after deallocate(p2): p4=%p (reuses p2: %s)\n", p4, p4 == p2 ? "yes" : "no");

    return 0;
}
