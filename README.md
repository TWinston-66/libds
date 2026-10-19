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

## Roadmap

1. Allocator
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
