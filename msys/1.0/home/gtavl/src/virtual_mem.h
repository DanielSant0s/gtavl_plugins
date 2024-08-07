
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sifdev.h>

#define KB(val) val*1024
#define MB(val) KB(val)*1024

#define CACHE_SIZE 20  
#define MAX_SEGMENT_SIZE KB(8)  

typedef struct {
    int block_id;
    size_t offset;
    size_t size;
    int access_count;
    char *data;
} CacheSegment;

typedef struct {
    int id;
    size_t size;
    bool cached;
} VirtualMemoryBlock;

typedef struct {
    int file;
    char* name;
    size_t total_size;
    size_t usage;
    CacheSegment cache[CACHE_SIZE];
    int next_block_id;
} VirtualMemory;

void init_virtual_memory(const char *filename, size_t total_size);

void register_block(VirtualMemoryBlock *block, size_t block_size, bool cached);

void read_block(VirtualMemoryBlock *block, char *buffer, size_t offset, size_t length);

void write_block(VirtualMemoryBlock *block, const char *buffer, size_t offset, size_t length);

void free_virtual_memory();
