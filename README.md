# Mini Malloc

Mini Malloc is a small C++ project that demonstrates three arena-style memory
allocation strategies: linear, stack, and pool allocation. Each allocator owns
one backing buffer and serves allocations from it without calling the system
allocator for every request.

This is an educational allocator library, not a replacement for the C standard
library's `malloc` and `free`.

## Allocators

| Allocator | Allocation | Reclamation | Good fit |
| --- | --- | --- | --- |
| `LinearAllocator` | O(1) bump allocation | Reset the entire arena | Per-frame or request-scoped temporary data |
| `StackAllocator` | O(1) bump allocation | Roll back to a saved marker or reset | Nested scratch allocations with LIFO lifetimes |
| `PoolAllocator` | O(1) free-list pop | Return individual fixed-size blocks | Many same-sized objects with arbitrary lifetimes |

All allocation methods return `nullptr` when their backing storage is
exhausted. Linear and stack allocations accept an optional alignment, which
defaults to `alignof(std::max_align_t)`.

## Project layout

```text
src/
├── main.cpp                 # Demonstrates all three allocators
├── linear/
│   ├── linear-alloc.h
│   └── linear-alloc.cpp
├── stack/
│   ├── stack-alloc.h
│   └── stack-alloc.cpp
└── pool/
    ├── pool-alloc.h
    └── pool-alloc.cpp
```

## Build and run

You need a C++17-compatible compiler such as Clang or GCC.

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic \
  src/main.cpp \
  src/linear/linear-alloc.cpp \
  src/stack/stack-alloc.cpp \
  src/pool/pool-alloc.cpp \
  -o mini-malloc

./mini-malloc
```

The demo allocates memory with each strategy, releases or resets part of the
arena, and shows that the reclaimed addresses are reused.

## Usage

### Linear allocator

```cpp
#include "linear/linear-alloc.h"

LinearAllocator arena(1024);
void* first = arena.allocate(128);
void* second = arena.allocate(256, 32);

arena.reset(); // invalidates every allocation from this arena
```

### Stack allocator

```cpp
#include "stack/stack-alloc.h"

StackAllocator arena(1024);
void* persistent = arena.allocate(64);

auto marker = arena.getMarker();
void* temporary = arena.allocate(256);
arena.freeToMarker(marker); // invalidates temporary, but not persistent
```

### Pool allocator

```cpp
#include "pool/pool-alloc.h"

PoolAllocator pool(64, 32); // 32 blocks, each 64 bytes
void* block = pool.allocate();
pool.deallocate(block);
```

The pool's block size must be at least `sizeof(void*)`. In practice, use a block
size that is also a multiple of the alignment required by the stored type.

## Important limitations

- The allocators provide raw storage; they do not construct or destroy C++
  objects. Use placement `new` and explicit destruction when storing objects.
- Allocations become invalid when their allocator is reset, rolled back, or
  destroyed.
- Allocator instances cannot be copied.
- `PoolAllocator` validates range and block alignment with debug assertions,
  but it does not detect double-free errors.
- The implementation is not thread-safe.

## License

No license file is currently included. Add one before redistributing the
project.
