
#include <CPad.h>
#include <bit.h>
#include <CVector.h>
#include <CVehicle.h>
#include <CCoronas.h>
#include <injector.h>
#include <common.h>
#include <CCam.h>
#include <math.h>

#include "hooks.h"

#define MAX_RADIUS 300.0f

static CVector* TheCamera_m_vecGameCamPos = (CVector*)(0x6FE530 + 0x908);

extern uint32_t CModelInfo_ms_modelInfoPtrs[];

void getVehicleDummyPos(CVector* ret, uint16_t veh_id, uint16_t dummy_id) {
	CVector* base = &(*(CVector**)(CModelInfo_ms_modelInfoPtrs[veh_id] + 0x5C))[dummy_id];
	ret->x = base->x;
	ret->y = base->y;
	ret->z = base->z;
}

static int (*CDamageManager_GetLightStatus)(uint32_t, uint8_t) = (int (*)(uint32_t, uint8_t))0x162500;

float getVehicleHeading(uint32_t vehicle) {
	uint32_t v70;
	float v71;

    v70 = *(uint32_t *)(vehicle + 20);
    if ( v70 )
        v71 = atan2f(-*(float *)(v70 + 16), *(float *)(v70 + 20));
    else
        v71 = *(float *)(vehicle + 16);
	return v71;
}

static void (*CShadows_StoreShadowToBeRendered)(unsigned char, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char) = (
	void (*)(unsigned char, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char)
)0x113FF0;

void drawRearLights(CVector *pos, float a, float length, uint8_t r, uint8_t g, uint8_t b) {
	float x1 = 0.0f, y1 = 0.0f;

    if ( a == 0.0f ) {
        x1 = length;
        y1 = 0.0f;
    } else {
       x1 = cosf(a) * length;
       y1 = sinf(a) * length;
    }

	float test = x1 * -1.0f;
    CShadows_StoreShadowToBeRenderedSingle(
        2, *(RwTexture**)0x66B218, pos,
        x1, y1, y1, test,
        32, r, g, b, 
		15.0f, false,
		1.0f,
		NULL,
		true
		);

}

static void DrawVehicleReverselights(uint32_t vehicle) {
	CVector posn;
    getVehicleDummyPos(&posn, *(uint16_t*)(vehicle + 0x22), 1);
	if (posn.x == 0.0f) posn.x = 0.15f;
	posn.x -= 0.1f;

	float light_offset = posn.x;

	posn.x = 0.0f;

	CVector* veh_pos = getVehCoordinates(vehicle);
	float angle = getVehicleHeading(vehicle);
	CVector shadow_draw;

    shadow_draw.x = (posn.x) * cosf(angle) - (posn.y-1.1f) * sinf(angle);
    shadow_draw.y = (posn.y-1.1f) * cosf(angle) + (posn.x) * sinf(angle);
	shadow_draw.z = posn.z;
	
	shadow_draw.x += veh_pos->x;
	shadow_draw.y += veh_pos->y;
	shadow_draw.z += veh_pos->z;

	drawRearLights(&shadow_draw, angle - 1.5708f, 0.9f*(1.0f+light_offset), 32, 32, 32);

	posn.x = light_offset;

	if (!CDamageManager_GetLightStatus((vehicle + 1504), LIGHT_REAR_RIGHT)) {
		CCoronas_RegisterCorona((vehicle + 50 + 4), vehicle, 255, 255, 255, 255, &posn,
								 0.2f, 250.0f, CORONATYPE_SHINYSTAR, FLARETYPE_NONE, 0, 0, 0, 0.0f, 0, 0.5f, 0, 50.0f, 0, 1);
	}
	if (!CDamageManager_GetLightStatus((vehicle + 1504), LIGHT_REAR_LEFT)) {
		posn.x *= -1.0f;
		CCoronas_RegisterCorona((vehicle + 50 + 5), vehicle, 255, 255, 255, 255, &posn,
								 0.2f, 250.0f, CORONATYPE_SHINYSTAR, FLARETYPE_NONE, 0, 0, 0, 0.0f, 0, 0.5f, 0, 50.0f, 0, 1);
	}
}

void hookedVehicleRender(uint32_t vehicle) {
	uint8_t veh_lights = getVehicleBombLightsWinchFlags(vehicle);

	if ((getVehicleSubClass(vehicle) == VEHICLE_AUTOMOBILE || getVehicleSubClass(vehicle) == VEHICLE_BIKE) &&
	(getVehicleAppearance(vehicle) == VEHICLE_APPEARANCE_AUTOMOBILE || getVehicleAppearance(vehicle) == VEHICLE_APPEARANCE_BIKE) &&
	getVehicleFlags(vehicle)->bEngineOn && !*(char*)(0x66B310) && getVehicleHealth(vehicle) > 0.0f && !getVehicleFlags(vehicle)->bIsDrowning && !attachedTo(vehicle))
	{
		if (test_bit(veh_lights, 4)) {
			CVector posn;
            getVehicleDummyPos(&posn, *(uint16_t*)(vehicle + 0x22), 1);
			if (posn.x == 0.0f) posn.x = 0.15f;

			float light_offset = posn.x;

			posn.x = 0.0f;

			CVector* veh_pos = getVehCoordinates(vehicle);
			float angle = getVehicleHeading(vehicle);
			CVector shadow_draw;

    		shadow_draw.x = (posn.x) * cosf(angle) - (posn.y-1.1f) * sinf(angle);
    		shadow_draw.y = (posn.y-1.1f) * cosf(angle) + (posn.x) * sinf(angle);
			shadow_draw.z = posn.z;

			shadow_draw.x += veh_pos->x;
			shadow_draw.y += veh_pos->y;
			shadow_draw.z += veh_pos->z;

			drawRearLights(&shadow_draw, angle - 1.5708f, (1.0f+light_offset), 20, 0, 0);
		}

		if (getVehicleSubClass(vehicle) != VEHICLE_BIKE) {
			if (getCurrentGear(vehicle) == 0) {
				if (DistanceBetweenPoints(TheCamera_m_vecGameCamPos, getVehCoordinates(vehicle)) < MAX_RADIUS) {
					DrawVehicleReverselights(vehicle);
					if (*(uint32_t*)((uint32_t)vehicle + 1228))
						DrawVehicleReverselights(*(uint32_t*)((uint32_t)vehicle + 1228));
				}
			}
		}
	}
	
	CVehicle_Render(vehicle);
}

int (*get_exit_vehicle)(int a1) = (int (*)(int a1))0x2499D0;

int hookedExitVehicle(int a1) {
	int ret = get_exit_vehicle(a1);
	if(ret) {
		CVehicle__SetEngineState(FindPlayerVehicle(-1, 0), false);
	}
	return ret;
}

static void (*Camera_FollowCar)(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) = (void (*)(uint32_t, CVector *, int, int, int, char))0x209C90;

void VCarCamera(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) {
    CVector out, obj_space, ret;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.7f;

    sub_25F980(&ret, a2, &out);
    Camera_FollowCar(this, &ret, a3, a4, a5, a6);
}

void (*CAutomobile_ProcessControlInputs)(uint32_t vehicle, uint8_t a2) = (void (*)(uint32_t vehicle, uint8_t a2))0x155170;

void MyCAutomobile_ProcessControlInputs(uint32_t vehicle, uint8_t a2) {
	CPad* pad = CPad_GetPad(0);
	uint8_t veh_lights = getVehicleBombLightsWinchFlags(vehicle);

	CAutomobile_ProcessControlInputs(vehicle, a2);

	if(pad->NewState.DPadRight && !pad->OldState.DPadRight) {
		if(test_bit(veh_lights, 4)) {
			clear_bit(veh_lights, 4);
			set_bit(veh_lights, 3);
		} else {
			set_bit(veh_lights, 4);
			clear_bit(veh_lights, 3);
		}
		setVehicleBombLightsWinchFlags(vehicle, veh_lights);
	}
}

void CalculateDoorLock() {
    uint32_t *vehicle;
    int lockChance = rand() % 101;

    asm("move %0, $s0" : "=r" (vehicle)); // get from our parent function

    if (lockChance < 85) {
        *(uint32_t*)((uint32_t)vehicle + 0x500) = 2; // door lock = locked
    } else {
        *(uint32_t*)((uint32_t)vehicle + 0x500) = 1; // door lock = unlocked
    }
    
}

void injectVehiclePatches() {
    RedirectCall(0x2029EC, VCarCamera);
    RedirectCall(0x210FE8, VCarCamera);
    RedirectCall(0x211080, VCarCamera);
    RedirectCall(0x59BD40, VCarCamera);
    RedirectCall(0x2956BC, CalculateDoorLock);

	RedirectCall(0x155254, hookedExitVehicle);

	RedirectCall(0x154328, hookedVehicleRender);
    WriteDword(0x65B990, (uint32_t)hookedVehicleRender);

	WriteDword(0x65B6C8, (uint32_t)MyCAutomobile_ProcessControlInputs);
}
