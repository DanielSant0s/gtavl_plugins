#ifndef STDLIB_H_
#define STDLIB_H_

#include <stdint.h>

void* malloc(uint32_t size);
void free(void *ptr);

#define malloc_array(type, size) malloc(sizeof(type)*size)

double atof(const char* str);
int atoi(const char *str);
float dptofp(double num);

#endif
