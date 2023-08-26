#ifndef CVECTOR_H_
#define CVECTOR_H_

typedef struct {
    float x;
    float y;
    float z;

} CVector;

extern void (*CVector_Normalise)(CVector* this);
extern void (*CrossProduct)(CVector *out, CVector *a, CVector *b);
float DotProduct(CVector* v1, CVector* v2);
void *CVector_CVector_2(void *this);
int vector_constructor_403D60(char *objects, int size, int count, int callback);

#endif
