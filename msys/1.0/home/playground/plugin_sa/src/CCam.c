#include <CCam.h>

void (*CCam_Process_AimWeapon)(uint32_t this, CVector *a2, float a3, float a4, float a5) = (void (*)(uint32_t, CVector *, float, float, float))0x204CE0;
void (*CCam_Process_FollowPed_SA)(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) = (void (*)(uint32_t, CVector *, int, int, int, char))0x207ED0;

CCamera* TheCamera = (CCamera*)0x6FE530;