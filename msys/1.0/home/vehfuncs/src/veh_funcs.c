
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

CVector* TheCamera_m_vecGameCamPos = (CVector*)(0x6FE530 + 0x908);

uint32_t* CModelInfo_ms_modelInfoPtrs = (uint32_t*)0x7A8780;

void getVehicleDummyPos(CVector* ret, uint16_t veh_id, uint16_t dummy_id) {
	CVector* base = &(*(CVector**)(CModelInfo_ms_modelInfoPtrs[veh_id] + 0x5C))[dummy_id];
	ret->x = base->x;
	ret->y = base->y;
	ret->z = base->z;
}

int (*CDamageManager_GetLightStatus)(uint32_t, uint8_t) = (int (*)(uint32_t, uint8_t))0x162500;

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

void (*CShadows_StoreShadowToBeRendered)(unsigned char, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char) = (
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
    CShadows_StoreShadowToBeRendered(
        5, pos,
        x1, y1, y1, test,
        32, r, g, b);

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


CVector* CTimeCycle_m_VectorToSun = (CVector*)0x709380;

CColModel *(*CEntity_GetColModel)(uint32_t) = (CColModel *(*)(uint32_t))0x232B50;

void CVector2D_Normalise(CVector* a1)
{
    float v1; // st5
    float v2; // st7

    v1 = *(float *)a1 * *(float *)a1 + *(float *)(a1 + 4) * *(float *)(a1 + 4);
    if ( v1 > 0.0f )
    {
        v2 = 1.0f / sqrtf(v1);
        *(float *)a1 = v2 * *(float *)a1;
        *(float *)(a1 + 4) = v2 * *(float *)(a1 + 4);
    }
    else
    {
        *(float *)a1 = 1.0f;
    }
}

typedef struct {
    float          m_fAmbientRed;
    float          m_fAmbientGreen;
    float          m_fAmbientBlue;
    float          m_fAmbientRed_Obj;
    float          m_fAmbientGreen_Obj;
    float          m_fAmbientBlue_Obj;
    unsigned short m_nSkyTopRed;
    unsigned short m_nSkyTopGreen;
    unsigned short m_nSkyTopBlue;
    unsigned short m_nSkyBottomRed;
    unsigned short m_nSkyBottomGreen;
    unsigned short m_nSkyBottomBlue;
    unsigned short m_nSunCoreRed;
    unsigned short m_nSunCoreGreen;
    unsigned short m_nSunCoreBlue;
    unsigned short m_nSunCoronaRed;
    unsigned short m_nSunCoronaGreen;
    unsigned short m_nSunCoronaBlue;
    float          m_fSunSize;
    float          m_fSpriteSize;
    float          m_fSpriteBrightness;
    unsigned short m_nShadowStrength;
    unsigned short m_nLightShadowStrength;
    unsigned short m_nPoleShadowStrength;
    float          m_fFarClip;
    float          m_fFogStart;
    float          m_fLightsOnGroundBrightness;
    unsigned short m_nLowCloudsRed;
    unsigned short m_nLowCloudsGreen;
    unsigned short m_nLowCloudsBlue;
    unsigned short m_nFluffyCloudsBottomRed;
    unsigned short m_nFluffyCloudsBottomGreen;
    unsigned short m_nFluffyCloudsBottomBlue;
    float          m_fWaterRed;
    float          m_fWaterGreen;
    float          m_fWaterBlue;
    float          m_fWaterAlpha;
    float          m_fPostFx1Red;
    float          m_fPostFx1Green;
    float          m_fPostFx1Blue;
    float          m_fPostFx1Alpha;
    float          m_fPostFx2Red;
    float          m_fPostFx2Green;
    float          m_fPostFx2Blue;
    float          m_fPostFx2Alpha;
    float          m_fCloudAlpha;
    unsigned int   m_nHighLightMinIntensity;
    unsigned short m_nWaterFogAlpha;
    float          m_fIllumination;
    float          m_fLodDistMult;

} CColourSet;

void (*CCollision_CalculateTrianglePlanes)(CCollisionData* a1) = (void (*)(CCollisionData* a1))0x11EF30;

void (*CMatrix_mul)(CVector *outPoint, CMatrix *m, CVector *point) = (void (*)(CVector *outPoint, CMatrix *m, CVector *point))0x110310;

void CVehicle_DoSunGlare(int this)
{
    bool v2;
    float *v3;
    int v4;
    float *v5;
    CSimpleTransform *v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    int v14;
    float v15;
    float fwdness;
    float strength;
    float intensity;
    int v19;
    int v20; 
    short *v21; 
    int v22; 
    int v23; 
    int v24; 
    int v25; 
    short *v26; 
    int v27; 
    int v28; 
    int v29; 
    short *v30; 
    int v31; 
    int v32; 
    float v33;
    int v34; 
    int v35; 
    short *v36; 
    int v37; 
    int v38; 
    float v39; 
    int v40; 
    short *v41; 
    int v42; 
    int v43;
    float v44; 
    int v45; 
    short *v46; 
    int v47; 
    int v48; 
    int v49; 
    float v50; 
    float v52; 
    uint8_t v53;
    uint8_t v54; 
    float v55;
    uint8_t v57; 
    uint8_t v58;
    bool v59; 
    uint8_t v60; 
    float v62;
    int v63;
    float radius; 
    CMatrix *v67;
    float dist_;
    int v69; 
    int v70; 
    int v71; 
    int v72; 
    int v73; 
    int v74; 
    float v75;
    int v76;
    int v77;
    CVector v79; 
    float v81;
    float v82;
    float v83;
    float v84;
    int v85; 
    int v86; 
    CVector v87;
    CVector a7; 
    int v90;
    int v91;
    int v92;
    int v93;
    int v94;
    int v95;
    CVector thisa;
    float v97;
    float v98;
    float v99;
    int v101; 
    float v102; 
    float v103; 
    float v104; 
    float v105; 
    float v106; 
    float v107; 
    float v108; 
    float v109; 
    int objects[9]; 
    float v111;
    float v112;
    float v113;
    float v114;
    float v115;
    CVector point;
    float v117;
    float v118;
    float v119;
    CVector outPoint;

	int r, g, b;

    float CWeather_SunGlare = *(float*)0x66BD74;
    int CTimeCycle_m_CurrentStoredValue = *(int*)0x66B9E4;
    CColourSet* CTimeCycle_m_CurrentColours = (CColourSet*)0x7067C0;

    v2 = (*(uint32_t *)(this + 64) & 0x20000000) == 0;
    v101 = this;
    if ( v2
      && *(float *)(*(uint32_t *)(this + 20) + 40) >= 0.0f
      && getVehicleAppearance(this) == 1
      && CWeather_SunGlare > 0.0f )
    {
        vector_constructor_403D60((char *)objects, 12, 4, (int)CVector_CVector_2);
        v3 = *(float **)(this + 20);
        v4 = 0;
        v5 = v3 + 12;
        if ( !v3 )
            v5 = (float *)(this + 4);
        if ( TheCamera->placeable.m_matrix )
            v6 = (CSimpleTransform *)&TheCamera->placeable.m_matrix->pos;
        else
            v6 = &TheCamera->placeable.placement;
        v79.x = v6->m_vPosn.x - *v5;
        v79.y = v6->m_vPosn.y - v5[1];
        v79.z = v6->m_vPosn.z - v5[2];
        dist_ = sqrtf(v79.x * v79.x + v79.z * v79.z + v79.y * v79.y);
        v7 = 2.0f / dist_;
        v81 = v79.x * v7;
        v82 = v79.y * v7;
        v83 = v79.z * v7;
        v8 = v81 + CTimeCycle_m_VectorToSun[CTimeCycle_m_CurrentStoredValue].x;
        v9 = v82 + CTimeCycle_m_VectorToSun[CTimeCycle_m_CurrentStoredValue].y;
        v10 = v83 + CTimeCycle_m_VectorToSun[CTimeCycle_m_CurrentStoredValue].z;
        thisa.x = v10 * v3[2] + v9 * v3[1] + v8 * *v3;
        v11 = v10 * v3[6];
        v12 = v9 * v3[5];
        v13 = v8 * v3[4];
        thisa.z = 0.0f;
        thisa.y = v11 + v12 + v13;
        CVector_Normalise((CVector *)&thisa);
        v14 = *(uint32_t *)(this + 20);
        v15 = *(float *)(v14 + 20);
        v87.x = *(float *)(v14 + 16);
        v87.y = v15;
        v79.y = v82;
        v79.x = v81;
        CVector2D_Normalise(&v87);
        CVector2D_Normalise(&v79);
        fwdness = v79.y * v87.y + v79.x * v87.x;
        if ( fwdness < 0.0f )
            fwdness = -fwdness;
        if ( fwdness <= 0.995f )
        {
            if ( fwdness <= 0.99000001f )
                return;
            strength = (fwdness - 0.99000001f) * 200.0f;
        }
        else
        {
            strength = 1.0f;
        }
        if ( dist_ <= 30.0f )
        {
            if ( dist_ <= 13.0f )
                return;
            strength = strength * (dist_ - 13.0f) * 0.05882353f;
        }
        intensity = strength * 0.80000001f * CWeather_SunGlare;
        r = ((float)(CTimeCycle_m_CurrentColours->m_nSunCoreRed + 510) * intensity * 0.33333334f);
        g = ((float)(CTimeCycle_m_CurrentColours->m_nSunCoreGreen + 510) * intensity * 0.33333334f);
        b = ((float)(CTimeCycle_m_CurrentColours->m_nSunCoreBlue + 510) * intensity * 0.33333334f);

        v19 = (int)CEntity_GetColModel(this)->colData;
        CCollision_CalculateTrianglePlanes((CCollisionData*)v19);
        if ( *(short *)(v19 + 4) - 2 >= 0 )
        {
            do
            {
                v20 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4);
                v21 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v20);
                v22 = *v21;
                v23 = v21[1];
                v24 = v21[2];
                v84 = (float)v22 * 0.0078125f;
                *(float *)objects = v84;
                *(float *)&v85 = (float)v23 * 0.0078125f;
                objects[1] = v85;
                *(float *)&v86 = (float)v24 * 0.0078125f;
                objects[2] = v86;
                if ( *(float *)&v86 > 0.0 )
                {
                    v25 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 2);
                    v26 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v25);
                    v69 = *v26;
                    v27 = v26[1];
                    v28 = v26[2];
                    *(float *)&v90 = (float)v69 * 0.0078125f;
                    objects[3] = v90;
                    *(float *)&v91 = (float)v27 * 0.0078125f;
                    objects[4] = v91;
                    *(float *)&v92 = (float)v28 * 0.0078125f;
                    objects[5] = v92;
                    v29 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 4);
                    v30 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v29);
                    v70 = *v30;
                    v31 = v30[1];
                    v32 = v30[2];
                    v33 = (float)v70 * 0.0078125f;
                    v71 = v31;
                    v34 = 0;
                    *(float *)&v93 = v33;
                    objects[6] = v93;
                    *(float *)&v94 = (float)v71 * 0.0078125f;
                    objects[7] = v94;
                    v72 = 0;
                    *(float *)&v95 = (float)v32 * 0.0078125f;
                    objects[8] = v95;
                    v35 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 8);
                    if ( v35 != v20 && v35 != v25 && v35 != v29 )
                    {
                        v36 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v35);
                        v73 = *v36;
                        v37 = v36[1];
                        v38 = v36[2];
                        v39 = (float)v73 * 0.0078125f;
                        v74 = v37;
                        v34 = 1;
                        v105 = v39;
                        v111 = v105;
                        v106 = (float)v74 * 0.0078125f;
                        v112 = v106;
                        v72 = 1;
                        v107 = (float)v38 * 0.0078125f;
                        v113 = v107;
                    }
                    v40 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 10);
                    if ( v40 != v20 && v40 != v25 && v40 != v29 )
                    {
                        v41 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v40);
                        v76 = *v41;
                        v42 = v41[1];
                        v43 = v41[2];
                        v44 = (float)v76 * 0.0078125f;
                        v77 = v42;
                        v34 = v72 + 1;
                        v97 = v44;
                        v111 = v97;
                        v98 = (float)v77 * 0.0078125f;
                        v112 = v98;
                        v99 = (float)v43 * 0.0078125f;
                        v113 = v99;
                    }
                    v45 = *(unsigned short *)(*(uint32_t *)(v19 + 24) + 8 * v4 + 12);
                    if ( v45 != v20 && v45 != v25 && v45 != v29 )
                    {
                        v46 = (short *)(*(uint32_t *)(v19 + 20) + 6 * v45);
                        v47 = *v46;
                        v48 = v46[1];
                        v49 = v46[2];
                        ++v34;
                        v102 = (float)v47 * 0.0078125f;
                        v111 = v102;
                        v103 = (float)v48 * 0.0078125f;
                        v112 = v103;
                        v104 = (float)v49 * 0.0078125f;
                        v113 = v104;
                    }
                    if ( v34 == 1 )
                    {
                        v119 = *(float *)&v92 + *(float *)&v86;
                        v118 = *(float *)&v90 + v84 + *(float *)&v93;
                        v114 = v111 + v118;
                        v115 = v112 + *(float *)&v91 + *(float *)&v85 + *(float *)&v94;
                        v108 = v114 * 0.25f;
                        v109 = v115 * 0.25f;
                        v50 = v108 - v84;
                        v52 = v50;
                        if ( v53 | v54 )
                            v52 = -v50;
                        v55 = v109 - *(float *)&v85;
                        v75 = v55;
                        if ( v57 | v58 )
                            v55 = -v55;
                        if ( v52 >= v55 )
                        {
                            v50 = v75;
                        }
                        else
                        {
                            v59 = v50 < 0.0f;
                            v60 = 0;
                        }
                        if ( v59 | v60 )
                            v50 = -v50;
                        v62 = v50 * 1.4f;
                        if ( v62 > 0.60000002f )
                        {
                            v63 = v101;
                            v67 = *(CMatrix **)(v101 + 20);
                            v117 = v62 * thisa.z;
                            point.x = thisa.x * v62 + v108;
                            point.y = v62 * thisa.y + v109;
                            point.z = v117 + (v113 + v119 + *(float *)&v95) * 0.25f;
							
                            CMatrix_mul((CVector *)&outPoint, v67, (CVector *)&point);

                            a7.x = outPoint.x + v81;
                            a7.y = outPoint.y + v82;
                            a7.z = outPoint.z + v83;
                            radius = CWeather_SunGlare * 0.89999998f;
                            CCoronas_RegisterCorona(v4 + v63 + 27, NULL, r, g, b, 255, &a7, radius, 90.0f, 0, 0, 0, 0, 0, 0, 0, 1.5f, 0, 15.0f, 0, 0);
                        }
                    }
                }
                v4 += 2;
            }
            while ( v4 <= *(short *)(v19 + 4) - 2 );
        }
    }
}
void (*CShadows_StoreShadowForVehicle)(uint32_t, uint32_t) = (void (*)(uint32_t, uint32_t))0x1142A0;

void SunGlareInject(uint32_t vehicle, uint32_t a2) {
    CShadows_StoreShadowForVehicle(vehicle, a2);
    CVehicle_DoSunGlare(vehicle);
}


void hookedVehicleRender(uint32_t vehicle) {
	CPad* pad = CPad_GetPad(0);
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

	if(pad->NewState.DPadRight && !pad->OldState.DPadRight && vehicle == FindPlayerVehicle(-1, 0)) {
		if(test_bit(veh_lights, 4)) {
			clear_bit(veh_lights, 4);
			set_bit(veh_lights, 3);
		} else {
			set_bit(veh_lights, 4);
			clear_bit(veh_lights, 3);
		}
		setVehicleBombLightsWinchFlags(vehicle, veh_lights);
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

void (*Camera_FollowCar)(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) = (void (*)(uint32_t, CVector *, int, int, int, char))0x209C90;

void VCarCamera(uint32_t this, CVector *a2, float a3, float a4, float a5, char a6) {
    CVector out, obj_space, ret;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.7f;

    sub_25F980(&ret, a2, &out);
    Camera_FollowCar(this, &ret, a3, a4, a5, a6);
}

short CPad_GetAccelerate(CPad *this)
{
    short result;

    if ( this->DisablePlayerControls || this->Mode > 3 )
        return 0;
    switch ( this->Mode )
    {
        case 0:
        case 1:
        case 2:
            result = this->NewState.RightShoulder2;
            break;
        case 3:
            result = -2 * this->NewState.RightStickY < 0 ? 0 : (-2 * this->NewState.RightStickY);
            break;
        default:
            return 0;
    }
    return result;
}

short CPad_GetBrake(CPad *this)
{
    short result;

    if ( this->DisablePlayerControls || this->Mode > 3 )
        return 0;
    switch ( this->Mode )
    {
        case 0:
        case 1:
        case 2:
            result = this->NewState.LeftShoulder2;
            break;
        case 3:
            result = (this->NewState.RightStickY & 0x40000000) != 0 ? 0 : (2 * this->NewState.RightStickY);
            break;
        default:
            return 0;
    }
    return result;
}

short CPad_GetHandBrake(CPad *this)
{
    short result;

    if ( this->DisablePlayerControls || this->Mode > 3 )
        return 0;
    switch ( this->Mode )
    {
        case 0:
        case 1:
            result = this->NewState.ButtonCross;
            break;
        case 2:
            result = this->NewState.ButtonTriangle;
            break;
        case 3:
            result = this->NewState.LeftShoulder1;
            break;
        default:
            return 0;
    }
    return result;
}

int CPad_GetLookLeft(CPad *this)
{
    short v1;
    short v2;

    if ( this->DisablePlayerControls )
        return 0;
    v1 = this->NewState.LeftShoulder2;
    if ( v1 )
    {
        if ( !this->OldState.LeftShoulder2 )
            return 0;
    }
    v2 = this->NewState.RightShoulder2;
    if ( !v2 )
    {
        if ( this->OldState.RightShoulder2 )
            return 0;
    }
    return v1 && !v2;
}

int CPad_GetLookRight(CPad *this)
{
    short v1; // dx
    short v2; // ax

    if ( this->DisablePlayerControls )
        return 0;
    v1 = this->NewState.RightShoulder2;
    if ( v1 )
    {
        if ( !this->OldState.RightShoulder2 )
            return 0;
    }
    v2 = this->NewState.LeftShoulder2;
    if ( !v2 )
    {
        if ( this->OldState.LeftShoulder2 )
            return 0;
    }
    return v1 && !v2;
}

int CPad_GetLookLeftRight(CPad *this)
{
    if ( !this->DisablePlayerControls )
        return (this->NewState.RightShoulder2 - this->NewState.LeftShoulder2) / 2;
    return 0;
}

int _start() {
    RedirectCall(0x2029EC, VCarCamera);
    RedirectCall(0x210FE8, VCarCamera);
    RedirectCall(0x211080, VCarCamera);
    RedirectCall(0x59BD40, VCarCamera);

	RedirectCall(0x155254, hookedExitVehicle);

	RedirectCall(0x154328, hookedVehicleRender);
    WriteDword(0x65B990, (uint32_t)hookedVehicleRender);

    RedirectFunction(0x249FF0, CPad_GetAccelerate);
    RedirectFunction(0x249930, CPad_GetBrake);
    RedirectFunction(0x2498B0, CPad_GetHandBrake);

    RedirectCall(0x152D88, SunGlareInject);
    RedirectCall(0x152D9C, SunGlareInject);
    RedirectCall(0x152DB4, SunGlareInject);

    return 0;
}
