#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Block {
    size_t size;
    struct Block *next;
    int free;
} Block;

typedef struct Allocator {
    void *start;
    void *end;
    size_t size;
    size_t allocated;
    Block *freeList;
} Allocator;

#define BLOCK_SIZE sizeof(Block)

void init_allocator(Allocator *allocator, void *start, void *end);

void *aligned_allocate(Allocator *allocator, size_t size, size_t alignment);

void deallocate(Allocator *allocator, void *ptr);


