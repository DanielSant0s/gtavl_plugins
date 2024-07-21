#ifndef MATH_H_
#define MATH_H_

#include <stdint.h>

#define FLT_MAX 3.402823466e+38f
#define RAND_MAX 2147483600

float sinf(float);
float cosf(float);
float atan2f(float, float);
float floorf(float);
float powf(float, float);
void srand(unsigned int a1);
int rand();

float fabsf(float __x);
float sqrtf(float __x);

int max(int a, int b);
int min(int a, int b);

int randint(int min, int max);

float random_float_seed(float min, float max, uint16_t seed);
float random_float(float min, float max);

#endif
