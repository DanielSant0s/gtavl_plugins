#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "alloc.h"

void init_allocator(Allocator *allocator, void *start, void *end) {
    allocator->start = start;
    allocator->end = end;
    allocator->size = ((size_t)(end)-(size_t)(start));
    allocator->freeList = (Block *)start;
    allocator->freeList->size = (char *)end - (char *)start - BLOCK_SIZE;
    allocator->freeList->next = NULL;
    allocator->freeList->free = 1;
}

size_t align_up(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

void *aligned_allocate(Allocator *allocator, size_t size, size_t alignment) {
    Block *current = allocator->freeList;
    Block *previous = NULL;

    while (current != NULL) {
        if (current->free) {
            char *alignedStart = (char *)align_up((size_t)((char *)current + BLOCK_SIZE), alignment);
            size_t totalSize = (alignedStart - (char *)current) + size;
            if (current->size >= totalSize) {
                if (current->size > totalSize + BLOCK_SIZE) {
                    Block *newBlock = (Block *)((char *)current + totalSize + BLOCK_SIZE);
                    newBlock->size = current->size - totalSize - BLOCK_SIZE;
                    newBlock->next = current->next;
                    newBlock->free = 1;
                    current->size = totalSize - BLOCK_SIZE;
                    current->next = newBlock;
                }
                current->free = 0;
                allocator->allocated += current->size;
                return alignedStart;
            }
        }
        previous = current;
        current = current->next;
    }
    return NULL; // No sufficient block found
}

void deallocate(Allocator *allocator, void *ptr) {
    if (ptr == NULL) return;

    Block *block = (Block *)((char *)ptr - BLOCK_SIZE);
    allocator->allocated -= block->size;
    block->free = 1;

    Block *current = allocator->freeList;
    while (current != NULL) {
        if (current->free && current->next != NULL && current->next->free) {
            current->size += current->next->size + BLOCK_SIZE;
            current->next = current->next->next;
        }
        current = current->next;
    }
}
