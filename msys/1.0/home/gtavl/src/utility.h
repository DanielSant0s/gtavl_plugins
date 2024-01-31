#ifndef MATH_UTILS
#define MATH_UTILS

#include <rwcore.h>
#include <CVector.h>
#include <math.h>

float DotProduct2D(CVector2D* v1, CVector2D* v2) {
    return v1->x * v2->x + v1->y * v2->y;
}

float CrossProduct2D(CVector2D* v1, CVector2D* v2) {
    return v1->x * v2->y - v1->y * v2->x;
}

float Distance2D(CVector2D* v, float x, float y) {
    return sqrtf((v->x - x) * (v->x - x) + (v->y - y) * (v->y - y));
}

float DistanceSqr2D(CVector2D* v, float x, float y) {
    return (v->x - x) * (v->x - x) + (v->y - y) * (v->y - y);
}

#define SQR(x) ((x) * (x))

int GetRandomNumber() {
    return rand() & RAND_MAX;
}

RwTexture* (*RwTextureRead)(const char* name, const char* mask) = (RwTexture* (*)(const char* name, const char* mask))0x345A80;

#endif
