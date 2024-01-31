#include <math.h>

float fabsf(float __x) { return __builtin_fabsf(__x); }
float sqrtf(float __x) { return __builtin_sqrtf(__x); }


int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int randint(int min, int max) {
    return (min + rand() % (max - min));
}