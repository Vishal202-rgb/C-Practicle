# Generic Min-Heap

A generic, dynamically growing min-heap implementation in C11.

## Features

- Generic element storage using `element_size`
- Custom comparator support
- Dynamic capacity growth
- Sift-up and sift-down operations
- Push, peek, and pop
- Explicit capacity reservation
- Overflow-safe memory allocation
- Allocation failure handling
- Duplicate and negative values supported
- C11 compatible
- No external dependencies

## API

### Create

`MinHeap *min_heap_create(size_t element_size, size_t initial_capacity, min_heap_compare_fn compare);`

Creates an empty heap for elements of `element_size` bytes.

### Push

`int min_heap_push(MinHeap *heap, const void *element);`

Adds an element while maintaining the min-heap property.

### Peek

`int min_heap_peek(const MinHeap *heap, void *out_element);`

Copies the smallest element without removing it.

### Pop

`int min_heap_pop(MinHeap *heap, void *out_element);`

Removes the smallest element and optionally copies it into `out_element`.

### Reserve

`int min_heap_reserve(MinHeap *heap, size_t capacity);`

Ensures that the heap has at least the requested capacity.

### Destroy

`void min_heap_destroy(MinHeap *heap);`

Releases all memory owned by the heap.

## Complexity

| Operation | Complexity |
|---|---|
| Push | O(log n * s) |
| Peek | O(s) |
| Pop | O(log n * s) |
| Reserve | O(n * s) when reallocation occurs |
| Size | O(1) |
| Is Empty | O(1) |

Here `n` is the number of elements and `s` is the element size in bytes.

## Heap Representation

For an element at index `i`:

- Parent: `(i - 1) / 2`
- Left child: `2 * i + 1`
- Right child: `2 * i + 2`

### Sift-Up

After insertion, the new element moves upward until the heap property is restored.

### Sift-Down

After removing the root, the final element moves downward until the heap property is restored.

## Building and Testing

Compile with GCC:

`gcc -std=c11 -Wall -Wextra -Wpedantic min_heap.c min_heap_test.c -o min_heap_test`

Run:

`./min_heap_test`

Expected output:

`All min-heap tests passed.`

## Files

```text
min-heap/
+-- min_heap.h
+-- min_heap.c
+-- min_heap_test.c
+-- README.md
```
