#include <math.h>

float (*sinf)(float) = (float (*)(float))0x53B178;
float (*cosf)(float) = (float (*)(float))0x53AE48;
float (*atan2f)(float, float) = (float (*)(float, float))0x53B338;
float (*floorf)(float) = (float (*)(float))0x53AF48;
float (*powf)(float, float) = (float (*)(float, float))0x00537468;
int (*rand)() = (int (*)())0x550248;

float fabsf(float __x) { return __builtin_fabsf(__x); }
float sqrtf(float __x) { return __builtin_sqrtf(__x); }


int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}
