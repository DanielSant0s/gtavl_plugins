#include <injector.h>
#include <CCam.h>
#include <CPad.h>
#include <stdio.h>
#include <math.h>
#include <common.h>
#include "hooks.h"

enum eWeaponAimOffset {
    AIM_OFFSET_WEAPON_DEFAULT,
    AIM_OFFSET_WEAPON_COLT45,
    AIM_OFFSET_WEAPON_SILENCED,
    AIM_OFFSET_WEAPON_DESERTEAGLE,
    AIM_OFFSET_WEAPON_CHROMEGUN,
    AIM_OFFSET_WEAPON_SAWNOFF,
    AIM_OFFSET_WEAPON_SHOTGSPA,
    AIM_OFFSET_WEAPON_MICROUZI,
    AIM_OFFSET_WEAPON_MP5,
    AIM_OFFSET_WEAPON_TEC9,
    AIM_OFFSET_WEAPON_AK47,
    AIM_OFFSET_WEAPON_M4,
    AIM_OFFSET_WEAPON_CUNTGUN,
    AIM_OFFSET_WEAPON_ROCKETLA,
    AIM_OFFSET_WEAPON_HEATSEEK,
    AIM_OFFSET_WEAPON_FLAME,
    AIM_OFFSET_WEAPON_GRENADE
};

typedef struct {
    float f0, f1, f2, f3, f4, f5, f6;
} tAimingCamData;

static tAimingCamData gData[4];
static CVector gOffsets[17] = { };

void Process_AdvancedAimWeapon(uint32_t cam, CVector *vec, float arg3, float arg4, float arg5) {
    uint32_t playa = FindPlayerPed(-1);
    uint8_t active_weapon;
    CVector obj_space;

    if (playa && !(*(uint8_t *)(playa + 0x4A5) & 1)) {
        int aimTypeId = -1;

        active_weapon = *(uint8_t *)(playa + 0x758);
        switch (*(uint32_t *)(playa + 0x1C * active_weapon + 0x5E0)) {
            case WEAPON_PISTOL:
                aimTypeId = AIM_OFFSET_WEAPON_COLT45;
                break;
            case WEAPON_PISTOL_SILENCED:
                aimTypeId = AIM_OFFSET_WEAPON_SILENCED;
                break;
            case WEAPON_DESERT_EAGLE:
                aimTypeId = AIM_OFFSET_WEAPON_DESERTEAGLE;
                break;
            case WEAPON_SHOTGUN:
                aimTypeId = AIM_OFFSET_WEAPON_CHROMEGUN;
                break;
            case WEAPON_SAWNOFF:
                aimTypeId = AIM_OFFSET_WEAPON_SAWNOFF;
                break;
            case WEAPON_SPAS12:
                aimTypeId = AIM_OFFSET_WEAPON_SHOTGSPA;
                break;
            case WEAPON_MICRO_UZI:
                aimTypeId = AIM_OFFSET_WEAPON_MICROUZI;
                break;
            case WEAPON_MP5:
                aimTypeId = AIM_OFFSET_WEAPON_MP5;
                break;
            case WEAPON_TEC9:
                aimTypeId = AIM_OFFSET_WEAPON_TEC9;
                break;
            case WEAPON_AK47:
                aimTypeId = AIM_OFFSET_WEAPON_AK47;
                break;
            case WEAPON_M4:
                aimTypeId = AIM_OFFSET_WEAPON_M4;
                break;
            case WEAPON_COUNTRYRIFLE:
                aimTypeId = AIM_OFFSET_WEAPON_CUNTGUN;
                break;
            case WEAPON_RLAUNCHER:
                aimTypeId = AIM_OFFSET_WEAPON_ROCKETLA;
                break;
            case WEAPON_RLAUNCHER_HS:
                aimTypeId = AIM_OFFSET_WEAPON_HEATSEEK;
                break;
            case WEAPON_FTHROWER:
                aimTypeId = AIM_OFFSET_WEAPON_FLAME;
                break;
            case WEAPON_GRENADE:
            case WEAPON_TEARGAS:
            case WEAPON_MOLOTOV:
            case WEAPON_SATCHEL_CHARGE:
                aimTypeId = AIM_OFFSET_WEAPON_GRENADE;
                break;
        }
        if (aimTypeId != -1) {
            obj_space = TransformFromObjectSpace(playa, gOffsets[aimTypeId]);
            CCam_Process_AimWeapon(cam, &obj_space, arg3, arg4, arg5);
        } else {
            CCam_Process_FollowPed_SA(cam, vec, arg3, arg4, arg5, 0);
        }
    }
    else
        CCam_Process_AimWeapon(cam, vec, arg3, arg4, arg5);
}

float WeaponAimZoomSpeed = 4.0f;

void AdjustTimeStep() {
    uint32_t a1;
    float weapon_zoom;

    asm("move %0, $s4" : "=r" (a1)); // get from our parent function
    asm("mfc1 %0, $f3" : "=r" (*(uint32_t*)&weapon_zoom));

    float v17 = *(float *)(a1 + 180);
    if ( weapon_zoom <= (float)(v17 + CTimer_ms_fTimeStep) )
    {
        if ( weapon_zoom >= (float)(v17 - CTimer_ms_fTimeStep) )
            *(float *)(a1 + 180) = weapon_zoom;
        else
            *(float *)(a1 + 180) = v17 - (CTimer_ms_fTimeStep*WeaponAimZoomSpeed);
    }
    /*else
    {
        *(float *)(a1 + 180) = v17 + (CTimer_ms_fTimeStep*4.0f);
    }*/
}



float (*CGeneral_GetATanOfXY)(float x, float y) = (float (*)(float x, float y))0x245200;

static float mymodf(float numerator, float denominator) {
    float quotient = numerator / denominator;
    float integer_part = (quotient >= 0.0f) ? (int)quotient : (int)(quotient - 1);
    float remainder = numerator - integer_part * denominator;

    return remainder;
}
inline float invert_angle(float angle) {
    angle = mymodf(angle, 134.0f);
    float inverted_angle = mymodf(134.0f - angle, 134.0f);

    return inverted_angle;
}

char CWeapon_Fire(void *this, CEntity *owner, CVector *vecOrigin, CVector *vecEffectPosn, CEntity *targetEntity, CVector *vecTarget, CVector *arg_14);

void CCamera_Find3rdPersonCamTargetVector(CCamera *this, float dist, CVector* pos, CVector *unkVec, CVector *output);

static CVector PlayerTargettedVector = {};
static CVector UnkVector = {};

static void (*Multiply3x3)(CVector *out, CMatrix *m, CVector *in) = (void (*)(CVector *out, CMatrix *m, CVector *in))0x1100D0;

#define clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))

static float fastExp4(register float x)  // quartic spline approximation
{
    union { float f; int32_t i; } reinterpreter;

    reinterpreter.i = (int32_t)(12102203.0f*x) + 127*(1 << 23);
    int32_t m = (reinterpreter.i >> 7) & 0xFFFF;  // copy mantissa
    // empirical values for small maximum relative error (1.21e-5):
    reinterpreter.i += (((((((((((3537*m) >> 16)
        + 13668)*m) >> 18) + 15817)*m) >> 14) - 80470)*m) >> 11);
    return reinterpreter.f;
}

static float expClamp(float valor, float max, float ln) {
    float result = fastExp4(clamp(valor, 0.0f, 1.0f) * ln) - 1.0f;
    return clamp(result, 0.0f, max);
}


bool crosshair_defined = false;

void ThrowProjectileControl() {
    uint32_t a1, a2;
    asm("move %0, $s2" : "=r" (a1)); // get from our parent function
    asm("move %0, $s1" : "=r" (a2)); // get from our parent function

    if (a2 == FindPlayerPed(-1)) {
	    CMatrix *matrix = TheCamera.placeable.m_matrix;
	    float x = matrix->up.x;
	    float y = matrix->up.y;
	    float z = matrix->up.z;
	    float angle = CGeneral_GetATanOfXY(z, sqrtf(x * x + y * y)) * 57.295776f - 41.0f;
	    while (angle < 0.0f)
	    	angle += 360.0f;

        float final = (invert_angle(angle) / 5.0f);
        final = final > 15.0f? final : expClamp(final/26.0f, 35.0f, 3.55535f);
        final = final < 15.6f? final : expClamp(final/26.0f, 100.0f, 4.60517f);
        int slot = getPedActiveWeaponSlot(a2);
        CWeapon* weapons = getPedWeapons(a2);
        int weaponType = weapons[slot].m_nType;

        if (  weaponType == WEAPON_SATCHEL_CHARGE ) 
            final *= 2.5f;

        CEntity* v22 = (CEntity*)FindPlayerPed(-1);
        CVector in = { -0.2f, 0.0f, 0.3f };
        CVector posn = { };
        CVector v24 = { };
        CVector *v28;

        Multiply3x3(&v24, v22->placeable.m_matrix, &in);

        posn.x = v24.x;
        posn.y = v24.y;
        posn.z = v24.z;

        if ( v22->placeable.m_matrix )
          v28 = &v22->placeable.m_matrix->pos;
        else
          v28 = &v22->placeable.placement.m_vPosn;

        posn.x += v28->x;
        posn.y += v28->y;
        posn.z += v28->z;
        CCamera_Find3rdPersonCamTargetVector(&TheCamera, final, &posn, &UnkVector, &PlayerTargettedVector);
    } else {
        *(float *)(*(uint32_t *)(a2 + 1208) + 44) = (float)(50.0f * (float)*(int *)(a1 + 32)) / 1000.0f;
    }

}

char CWeapon_CustomTargetFire(void *this, CEntity *owner, CVector *vecOrigin, CVector *vecEffectPosn, CEntity *targetEntity, CVector *vecTarget, CVector *arg_14) {
    if (owner == FindPlayerPed(-1)) {
        CWeapon_Fire(this, owner, vecOrigin, vecEffectPosn, targetEntity, &PlayerTargettedVector, arg_14);
    } else {
       CWeapon_Fire(this, owner, vecOrigin, vecEffectPosn, targetEntity, vecTarget, arg_14); 
    }
}

void VCamera(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) {
    CVector out, obj_space;
    uint32_t playa = FindPlayerPed(-1);

    out.x = 0.3f;
    out.y = 0.0f;
    out.z = 0.0f;

    int slot = getPedActiveWeaponSlot(playa);

    CPad* pad = CPad_GetPad(0);

    if(pad->NewState.LeftShoulder2 && slot == 8) {
        Process_AdvancedAimWeapon(this, a2, a3, a4, a5);
        if (!crosshair_defined) {
            *(uint32_t*)0x66B5A4 = 1;
            crosshair_defined = true;
        }
    } else {
        if (crosshair_defined) {
            *(uint32_t*)0x66B5A4 = 0;
            crosshair_defined = false;
        }
        obj_space = TransformFromObjectSpace(playa, out);
        CCam_Process_FollowPed_SA(this, &obj_space, a3, a4, a5, a6);
    }

}

void (*CTaskSimplePlayerOnFoot_ProcessPlayerWeapon)(void* this, CEntity* ped) = (void (*)(void* this, CEntity* ped))0x463BB0;
void (*CWorld_UseDetonator)(CEntity* ped) = (void (*)(CEntity* ped))0x288470;

void CTaskSimplePlayerOnFoot_CustomProcessPlayerWeapon(void* this, CEntity* ped) {
    CPad* pad = CPad_GetPad(0);
    if (pad->NewState.DPadLeft && !pad->OldState.DPadLeft) {
        CWorld_UseDetonator(ped);
    }
    CTaskSimplePlayerOnFoot_ProcessPlayerWeapon(this, ped);
}

void setupAimPatches()
{  
    RedirectCall(0x202A10, VCamera);

    RedirectCall(0x408DE4, ThrowProjectileControl);
    WriteByte(0x2AAD9F, 0x14);

    WriteByte(0x409264, 0xFF); // Invalidate satchel charge anim decision

    RedirectCall(0x408F40, CWeapon_CustomTargetFire);
    RedirectCall(0x408F7C, CWeapon_CustomTargetFire);

    RedirectCall(0x4676D4, CTaskSimplePlayerOnFoot_CustomProcessPlayerWeapon);
    RedirectCall(0x4CBABC, CTaskSimplePlayerOnFoot_CustomProcessPlayerWeapon);

    MakeNop(0x134268); // Make detonator useless
    MakeNop(0x1342BC); // Make detonator useless
    MakeNop(0x134168); // Make detonator useless
    

    MakeNop(0x408DE8);
    MakeNop(0x408DEC);
    MakeNop(0x408DF0);
    MakeNop(0x408DF4);
    MakeNop(0x408DF8);
    MakeNop(0x408DFC);
    MakeNop(0x408E00);
    MakeNop(0x408E04);
    MakeNop(0x408E08);

    *(float *)0x66521C = 30.0f; // AIMWEAPON_RIFLE1_ZOOM
    *(float *)0x665218 = 30.0f; // AIMWEAPON_RIFLE2_ZOOM
    *(uint32_t *)0x204E44 = 0x3C0241F0; // AIMWEAPON_DEFAULT_ZOOM = 25.0f
    *(float *)0x665214 = 0.17453f; // AIMWEAPON_DRIVE_CLOSE_ENOUGH
    *(float *)0x665210 = 0.25f; // AIMWEAPON_DRIVE_SENS_MULT
    *(float *)0x66520C = 0.1f; // AIMWEAPON_FREETARGET_SENS
    *(float *)0x665204 = 0.007f; // AIMWEAPON_STICK_SENS
/*
    // GUN_DATA
    gData[0].f0 = 0.6f;
    gData[0].f1 = 0.5f;
    gData[0].f2 = 1.0f;
    gData[0].f3 = -0.12f;
    gData[0].f4 = 0.02f;
    gData[0].f5 = 0.7854f;
    gData[0].f6 = 1.5533f;

    // BIKE_DATA
    gData[1].f0 = 3.5f;
    gData[1].f1 = 0.7f;
    gData[1].f2 = 1.0f;
    gData[1].f3 = -0.16f;
    gData[1].f4 = 0.2f;
    gData[1].f5 = 0.61087f;
    gData[1].f6 = 1.2217f;

    // VEHICLE_DATA
    gData[2].f0 = 6.0f;
    gData[2].f1 = 0.7f;
    gData[2].f2 = 1.0f;
    gData[2].f3 = -0.16f;
    gData[2].f4 = 0.4f;
    gData[2].f5 = 0.61087f;
    gData[2].f6 = 1.2217f;

    // MELEE_DATA
    gData[3].f0 = 2.5f;
    gData[3].f1 = 0.7f;
    gData[3].f2 = 1.0f;
    gData[3].f3 = -0.12f;
    gData[3].f4 = 0.15f;
    gData[3].f5 = 0.7854f;
    gData[3].f6 = 1.79412f;

    memcpy((void *)0x665220, gData, 112);
*/
    gOffsets[0].x = 0.0f;
    gOffsets[0].y = 0.0f;
    gOffsets[0].z = 0.0f;

    gOffsets[1].x = 0.22f;
    gOffsets[1].y = 0.0f;
    gOffsets[1].z = 0.0f;

    gOffsets[2].x = 0.22f;
    gOffsets[2].y = 0.1f;
    gOffsets[2].z = 0.0f;

    gOffsets[AIM_OFFSET_WEAPON_ROCKETLA].x = 0.22f;
    gOffsets[AIM_OFFSET_WEAPON_ROCKETLA].y = 0.1f;
    gOffsets[AIM_OFFSET_WEAPON_ROCKETLA].z = 0.0f;

    gOffsets[AIM_OFFSET_WEAPON_GRENADE].x = 0.22f;
    gOffsets[AIM_OFFSET_WEAPON_GRENADE].y = 0.1f;
    gOffsets[AIM_OFFSET_WEAPON_GRENADE].z = 0.0f;

    RedirectCall(0x202AB8, Process_AdvancedAimWeapon);
    RedirectCall(0x204EB0, AdjustTimeStep);
    MakeNop(0x204EB4);
    MakeBranch(0x204EB8, 0xD);
}
