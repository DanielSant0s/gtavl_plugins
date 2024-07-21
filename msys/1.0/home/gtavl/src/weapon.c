#include <injector.h>
#include <CVehicle.h>
#include <CPad.h>
#include <CFont.h>
#include <CRadar.h>
#include <CPool.h>
#include <CRGBA.h>
#include <string.h>
#include <common.h>
#include <CTxdStore.h>
#include <stdio.h>
#include <math.h>
#include <bit.h>

#include "model_ids.h"

#include "hooks.h"

void (*CProjectileInfo_Update)(void) = (void (*)(void))0x131650;



static void (*Multiply3x3)(CVector *out, CMatrix *m, CVector *in) = (void (*)(CVector *out, CMatrix *m, CVector *in))0x1100D0;

static void (*CPhysical_AddToMovingList)(CPhysical* this) = (void (*)(CPhysical* this))0x24C2D0;

void CProjectileInfo_UpdateRotation() {
    CVector v173, outVec;

    CProjectileInfo_Update();

    int i;
    for (i = 0; i < 32; i++) {
        if (ms_apProjectile[i]) {
            if ( ms_apProjectile[i]->object.parent.entity.m_nModelIndex == MODEL_GRENADE || 
                 ms_apProjectile[i]->object.parent.entity.m_nModelIndex == MODEL_MOLOTOV || 
                 ms_apProjectile[i]->object.parent.entity.m_nModelIndex == MODEL_SATCHEL || 
                 ms_apProjectile[i]->object.parent.entity.m_nModelIndex == MODEL_TEARGAS ) {
                if (!ms_apProjectile[i]->object.parent.m_nPhysicalFlags.bOnSolidSurface &&
                    !ms_apProjectile[i]->object.parent.m_nPhysicalFlags.bSubmergedInWater) {
                    CVector *vel = &ms_apProjectile[i]->object.parent.m_vecMoveSpeed;
                    if ((sqrtf(vel->x * vel->x + vel->y * vel->y + vel->z * vel->z) * 50.0f) > 3.0f) {

                        uint16_t randomSeed = ms_apProjectile[i]->object.parent.entity.m_nRandomSeed;

                        v173.x = CTimer_ms_fTimeStep * random_float_seed(1.0f, 5.0f, randomSeed);
                        v173.y = CTimer_ms_fTimeStep * random_float_seed(-7.0f, -3.0f, randomSeed);
                        v173.z = CTimer_ms_fTimeStep * random_float_seed(1.0f, 5.0f, randomSeed);

                        Multiply3x3(&outVec, CPlaceable_GetMatrix(&ms_apProjectile[i]->object.parent.entity.placeable), &v173);

                        ms_apProjectile[i]->object.parent.m_vecTurnSpeed.x = outVec.x * 0.02f;
                        ms_apProjectile[i]->object.parent.m_vecTurnSpeed.y = outVec.y * 0.02f;
                        ms_apProjectile[i]->object.parent.m_vecTurnSpeed.z = outVec.z * 0.02f;
                    }   
                }
            }
        }
    }
}

void (*CWeapon_DoBulletImpact)(void* this, CEntity* pFiringEntity, CEntity* pCollideEntity, CVector* vecStart, CVector* vecEnd, CColPoint* pColPoint, CVector2D vecAhead) = 
(void (*)(void* this, CEntity* pFiringEntity, CEntity* pCollideEntity, CVector* vecStart, CVector* vecEnd, CColPoint* pColPoint, CVector2D vecAhead))0x136AF0;

enum eExplosionType {
    EXPLOSION_GRENADE,
    EXPLOSION_MOLOTOV,
    EXPLOSION_ROCKET,
    EXPLOSION_WEAK_ROCKET,
    EXPLOSION_CAR,
    EXPLOSION_QUICK_CAR,
    EXPLOSION_BOAT,
    EXPLOSION_AIRCRAFT,
    EXPLOSION_MINE,
    EXPLOSION_OBJECT,
    EXPLOSION_TANK_FIRE,
    EXPLOSION_SMALL,
    EXPLOSION_RC_VEHICLE
};

bool (*CExplosion_AddExplosion)(CEntity* victim, CEntity* creator, int explosionType, CVector *posn, unsigned int time, unsigned char makeSound, float camShake, unsigned char visibility) = 
(bool (*)(CEntity* victim, CEntity* creator, int explosionType, CVector *posn, unsigned int time, unsigned char makeSound, float camShake, unsigned char visibility))0x12E1E0;

void (*CWorld_Remove)(CEntity* entity) = (void (*)(CEntity* entity))0x27DF60;

void (*FxSystem_c_Kill)(void* fx_system) = (void (*)(void* fx_system))0x3D5800;

void CWeapon_DoBulletImpactEvent(void* this, CEntity* pFiringEntity, CEntity* pCollideEntity, CVector* vecStart, CVector* vecEnd, CColPoint* pColPoint, CVector2D vecAhead) {
    int i;
    CVector last_exploded_projectile = { 0.0f, 0.0f, 0.0f };
    for (i = 0; i < 32; i++) {
        if (ms_apProjectile[i]) {
            if ( ms_apProjectile[i]->object.parent.entity.m_nModelIndex == MODEL_GRENADE || 
                 ms_apProjectile[i]->object.parent.entity.m_nModelIndex == MODEL_SATCHEL ) {
                if (DistanceBetweenPoints(&pColPoint->m_vecPoint, getVehCoordinates(&ms_apProjectile[i]->object.parent.entity)) < 1.2f ||
                    DistanceBetweenPoints(&last_exploded_projectile, getVehCoordinates(&ms_apProjectile[i]->object.parent.entity)) < 6.5f) {
                    if (gaProjectileInfo[i].m_bActive) {
                        CExplosion_AddExplosion(
                            NULL,
                            pFiringEntity, 
                            EXPLOSION_GRENADE, 
                            getVehCoordinates(&ms_apProjectile[i]->object.parent.entity),
                            gaProjectileInfo[0].m_nDestroyTime,
                            true,
                            -1.0f,
                            false
                        );

                        gaProjectileInfo[i].m_bActive = false;

                        if (gaProjectileInfo[i].m_pFxSystem) {
                            FxSystem_c_Kill(gaProjectileInfo[i].m_pFxSystem);
                            gaProjectileInfo[i].m_pFxSystem = NULL;
                        }

                        CWorld_Remove(&ms_apProjectile[i]->object.parent.entity);

                        ms_apProjectile[i]->object.parent.entity.m_bIsVisible = false;

                        last_exploded_projectile = *getVehCoordinates(&ms_apProjectile[i]->object.parent.entity);

                    }

                }
            }
        }
    }

    CWeapon_DoBulletImpact(this, pFiringEntity, pCollideEntity, vecStart, vecEnd, pColPoint, vecAhead);
}

void (*CTaskSimpleUseGun_AimGun)(void* task, CEntity* ped) = (void (*)(void* task, CEntity* ped))0x407230;

void CTaskSimpleUseGun_AimGunSmoothIK(void* task, CEntity* ped) {

     // MoveState
	if (*(uint32_t*)(((uint32_t)(ped) + 0x570)) <= 4)
	{
		WriteFloat(0x8D2E70, 0.17f);
	}
	else
	{
		WriteFloat(0x8D2E70, 0.3f);
	}
    CTaskSimpleUseGun_AimGun(task, ped);
}

void injectWeaponPatches() {
    RedirectCall(0x133938, CProjectileInfo_UpdateRotation);

    RedirectCall(0x136248, CWeapon_DoBulletImpactEvent);
    RedirectCall(0x136284, CWeapon_DoBulletImpactEvent);
    RedirectCall(0x136394, CWeapon_DoBulletImpactEvent);
    RedirectCall(0x1363D8, CWeapon_DoBulletImpactEvent);
    RedirectCall(0x13A198, CWeapon_DoBulletImpactEvent);
    RedirectCall(0x13B6B4, CWeapon_DoBulletImpactEvent);

    RedirectCall(0x405760, CTaskSimpleUseGun_AimGunSmoothIK);

    // Disable sniper changing moon size
    MakeNop(0x139508);
    MakeNop(0x13950C);
    MakeNop(0x139510);
    MakeNop(0x139514);
}
