#ifndef CVECTOR_H_
#define CVECTOR_H_

typedef struct {
    float x;
    float y;
    float z;

} CVector;

extern void (*Normalise)(CVector* this);
extern void (*CrossProduct)(CVector *out, CVector *a, CVector *b);
float DotProduct(CVector* v1, CVector* v2);

#endif
