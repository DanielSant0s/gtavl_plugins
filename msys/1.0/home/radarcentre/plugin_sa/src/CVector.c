#include <CVector.h>

void (*CVector_Normalise)(CVector* this) = (void (*)(CVector *))0x110390;
void (*CrossProduct)(CVector *out, CVector *a, CVector *b) = (void (*)(CVector *, CVector *, CVector *))0x110060;

float DotProduct(CVector* v1, CVector* v2) {
    return v1->x*v2->x+v1->y*v2->y+v1->x*v2->y;
}

int vector_constructor_403D60(char *objects, int size, int count, int callback)
{
    int result;
    int v6;

    result = count - 1;
    if ( count - 1 >= 0 )
    {
        v6 = count;
        do
        {
            result = ((int (*)(char *))callback)(objects);
            objects += size;
            --v6;
        }
        while ( v6 );
    }
    return result;
}

void *CVector_CVector_2(void *this)
{
    return this;
}
