#include "wrappers.h"

extern "C" {
    #define CORDIC_MAX_ITER 30

    float logf(float x) {
        if (x <= 0.0f) {
            return 3.402823466e+38f;
        }

        int n;
        float result = 0.0f;
        float y = (x - 1) / (x + 1);
        float y2 = y * y;
        float term = y;
        for (n = 1; n < CORDIC_MAX_ITER; n++) {
            result += term / (2.0f * n - 1);
            term *= y2;
        }
        result *= 2.0f;

        return result;
    }
}