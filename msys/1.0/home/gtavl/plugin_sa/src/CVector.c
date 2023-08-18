#include <CVector.h>

void (*Normalise)(CVector* this) = (void (*)(CVector *))0x110390;
void (*CrossProduct)(CVector *out, CVector *a, CVector *b) = (void (*)(CVector *, CVector *, CVector *))0x110060;

float DotProduct(CVector* v1, CVector* v2) {
    return v1->x*v2->x+v1->y*v2->y+v1->x*v2->y;
}
