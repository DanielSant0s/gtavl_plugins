#ifndef STDLIB_H_
#define STDLIB_H_

#include <stdint.h>

void* (*malloc)(uint32_t);
void (*free)(void*);

#endif
