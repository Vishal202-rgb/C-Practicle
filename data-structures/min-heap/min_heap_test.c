#include "min_heap.h"
#include <assert.h>
#include <stdio.h>
static int int_compare(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}
static void test_basic_operations(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        4,
        int_compare
    );
    assert(heap != NULL);
    assert(min_heap_is_empty(heap));
    assert(min_heap_size(heap) == 0);
    assert(min_heap_capacity(heap) >= 8);
    int values[] = {42, 7, 19, 3, 25, 1, 100, 8};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        assert(min_heap_push(heap, &values[i]) == 1);
    }
    assert(min_heap_size(heap) == 8);
    assert(!min_heap_is_empty(heap));
    int top = 0;
    assert(min_heap_peek(heap, &top) == 1);
    assert(top == 1);
    min_heap_destroy(heap);
}
static void test_sorted_extraction(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        8,
        int_compare
    );
    assert(heap != NULL);
    int values[] = {
        50, 20, 80, 10, 60,
        30, 40, 90, 5, 70
    };
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        assert(min_heap_push(heap, &values[i]) == 1);
    }
    int previous = -2147483647;
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        int current = 0;
        assert(min_heap_pop(heap, &current) == 1);
        assert(current >= previous);
        previous = current;
    }
    assert(min_heap_is_empty(heap));
    assert(min_heap_pop(heap, NULL) == 0);
    min_heap_destroy(heap);
}
static void test_duplicates_and_negative_values(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        8,
        int_compare
    );
    assert(heap != NULL);
    int values[] = {
        -10, 5, -10, 0, 5,
        -100, 50, -1, 0
    };
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        assert(min_heap_push(heap, &values[i]) == 1);
    }
    int expected[] = {
        -100, -10, -10, -1,
        0, 0, 5, 5, 50
    };
    for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i) {
        int actual = 0;
        assert(min_heap_pop(heap, &actual) == 1);
        assert(actual == expected[i]);
    }
    min_heap_destroy(heap);
}
static void test_large_workload(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        8,
        int_compare
    );
    assert(heap != NULL);
    for (int i = 10000; i >= 1; --i) {
        assert(min_heap_push(heap, &i) == 1);
    }
    assert(min_heap_size(heap) == 10000);
    assert(min_heap_capacity(heap) >= 10000);
    for (int expected = 1; expected <= 10000; ++expected) {
        int actual = 0;
        assert(min_heap_pop(heap, &actual) == 1);
        assert(actual == expected);
    }
    assert(min_heap_is_empty(heap));
    min_heap_destroy(heap);
}
static void test_reserve(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        8,
        int_compare
    );
    assert(heap != NULL);
    assert(min_heap_reserve(heap, 2048) == 1);
    assert(min_heap_capacity(heap) >= 2048);
    int value = 123;
    assert(min_heap_push(heap, &value) == 1);
    assert(min_heap_size(heap) == 1);
    int top = 0;
    assert(min_heap_peek(heap, &top) == 1);
    assert(top == 123);
    min_heap_destroy(heap);
}
static void test_custom_comparator(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        8,
        int_compare
    );
    assert(heap != NULL);
    int values[] = {9, 4, 7, 1, 6};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        assert(min_heap_push(heap, &values[i]) == 1);
    }
    int result = 0;
    assert(min_heap_pop(heap, &result) == 1);
    assert(result == 1);
    assert(min_heap_pop(heap, &result) == 1);
    assert(result == 4);
    min_heap_destroy(heap);
}
static void test_invalid_operations(void)
{
    MinHeap *heap = min_heap_create(
        sizeof(int),
        8,
        int_compare
    );
    assert(heap != NULL);
    int value = 10;
    int output = 0;
    assert(min_heap_push(NULL, &value) == 0);
    assert(min_heap_push(heap, NULL) == 0);
    assert(min_heap_peek(NULL, &output) == 0);
    assert(min_heap_peek(heap, NULL) == 0);
    assert(min_heap_pop(NULL, &output) == 0);
    assert(min_heap_reserve(NULL, 100) == 0);
    assert(min_heap_pop(heap, &output) == 0);
    assert(min_heap_is_empty(heap));
    assert(min_heap_create(0, 8, int_compare) == NULL);
    assert(min_heap_create(sizeof(int), 8, NULL) == NULL);
    min_heap_destroy(heap);
    min_heap_destroy(NULL);
}
int main(void)
{
    test_basic_operations();
    test_sorted_extraction();
    test_duplicates_and_negative_values();
    test_large_workload();
    test_reserve();
    test_custom_comparator();
    test_invalid_operations();
    printf("All min-heap tests passed.\n");
    return 0;
}
