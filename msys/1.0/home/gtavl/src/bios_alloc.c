
#include <stdbool.h>
#include "alloc.h"
#include "bios_alloc.h"

#define bios_ram84 0x00084000
#define bios_ramD0 0x000D0000
#define game_start 0x00100008

Allocator allocator;

void br_init(bool fill) {
    init_allocator(&allocator, (char*)(fill? bios_ram84 : bios_ramD0), (char *)game_start);
}

void* br_alloc(size_t size) {
    return aligned_allocate(&allocator, size, 16);
}

void br_free(void* ptr) {
    deallocate(&allocator, ptr);
}

size_t br_size() {
    return allocator.size;
}

size_t br_used() {
    return allocator.allocated;
}
