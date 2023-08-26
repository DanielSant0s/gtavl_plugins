#ifndef CCAM_H_
#define CCAM_H_

#include <CVector.h>
#include <stdint.h>
#include <common.h>

typedef struct
{
    CPlaceable placeable;
} CCamera;


extern void (*CCam_Process_AimWeapon)(uint32_t this, CVector *a2, float a3, float a4, float a5);
extern void (*CCam_Process_FollowPed_SA)(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6);

extern CCamera* TheCamera;

#endif
