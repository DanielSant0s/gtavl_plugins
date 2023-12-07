#ifndef STDLIB_H_
#define STDLIB_H_

#include <stdint.h>

extern void* (*malloc)(uint32_t);
extern void (*free)(void*);

extern double (*atof)(const char* str);
extern int (*atoi)(const char * str);
extern float (*dptofp)(double num);

#endif
