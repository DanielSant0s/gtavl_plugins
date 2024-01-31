#include <stdio.h>
#include <injector.h>
#include <CCam.h>
#include <common.h>
#include <rwcore.h>
#include <CTxdStore.h>
#include <CVector.h>

#include "hooks.h"

void (*CPed_PreRenderProcessFire)(uint32_t);

void CShadows_AddPermanentShadow(uint8_t type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, uint8_t red, uint8_t greeb, uint8_t blue, float drawDistance, uint32_t time, float upDistance);


static bool AddedOil = false;
static uint32_t AddedOilTime = 0;

void AddOilPuddle(RwTexture* texture, CVector* pos, int r, int g, int b, int a, float size) {
    CShadows_AddPermanentShadow(4,
	texture, pos,
	0.4f, 0.0f, 0.0f, -0.4f,
	a,
	r,
	g,
	b,
	4.0f, 40000, 1.0);

	AddedOil = true;
	AddedOilTime = CTimer_m_snTimeInMilliseconds + 30000;
}

static void ProcessFireCPed_PreRender(uint32_t this) {
    void* playa = FindPlayerPed(0);
    CWeapon* weapons = getPedWeapons(playa);
	bool hasGasoline = weapons[10].m_nType == WEAPON_DILDO2;

	if (hasGasoline && AddedOil) {
        if (GetNumFiresInRange(&gFireManager, getCharCoordinates((uint32_t*)this), 2.0f) > 0 ) {
			StartFireEntity(&gFireManager, (CEntity*)this, playa, 0.8f, 1, 7000, 0);
		}
    }

	if ((CTimer_m_snTimeInMilliseconds > AddedOilTime) && AddedOil) {
		AddedOil = false;
		AddedOilTime = 0;
	}
	
    CPed_PreRenderProcessFire(this);
}

void installOilSystem() {
	CPed_PreRenderProcessFire = (void (*)(uint32_t))ReadCall(0x19A728);
	RedirectCall(0x19A728, &ProcessFireCPed_PreRender);

}
