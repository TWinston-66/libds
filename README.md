# libds

Simple DSA lib in C17.

Intrusive structures take no allocator; owning structures take `ds_alloc`

## Structures

### Intrusive

- Doubly linked list
- Stack
- Queue

Stack and queue are thin wrappers over the doubly linked list.

### Owning

- Vector
- Vector Stack
- Ring Buffer

## Allocators

`ds_alloc` (`<ds/alloc/alloc.h>`) is a small vtable of `alloc`, `realloc`, and
`free`, plus a `ctx` pointer passed back to each call. Every call takes an
`align` (must be a power of two) and the block's size, so allocators can be
sized and alignment-aware.

- **libc** (`<ds/alloc/default.h>`): `malloc` / `realloc` / `free`, falling back
  to `aligned_alloc` for alignments above `max_align_t`
- **Arena** (`<ds/alloc/arena.h>`): bump allocator over a caller-owned buffer.
  `free` only reclaims the most recent allocation; `ds_arena_reset` releases
  everything at once

```c
static unsigned char buf[4096];
ds_arena arena = {buf, sizeof(buf), 0};

ds_alloc a = {
    .alloc = ds_arena_alloc,
    .realloc = ds_arena_realloc,
    .free = ds_arena_free,
    .ctx = &arena,
};

int *xs = a.alloc(a.ctx, 16 * sizeof(int), _Alignof(int));
```

See `alloc.h` for the full contract (failure behaviour, size 0, etc.).

## Building

```sh
make          # configure + build (Debug)
make test     # build + run tests (Criterion)
make format   # clang-format
```

## Roadmap

1. Allocator (done)
2. Vector (uses allocator)
3. Vector based Stack (owning)
4. Ring Buffer (owning)
5. Binary search (raw arrays plus a vec wrapper)
6. BST, with traversals and the augmented-size variant
7. Treap
8. AVL Tree
9. Heap
10. Priority Queue
11. Hash Table
12. Trie
13. LRU Cache
14. Skip List
15. Bloom Filter
16. B-Tree
17. Grid
18. Quadtree
19. k-d tree
20. Graph (BFS, DFS, Dijkstra)

## License

See [LICENSE](LICENSE).
