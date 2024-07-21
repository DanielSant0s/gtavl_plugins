#include <stdint.h>
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

float random_float_seed(float min, float max, uint16_t seed) {
    unsigned int resetSeed = rand() / 2;

    srand(seed);
    float x = (float)rand();
    srand((RsTimer(0) / 2) + resetSeed); // reset

    return (max - min) * (x / 2147483600) + min;
}

float random_float(float min, float max) {
    float x = (float)rand();
    return (max - min) * (x / 2147483600) + min;
}