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

CWanted* (*FindPlayerWanted)(int) = (CWanted* (*)(int))0x264080;

int (*IsPlayer)(void* ped) = (int (*)(void*))0x19CC40;

uint32_t* (*FindPlayerVehicle)(int, int) = (uint32_t* (*)(int, int))0x263DE0;

unsigned int (*FindPlayerPed)(int playerIndex) = (unsigned int (*)(int))0x264040;

void (*FindPlayerCoors)(CVector *outPoint, int playerIndex) = (void (*)(CVector *, int))0x263C90;

void (*CMatrix_Attach)(CVector *, uint32_t, CVector*) = (void (*)(CVector *, uint32_t, CVector*))0x110310;
void (*sub_25F980)(CVector *, uint32_t, CVector*) = (void (*)(CVector *, uint32_t, CVector*))0x25F980;

CVector TransformFromObjectSpace(uint32_t entity, CVector offset) {
    CVector ret;

    if ( *(uint32_t *)(entity + 20) ) {
        CMatrix_Attach(&ret, *(uint32_t *)(entity + 20), &offset);
    } else {
        sub_25F980(&ret, (float *)(entity + 4), &offset);
    }

    return ret;
}

float (*CWorld_FindGroundZFor3DCoord)(float x, float y, float z, int *b, uint32_t **ent) = (float (*)(float, float, float, int *, uint32_t **))0x286BB0;

unsigned int FrontEndMenuManager = 0x6FF940;

CColModel *(*CEntity_GetColModel)(uint32_t) = (CColModel *(*)(uint32_t))0x232B50;

uint32_t* CTimer_m_snTimeInMilliseconds = (uint32_t*)0x0066BA14;

uint32_t* CTimer_m_FrameCounter = (uint32_t*)0x66BA38;

uint32_t* CTimer_ms_fTimeStep = (uint32_t*)0x66BA30;

float* CTimer_ms_fTimeScale = (float*)0x66BA28;

uint32_t* CGame_currArea = (uint32_t*)0x66B868;

float* CWeather_Wind = (float*)0x66BD64;

CBulletTrace* CBulletTrace_aTraces = (CBulletTrace*)0x8171F0;

void (*CCoronas_RegisterCorona)(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t, CVector*,
 float, float, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, float, int, float, int, float, int, int) = (
	void (*)(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t, CVector*,
 float, float, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, float, int, float, int, float, int, int)
 )0x299380;
