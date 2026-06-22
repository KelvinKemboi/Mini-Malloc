#include <cstdio>
#include "linear-alloc.h"
#include "stack-alloc.h"
#include "pool-alloc.h"

int main() {
    printf("=== LinearAllocator Demo ===\n");
    LinearAllocator linear(1024);
    void* l1 = linear.allocate(256);
    void* l2 = linear.allocate(128);
    void* l3 = linear.allocate(64);
    printf("l1 = %p\n", l1);
    printf("l2 = %p\n", l2);
    printf("l3 = %p\n", l3);
    linear.reset();
    void* l4 = linear.allocate(512);
    printf("after reset, l4 = %p\n\n", l4);

    printf("=== StackAllocator Demo ===\n");
    StackAllocator stack(1024);
    void* s1 = stack.allocate(256);
    void* s2 = stack.allocate(128);
    void* s3 = stack.allocate(64);
    printf("s1 = %p\n", s1);
    printf("s2 = %p\n", s2);
    printf("s3 = %p\n", s3);
    stack.reset();
    void* s4 = stack.allocate(512);
    printf("after reset, s4 = %p\n\n", s4);

    printf("=== PoolAllocator Demo ===\n");
    PoolAllocator pool(64, 16);
    void* p1 = pool.allocate();
    void* p2 = pool.allocate();
    printf("p1 = %p\n", p1);
    printf("p2 = %p\n", p2);
    pool.deallocate(p1);
    printf("deallocated p1 = %p\n", p1);
    void* p3 = pool.allocate();
    printf("p3 = %p\n", p3);

    return 0;
}
