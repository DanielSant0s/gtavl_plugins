#ifndef MATH_H_
#define MATH_H_

#define FLT_MAX 3.402823466e+38f
#define RAND_MAX 0x7fff

extern float (*sinf)(float);
extern float (*cosf)(float);
extern float (*atan2f)(float, float);
extern float (*floorf)(float);
extern float (*powf)(float, float);
extern int (*rand)();

float fabsf(float __x);
float sqrtf(float __x);


int max(int a, int b);

int min(int a, int b);

#endif
