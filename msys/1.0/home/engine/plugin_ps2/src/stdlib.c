#include <stdint.h>
#include <stdlib.h>

void* (*malloc)(uint32_t)  = (void* (*)(uint32_t))0x001005E0;
void (*free)(void*)  = (void (*)(void*))0x00100780;

double (*atof)(const char* str) = (double (*)(const char* str))0x54A340;
int (*atoi)(const char * str) = (int (*)(const char * str))0x54A358;
float (*dptofp)(double num) = (float (*)(double num))0x531D78;



