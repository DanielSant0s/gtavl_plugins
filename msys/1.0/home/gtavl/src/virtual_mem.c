#include "virtual_mem.h"
#include "bios_alloc.h"

VirtualMemory vm;

void init_virtual_memory(const char *filename, size_t total_size) {
    int fd = sceOpen(filename, SCE_RDONLY);

    if (fd) {
        sceClose(fd);
        sceRemove(filename);
    }

    vm.file = sceOpen(filename, SCE_RDWR | SCE_CREAT);
    vm.name = filename;
    
    vm.usage = 0;
    vm.total_size = total_size;
    vm.next_block_id = 0;

    sceLseek(vm.file, total_size - 1, 0);

    int zero = 0;
    int ret = sceWrite(vm.file, &zero, 1);

    sceLseek(vm.file, 0, 0);

    int i;
    for (i = 0; i < CACHE_SIZE; i++) {
        vm.cache[i].block_id = -1;
        vm.cache[i].access_count = 0;
        vm.cache[i].offset = 0;
        vm.cache[i].size = 0;
        vm.cache[i].data = NULL;
    }
}

size_t virtual_memory_size() {
    return vm.total_size;
}

size_t virtual_memory_usage() {
    return vm.usage;
}

void alloc_block(VirtualMemoryBlock *block, size_t block_size, bool cached) {
    block->id = vm.next_block_id;
    block->size = block_size;
    block->cached = (cached && (block_size <= MAX_SEGMENT_SIZE * CACHE_SIZE));

    vm.usage += block->size;
    vm.next_block_id += block->size;
}

void free_block(VirtualMemoryBlock *block) {
    vm.usage -= block->size;

    block->id = -1;
    block->size = 0;
}

void read_block_raw(int block_id, char *buffer, size_t offset, size_t length) {
    sceLseek(vm.file, block_id + offset, 0);
    sceRead(vm.file, buffer, length);
}

void write_block_raw(int block_id, char *buffer, size_t offset, size_t length) {
    sceLseek(vm.file, block_id + offset, 0);
    sceWrite(vm.file, buffer, length);
}

void read_segment(int block_id, char *buffer, size_t offset, size_t length) {
    int i;
    for (i = 0; i < CACHE_SIZE; i++) {
        if (vm.cache[i].block_id == block_id && 
            vm.cache[i].offset <= offset && 
            (vm.cache[i].offset + vm.cache[i].size) >= (offset + length)) {
            memcpy(buffer, vm.cache[i].data + (offset - vm.cache[i].offset), length);
            vm.cache[i].access_count++;
            return;
        }
    }

    read_block_raw(block_id, buffer, offset, length);

    int least_used_index = 0;
    for (i = 1; i < CACHE_SIZE; i++) {
        if (vm.cache[i].access_count < vm.cache[least_used_index].access_count) {
            least_used_index = i;
        }
    }

    if (vm.cache[least_used_index].data != NULL) {
        br_free(vm.cache[least_used_index].data);
    }

    vm.cache[least_used_index].block_id = block_id;
    vm.cache[least_used_index].offset = offset;
    vm.cache[least_used_index].size = length;
    vm.cache[least_used_index].access_count = 1;
    vm.cache[least_used_index].data = (char *)br_alloc(length);

    read_block_raw(block_id, vm.cache[least_used_index].data, offset, length);
    memcpy(buffer, vm.cache[least_used_index].data, length);
}

void read_block(VirtualMemoryBlock *block, char *buffer, size_t offset, size_t length) {
    if (!block->cached) {
        read_block_raw(block->id, buffer, offset, length);
        return;
    }

    size_t segment_offset = offset % MAX_SEGMENT_SIZE;
    size_t segment_length = length > MAX_SEGMENT_SIZE - segment_offset ? MAX_SEGMENT_SIZE - segment_offset : length;

    while (length > 0) {
        read_segment(block->id, buffer, offset, segment_length);
        buffer += segment_length;
        offset += segment_length;
        length -= segment_length;
        segment_length = length > MAX_SEGMENT_SIZE ? MAX_SEGMENT_SIZE : length;
    }
}

void write_segment(int block_id, const char *buffer, size_t offset, size_t length) {
    write_block_raw(block_id, buffer, offset, length);

    int i;
    for (i = 0; i < CACHE_SIZE; i++) {
        if (vm.cache[i].block_id == block_id && 
            vm.cache[i].offset <= offset && 
            (vm.cache[i].offset + vm.cache[i].size) >= (offset + length)) {
            memcpy(vm.cache[i].data + (offset - vm.cache[i].offset), buffer, length);
            vm.cache[i].access_count++;
            return;
        }
    }

    int least_used_index = 0;
    for (i = 1; i < CACHE_SIZE; i++) {
        if (vm.cache[i].access_count < vm.cache[least_used_index].access_count) {
            least_used_index = i;
        }
    }

    if (vm.cache[least_used_index].data != NULL) {
        br_free(vm.cache[least_used_index].data);
    }

    vm.cache[least_used_index].block_id = block_id;
    vm.cache[least_used_index].offset = offset;
    vm.cache[least_used_index].size = length;
    vm.cache[least_used_index].access_count = 1;
    vm.cache[least_used_index].data = (char *)br_alloc(length);

    read_block_raw(block_id, vm.cache[least_used_index].data, offset, length);
    memcpy(vm.cache[least_used_index].data, buffer, length);
}

void write_block(VirtualMemoryBlock *block, const char *buffer, size_t offset, size_t length) {
    if (!block->cached) {
        write_block_raw(block->id, buffer, offset, length);
        return;
    }

    size_t segment_offset = offset % MAX_SEGMENT_SIZE;
    size_t segment_length = length > MAX_SEGMENT_SIZE - segment_offset ? MAX_SEGMENT_SIZE - segment_offset : length;

    while (length > 0) {
        write_segment(block->id, buffer, offset, segment_length);
        buffer += segment_length;
        offset += segment_length;
        length -= segment_length;
        segment_length = length > MAX_SEGMENT_SIZE ? MAX_SEGMENT_SIZE : length;
    }
}

void free_virtual_memory() {
    int i;
    for (i = 0; i < CACHE_SIZE; i++) {
        if (vm.cache[i].data != NULL) {
            br_free(vm.cache[i].data);
        }
    }
    sceClose(vm.file);
}

