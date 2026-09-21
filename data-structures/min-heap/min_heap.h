#ifndef MIN_HEAP_H
#define MIN_HEAP_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef int (*min_heap_compare_fn)(const void *a, const void *b);
typedef struct MinHeap MinHeap;
MinHeap *min_heap_create(
    size_t element_size,
    size_t initial_capacity,
    min_heap_compare_fn compare
);
void min_heap_destroy(MinHeap *heap);
int min_heap_push(MinHeap *heap, const void *element);
int min_heap_peek(
    const MinHeap *heap,
    void *out_element
);
int min_heap_pop(
    MinHeap *heap,
    void *out_element
);
int min_heap_reserve(
    MinHeap *heap,
    size_t capacity
);
size_t min_heap_size(const MinHeap *heap);
size_t min_heap_capacity(const MinHeap *heap);
size_t min_heap_element_size(const MinHeap *heap);
int min_heap_is_empty(const MinHeap *heap);
#ifdef __cplusplus
}
#endif
#endif
