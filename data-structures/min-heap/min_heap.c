#include "min_heap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define MIN_CAPACITY 8
struct MinHeap {
    unsigned char *data;
    size_t size;
    size_t capacity;
    size_t element_size;
    min_heap_compare_fn compare;
};
static int mul_overflow(size_t a, size_t b)
{
    return b != 0 && a > SIZE_MAX / b;
}
static int valid(const MinHeap *heap)
{
    return heap != NULL &&
           heap->element_size != 0 &&
           heap->compare != NULL;
}
static unsigned char *at(MinHeap *heap, size_t index)
{
    return heap->data + index * heap->element_size;
}
static const unsigned char *at_const(
    const MinHeap *heap,
    size_t index
)
{
    return heap->data + index * heap->element_size;
}
static void swap_elements(
    MinHeap *heap,
    size_t first,
    size_t second
)
{
    if (first == second) {
        return;
    }
    unsigned char temp[heap->element_size];
    unsigned char *a = at(heap, first);
    unsigned char *b = at(heap, second);
    memcpy(temp, a, heap->element_size);
    memcpy(a, b, heap->element_size);
    memcpy(b, temp, heap->element_size);
}
static int resize(MinHeap *heap, size_t capacity)
{
    if (!valid(heap) ||
        capacity == 0 ||
        capacity < heap->size ||
        mul_overflow(capacity, heap->element_size)) {
        return 0;
    }
    if (capacity == heap->capacity) {
        return 1;
    }
    unsigned char *data = realloc(
        heap->data,
        capacity * heap->element_size
    );
    if (data == NULL) {
        return 0;
    }
    heap->data = data;
    heap->capacity = capacity;
    return 1;
}
static int grow(MinHeap *heap)
{
    if (heap->size < heap->capacity) {
        return 1;
    }
    if (heap->capacity > SIZE_MAX / 2) {
        return 0;
    }
    size_t new_capacity =
        heap->capacity == 0
            ? MIN_CAPACITY
            : heap->capacity * 2;
    return resize(heap, new_capacity);
}
static void sift_up(MinHeap *heap, size_t index)
{
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap->compare(
                at(heap, index),
                at(heap, parent)
            ) >= 0) {
            break;
        }
        swap_elements(heap, index, parent);
        index = parent;
    }
}
static void sift_down(MinHeap *heap, size_t index)
{
    while (1) {
        size_t left = index * 2 + 1;
        size_t right = index * 2 + 2;
        size_t smallest = index;
        if (left < heap->size &&
            heap->compare(
                at(heap, left),
                at(heap, smallest)
            ) < 0) {
            smallest = left;
        }
        if (right < heap->size &&
            heap->compare(
                at(heap, right),
                at(heap, smallest)
            ) < 0) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        swap_elements(heap, index, smallest);
        index = smallest;
    }
}
MinHeap *min_heap_create(
    size_t element_size,
    size_t initial_capacity,
    min_heap_compare_fn compare
)
{
    if (element_size == 0 || compare == NULL) {
        return NULL;
    }
    if (initial_capacity < MIN_CAPACITY) {
        initial_capacity = MIN_CAPACITY;
    }
    if (mul_overflow(initial_capacity, element_size)) {
        return NULL;
    }
    MinHeap *heap = malloc(sizeof(*heap));
    if (heap == NULL) {
        return NULL;
    }
    heap->data = malloc(initial_capacity * element_size);
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->element_size = element_size;
    heap->compare = compare;
    return heap;
}
void min_heap_destroy(MinHeap *heap)
{
    if (heap == NULL) {
        return;
    }
    free(heap->data);
    free(heap);
}
int min_heap_push(MinHeap *heap, const void *element)
{
    if (!valid(heap) || element == NULL) {
        return 0;
    }
    if (!grow(heap)) {
        return 0;
    }
    memcpy(
        at(heap, heap->size),
        element,
        heap->element_size
    );
    heap->size++;
    sift_up(heap, heap->size - 1);
    return 1;
}
int min_heap_peek(
    const MinHeap *heap,
    void *out_element
)
{
    if (!valid(heap) ||
        heap->size == 0 ||
        out_element == NULL) {
        return 0;
    }
    memcpy(
        out_element,
        at_const(heap, 0),
        heap->element_size
    );
    return 1;
}
int min_heap_pop(
    MinHeap *heap,
    void *out_element
)
{
    if (!valid(heap) || heap->size == 0) {
        return 0;
    }
    if (out_element != NULL) {
        memcpy(
            out_element,
            at(heap, 0),
            heap->element_size
        );
    }
    heap->size--;
    if (heap->size > 0) {
        memcpy(
            at(heap, 0),
            at(heap, heap->size),
            heap->element_size
        );
        sift_down(heap, 0);
    }
    return 1;
}
int min_heap_reserve(
    MinHeap *heap,
    size_t capacity
)
{
    if (!valid(heap)) {
        return 0;
    }
    if (capacity <= heap->capacity) {
        return 1;
    }
    return resize(heap, capacity);
}
size_t min_heap_size(const MinHeap *heap)
{
    return valid(heap) ? heap->size : 0;
}
size_t min_heap_capacity(const MinHeap *heap)
{
    return valid(heap) ? heap->capacity : 0;
}
size_t min_heap_element_size(const MinHeap *heap)
{
    return valid(heap) ? heap->element_size : 0;
}
int min_heap_is_empty(const MinHeap *heap)
{
    return !valid(heap) || heap->size == 0;
}
