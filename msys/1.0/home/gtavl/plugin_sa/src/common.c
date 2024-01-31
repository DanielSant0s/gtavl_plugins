#include <common.h>
#include <math.h>
#include <CVector.h>
#include <CCoronas.h>

CVector* getVehCoordinates(uint32_t* veh) {
    CVector* ret;
    uint32_t v90;

    v90 = *(uint32_t *)((uint32_t)veh + 20);
    if ( v90 )
        ret = (float *)((uint32_t)v90 + 48);
    else
        ret = (float *)((uint32_t)veh + 4);
    
    return ret;
} 

CVector* getCharCoordinates(uint32_t* ped) {
    uint32_t v58;
    uint32_t v57;
    uint32_t v60;
    CVector* ret;

    if ( (*(uint8_t *)((uint32_t)ped + 1189) & 1) != 0 )
        v57 = *(uint32_t *)((uint32_t)ped + 1484);
    else
        v57 = 0;

    if ( v57 )
    {
        v58 = *(uint32_t *)(v57 + 20);
        if ( v58 )
            ret = (CVector *)(v58 + 48);
        else
            ret = (CVector *)(v57 + 4);
    }
    else
    {
        v60 = *(uint32_t *)((uint32_t)ped + 20);
        if ( v60 )
            ret = (CVector *)(v60 + 48);
        else
            ret = (CVector *)((uint32_t)ped + 4);
    }

    return ret;
}

float DistanceBetweenPoints(CVector* v1, CVector* v2) {
    return sqrtf((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y));
}

CVector TransformFromObjectSpace(uint32_t entity, CVector offset) {
    CVector ret;

    if ( *(uint32_t *)(entity + 20) ) {
        CMatrix_Attach(&ret, *(uint32_t *)(entity + 20), &offset);
    } else {
        sub_25F980(&ret, (float *)(entity + 4), &offset);
    }

    return ret;
}

CMatrix * CPlaceable_GetMatrix(CPlaceable *this)
{
    if ( !this->m_matrix )
    {
        CPlaceable_AllocateMatrix(this);
        CSimpleTransform_UpdateMatrix(&this->placement, this->m_matrix);
    }
    return this->m_matrix;
}


