#ifndef MATH_H_
#define MATH_H_

#define FLT_MAX 3.402823466e+38f
#define RAND_MAX 0x7fff

float sinf(float);
float cosf(float);
float atan2f(float, float);
float floorf(float);
float powf(float, float);
int rand();

float fabsf(float __x);
float sqrtf(float __x);

int max(int a, int b);
int min(int a, int b);

int randint(int min, int max);

#endif
