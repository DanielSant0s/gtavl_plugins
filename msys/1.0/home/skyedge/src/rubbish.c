#include "rubbish.h"
#include "renderbuffer.h"

#include <stdio.h>
#include <injector.h>
#include <CCam.h>
#include <common.h>
#include <rwcore.h>
#include <CTxdStore.h>
#include <CVector.h>
#include "utility.h"

#include "modules.h"

static void (*CVehicle_UpdatePassengerList)(void* veh) = (void (*)(void*))0x173F40;
static void (*CGame_ShutdownRenderWare)() = (void (*)())0x242AB0;
static void (*RenderEffects)() = (void (*)())0x246E80;
static void (*CGame_Init1)(const char*) = (void (*)(const char*))0x88E430;

#define RUBBISH_MAX_DIST (18.0f)
#define RUBBISH_FADE_DIST (16.5f)

static RwTexture* gpRubbishTexture[4];
static RwImVertexIndex RubbishIndexList[6];
static RwImVertexIndex RubbishIndexList2[6];
static RxObjSpace3dVertex RubbishVertices[4];
static bool CRubbish_bRubbishInvisible;
static int CRubbish_RubbishVisibility;
static COneSheet CRubbish_aSheets[NUM_RUBBISH_SHEETS];
static COneSheet CRubbish_StartEmptyList;
static COneSheet CRubbish_EndEmptyList;
static COneSheet CRubbish_StartStaticsList;
static COneSheet CRubbish_EndStaticsList;
static COneSheet CRubbish_StartMoversList;
static COneSheet CRubbish_EndMoversList;

static const float aAnimations[3][34] = {
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },

	{ 0.0f, 0.05f, 0.12f, 0.25f, 0.42f, 0.57f, 0.68f, 0.8f, 0.86f, 0.9f, 0.93f, 0.95f, 0.96f, 0.97f, 0.98f, 0.99f, 1.0f,
	0.15f, 0.35f, 0.6f, 0.9f, 1.2f, 1.25f, 1.3f, 1.2f, 1.1f, 0.95f, 0.8f, 0.6f, 0.45f, 0.3f, 0.2f, 0.1f, 0 },

	{ 0.0f, 0.05f, 0.12f, 0.25f, 0.42f, 0.57f, 0.68f, 0.8f, 0.95f, 1.1f, 1.15f, 1.18f, 1.15f, 1.1f, 1.05f, 1.03f, 1.0f,
	0.15f, 0.35f, 0.6f, 0.9f, 1.2f, 1.25f, 1.3f, 1.2f, 1.1f, 0.95f, 0.8f, 0.6f, 0.45f, 0.3f, 0.2f, 0.1f, 0 }
};

CVector* getCameraPosition() {
    if ( TheCamera.placeable.m_matrix ) {
        return &TheCamera.placeable.m_matrix->pos;
    }

    return &TheCamera.placeable.placement.m_vPosn;
}

float MagnitudeSqr2D(CVector* v) { return v->x * v->x + v->y * v->y; }

float Magnitude2D(CVector* v) { return sqrtf(v->x * v->x + v->y * v->y); }

float DistanceBetweenPointsSqr(CVector* v1, CVector* v2) {
    return (v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y);
}

static CMatrix * (*CPlaceable_AllocateMatrix)(CPlaceable *this) = (CMatrix * (*)(CPlaceable *this))0x260090;

static void (*CSimpleTransform_UpdateMatrix)(CSimpleTransform *this, CMatrix *matrix) = (void (*)(CSimpleTransform *this, CMatrix *matrix))0x25FAC0;

inline CMatrix * CPlaceable_GetMatrix(CPlaceable *this)
{
    if ( !this->m_matrix )
    {
        CPlaceable_AllocateMatrix(this);
        CSimpleTransform_UpdateMatrix(&this->placement, this->m_matrix);
    }
    return this->m_matrix;
}

enum eZoneAttributes {
    ATTRZONE_NONE = 0x0,
    ATTRZONE_CAMCLOSEIN = 0x1,
    ATTRZONE_STAIRS = 0x2,
    ATTRZONE_1STPERSONS = 0x4,
    ATTRZONE_NORAIN = 0x8,
    ATTRZONE_NOPOLICE = 0x10,
    ATTRZONE_DOINEEDCOLLISION = 0x40,
    ATTRZONE_POLICEABANDONCAR = 0x100,
    ATTRZONE_INROOMSFORAUDIO = 0x200,
    ATTRZONE_INROOMSFEWERPEDS = 0x400,
    ATTRZONE_MILITARYZONE = 0x1000,
    ATTRZONE_EXTRAAIRRESISTANCE = 0x4000,
    ATTRZONE_FEWERCARS = 0x8000,
};

static int (*CCullZones_FindAttributesForCoors)(CVector *a1) = (int (*)(CVector *a1))0X306AC0;

void CRubbish_Init(const char* a1) {
    CGame_Init1(a1);

    int i;
	for (i = 0; i < NUM_RUBBISH_SHEETS; i++) {
		CRubbish_aSheets[i].m_state = 0;
		if (i < NUM_RUBBISH_SHEETS - 1)
			CRubbish_aSheets[i].m_next = &CRubbish_aSheets[i + 1];
		else
			CRubbish_aSheets[i].m_next = &CRubbish_EndEmptyList;
		if (i > 0)
			CRubbish_aSheets[i].m_prev = &CRubbish_aSheets[i - 1];
		else
			CRubbish_aSheets[i].m_prev = &CRubbish_StartEmptyList;
	}

	CRubbish_StartEmptyList.m_next = &CRubbish_aSheets[0];
	CRubbish_StartEmptyList.m_prev = NULL;
	CRubbish_EndEmptyList.m_next = NULL;
	CRubbish_EndEmptyList.m_prev = &CRubbish_aSheets[NUM_RUBBISH_SHEETS - 1];

	CRubbish_StartStaticsList.m_next = &CRubbish_EndStaticsList;
	CRubbish_StartStaticsList.m_prev = NULL;
	CRubbish_EndStaticsList.m_next = NULL;
	CRubbish_EndStaticsList.m_prev = &CRubbish_StartStaticsList;

	CRubbish_StartMoversList.m_next = &CRubbish_EndMoversList;
	CRubbish_StartMoversList.m_prev = NULL;
	CRubbish_EndMoversList.m_next = NULL;
	CRubbish_EndMoversList.m_prev = &CRubbish_StartMoversList;

	RubbishVertices[0].u = 0.0f;
	RubbishVertices[0].v = 0.0f;
	RubbishVertices[1].u = 1.0f;
	RubbishVertices[1].v = 0.0f;
	RubbishVertices[2].u = 0.0f;
	RubbishVertices[2].v = 1.0f;
	RubbishVertices[3].u = 1.0f;
	RubbishVertices[3].v = 1.0f;

	RubbishIndexList2[0] = 0;
	RubbishIndexList2[1] = 2;
	RubbishIndexList2[2] = 1;
	RubbishIndexList2[3] = 1;
	RubbishIndexList2[4] = 2;
	RubbishIndexList2[5] = 3;

	RubbishIndexList[0] = 0;
	RubbishIndexList[1] = 1;
	RubbishIndexList[2] = 2;
	RubbishIndexList[3] = 1;
	RubbishIndexList[4] = 3;
	RubbishIndexList[5] = 2;

	CTxdStore_PushCurrentTxd();
	int slot = CTxdStore_FindTxdSlot("particle");

	CTxdStore_SetCurrentTxd(slot);
	gpRubbishTexture[0] = RwTextureRead("gameleaf01_64", NULL);
	gpRubbishTexture[1] = RwTextureRead("gameleaf02_64", NULL);
	gpRubbishTexture[2] = RwTextureRead("newspaper01_64", NULL);
	gpRubbishTexture[3] = RwTextureRead("newspaper02_64", NULL);
	CTxdStore_PopCurrentTxd();
	CRubbish_RubbishVisibility = 255;
	CRubbish_bRubbishInvisible = false;
}

void CRubbish_Shutdown() {
	RwTextureDestroy(gpRubbishTexture[0]);
	gpRubbishTexture[0] = NULL;

	RwTextureDestroy(gpRubbishTexture[1]);
	gpRubbishTexture[1] = NULL;

	RwTextureDestroy(gpRubbishTexture[2]);
	gpRubbishTexture[2] = NULL;

	RwTextureDestroy(gpRubbishTexture[3]);
	gpRubbishTexture[3] = NULL;

    CGame_ShutdownRenderWare();
}

void CRubbish_Render() {
	if (CGame_currArea > 0)
		return;

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)false);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)true);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)true);

    int i;
	for (i = 0; i < 4; i++) {
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, gpRubbishTexture[i]->raster);

		TempBufferIndicesStored = 0;
		TempBufferVerticesStored = 0;

		COneSheet* sheet;
		for (sheet = &CRubbish_aSheets[i * NUM_RUBBISH_SHEETS / 4];
			sheet < &CRubbish_aSheets[(i + 1) * NUM_RUBBISH_SHEETS / 4];
			sheet++) {

			if (sheet->m_state == 0)
				continue;

			uint32_t alpha = 128;
			CVector pos;
			if (sheet->m_state == 1) {
				pos = sheet->m_basePos;
				if (!sheet->m_isVisible)
					alpha = 0;
			}
			else {
				pos = sheet->m_animatedPos;
				if (!sheet->m_isVisible || !sheet->m_targetIsVisible) {
					float t = (float)(CTimer_m_snTimeInMilliseconds - sheet->m_moveStart) / sheet->m_moveDuration;
					float f1 = sheet->m_isVisible ? 1.0f - t : 0.0f;
					float f2 = sheet->m_targetIsVisible ? t : 0.0f;
					alpha = 128 * (f1 + f2);
				}
			}

			float camDist = DistanceBetweenPoints(&pos, getCameraPosition());
			if (camDist < RUBBISH_MAX_DIST) {
				if (camDist >= RUBBISH_FADE_DIST)
					alpha -= alpha * (camDist - RUBBISH_FADE_DIST) / (RUBBISH_MAX_DIST - RUBBISH_FADE_DIST);
				alpha = (CRubbish_RubbishVisibility * alpha) / 256;

				float vx = sinf(sheet->m_angle) * 0.4f;
				float vy = cosf(sheet->m_angle) * 0.4f;

				int v = TempBufferVerticesStored;

                TempBufferRenderVertices[v + 0].pos.x = pos.x + vx;
                TempBufferRenderVertices[v + 0].pos.y = pos.y + vy;
                TempBufferRenderVertices[v + 0].pos.z = pos.z;
                TempBufferRenderVertices[v + 0].color.r = 255;
                TempBufferRenderVertices[v + 0].color.g = 255;
                TempBufferRenderVertices[v + 0].color.b = 255;
                TempBufferRenderVertices[v + 0].color.a = alpha;

                TempBufferRenderVertices[v + 1].pos.x = pos.x - vy;
                TempBufferRenderVertices[v + 1].pos.y = pos.y + vx;
                TempBufferRenderVertices[v + 1].pos.z = pos.z;
                TempBufferRenderVertices[v + 1].color.r = 255;
                TempBufferRenderVertices[v + 1].color.g = 255;
                TempBufferRenderVertices[v + 1].color.b = 255;
                TempBufferRenderVertices[v + 1].color.a = alpha;

                TempBufferRenderVertices[v + 2].pos.x = pos.x + vy;
                TempBufferRenderVertices[v + 2].pos.y = pos.y - vx;
                TempBufferRenderVertices[v + 2].pos.z = pos.z;
                TempBufferRenderVertices[v + 2].color.r = 255;
                TempBufferRenderVertices[v + 2].color.g = 255;
                TempBufferRenderVertices[v + 2].color.b = 255;
                TempBufferRenderVertices[v + 2].color.a = alpha;

                TempBufferRenderVertices[v + 3].pos.x = pos.x - vy;
                TempBufferRenderVertices[v + 3].pos.y = pos.y - vx;
                TempBufferRenderVertices[v + 3].pos.z = pos.z;
                TempBufferRenderVertices[v + 3].color.r = 255;
                TempBufferRenderVertices[v + 3].color.g = 255;
                TempBufferRenderVertices[v + 3].color.b = 255;
                TempBufferRenderVertices[v + 3].color.a = alpha;

				TempBufferRenderVertices[v + 0].u = 0.0f;
				TempBufferRenderVertices[v + 0].v = 0.0f;
				TempBufferRenderVertices[v + 1].u = 1.0f;
				TempBufferRenderVertices[v + 1].v = 0.0f;
				TempBufferRenderVertices[v + 2].u = 0.0f;
				TempBufferRenderVertices[v + 2].v = 1.0f;
				TempBufferRenderVertices[v + 3].u = 1.0f;
				TempBufferRenderVertices[v + 3].v = 1.0f;

				int j = TempBufferIndicesStored;
				TempBufferRenderIndexList[j + 0] = RubbishIndexList[0] + TempBufferVerticesStored;
				TempBufferRenderIndexList[j + 1] = RubbishIndexList[1] + TempBufferVerticesStored;
				TempBufferRenderIndexList[j + 2] = RubbishIndexList[2] + TempBufferVerticesStored;
				TempBufferRenderIndexList[j + 3] = RubbishIndexList[3] + TempBufferVerticesStored;
				TempBufferRenderIndexList[j + 4] = RubbishIndexList[4] + TempBufferVerticesStored;
				TempBufferRenderIndexList[j + 5] = RubbishIndexList[5] + TempBufferVerticesStored;
				TempBufferVerticesStored += 4;
				TempBufferIndicesStored += 6;
			}
		}

		if (TempBufferIndicesStored != 0) {
			if (RwIm3DTransform(TempBufferRenderVertices, TempBufferVerticesStored, NULL, 1)) {
				RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, TempBufferRenderIndexList, TempBufferIndicesStored);
				RwIm3DEnd();
			}
		}
	}

	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)false);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)false);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)true);
}

void CRubbish_StirUp(CPhysical* veh) {
	if ((CTimer_m_FrameCounter ^ (veh->entity.m_nRandomSeed & 3)) == 0)
		return;

	if (fabsf(getVehCoordinates((uint32_t*)veh)->x - getCameraPosition()->x) < 20.0f &&
		fabsf(getVehCoordinates((uint32_t*)veh)->y - getCameraPosition()->y) < 20.0f)
		if (fabsf(veh->m_vecMoveSpeed.x) > 0.05f || fabsf(veh->m_vecMoveSpeed.y) > 0.05f) {
			float speed = Magnitude2D(&veh->m_vecMoveSpeed);
			if (speed > 0.05f) {
				bool movingForward = DotProduct2D((CVector2D*)&veh->m_vecMoveSpeed, (CVector2D*)&CPlaceable_GetMatrix(&veh->entity.placeable)->up) > 0.0f;
				COneSheet* sheet = CRubbish_StartStaticsList.m_next;
				CVector size = CEntity_GetColModel((uint32_t)veh)->box.max;

				while (sheet != &CRubbish_EndStaticsList) {
					COneSheet* next = sheet->m_next;
					CVector2D carToSheet;
                    carToSheet.x = sheet->m_basePos.x - getVehCoordinates((uint32_t*)veh)->x;
                    carToSheet.y = sheet->m_basePos.y - getVehCoordinates((uint32_t*)veh)->y;

					float distFwd = DotProduct2D((CVector2D*)&carToSheet, (CVector2D*)&CPlaceable_GetMatrix(&veh->entity.placeable)->up);

					if (movingForward && distFwd < -0.5f * size.y && distFwd > -1.5f * size.y ||
						!movingForward && distFwd > 0.5f * size.y && distFwd < 1.5f * size.y) {
						float distSide = fabsf(DotProduct2D((CVector2D*)&carToSheet, (CVector2D*)&CPlaceable_GetMatrix(&veh->entity.placeable)->right));
						if (distSide < 1.5f * size.x) {
							float speedToCheck = distSide < size.x ? speed : speed * 0.5f;
							if (speedToCheck > 0.05f) {
								sheet->m_state = 2;
								if (speedToCheck > 0.15f)
									sheet->m_animationType = 2;
								else
									sheet->m_animationType = 1;
								sheet->m_moveDuration = 2000;
								sheet->m_xDist = veh->m_vecMoveSpeed.x;
								sheet->m_yDist = veh->m_vecMoveSpeed.y;
								float dist = sqrtf(SQR(sheet->m_xDist) + SQR(sheet->m_yDist));
								sheet->m_xDist *= 25.0f * speed / dist;
								sheet->m_yDist *= 25.0f * speed / dist;
								sheet->m_animHeight = 3.0f * speed;
								sheet->m_moveStart = CTimer_m_snTimeInMilliseconds;
								float tx = sheet->m_basePos.x + sheet->m_xDist;
								float ty = sheet->m_basePos.y + sheet->m_yDist;
								float tz = sheet->m_basePos.z + 3.0f;
								sheet->m_targetZ = CWorld_FindGroundZFor3DCoord(tx, ty, tz, NULL, NULL) + 0.1f;
								COneSheet_RemoveFromList(sheet);
								COneSheet_AddToList(sheet, &CRubbish_StartMoversList);
							}
						}
					}

					sheet = next;
				}
			}
		}

    CVehicle_UpdatePassengerList(veh);
}

void CRubbish_Update() {
	char foundGround = false;

	if (CRubbish_bRubbishInvisible)
		CRubbish_RubbishVisibility = max(CRubbish_RubbishVisibility - 5, 0);
	else
		CRubbish_RubbishVisibility = min(CRubbish_RubbishVisibility + 5, 255);

	COneSheet* sheet = CRubbish_StartEmptyList.m_next;
	if (sheet != &CRubbish_EndEmptyList) {
		float spawnDist;
		float spawnAngle;

		spawnDist = (GetRandomNumber() & 0xFF) / 256.0f + RUBBISH_MAX_DIST;
		uint8_t r = GetRandomNumber();
		if (r & 1)
			spawnAngle = (GetRandomNumber() & 0xFF) / 256.0f * 6.28f;
		else
			spawnAngle = (r - 128) / 160.0f + TheCamera.m_fOrientation;
		sheet->m_basePos.x = getCameraPosition()->x + spawnDist * sinf(spawnAngle);
		sheet->m_basePos.y = getCameraPosition()->y + spawnDist * cosf(spawnAngle);
		sheet->m_basePos.z = CWorld_FindGroundZFor3DCoord(sheet->m_basePos.x, sheet->m_basePos.y, getCameraPosition()->z, &foundGround, NULL) + 0.1f;
		if (foundGround) {
			sheet->m_angle = (GetRandomNumber() & 0xFF) / 256.0f * 6.28f;
			sheet->m_state = 1;
			if (CCullZones_FindAttributesForCoors(&sheet->m_basePos) & ATTRZONE_NORAIN)
				sheet->m_isVisible = false;
			else
				sheet->m_isVisible = true;
			COneSheet_RemoveFromList(sheet);
			COneSheet_AddToList(sheet, &CRubbish_StartStaticsList);
		}
	}

	bool hit = false;
	sheet = CRubbish_StartMoversList.m_next;
	while (sheet != &CRubbish_EndMoversList) {
		uint32_t currentTime = CTimer_m_snTimeInMilliseconds - sheet->m_moveStart;
		if (currentTime < sheet->m_moveDuration) {
			int step = 16 * currentTime / sheet->m_moveDuration;
			int stepTime = sheet->m_moveDuration / 16;
			float s = (float)(currentTime - stepTime * step) / stepTime;
			float t = (float)currentTime / sheet->m_moveDuration;
			float fxy = aAnimations[sheet->m_animationType][step] * (1.0f - s) + aAnimations[sheet->m_animationType][step + 1] * s;
			float fz = aAnimations[sheet->m_animationType][step + 17] * (1.0f - s) + aAnimations[sheet->m_animationType][step + 1 + 17] * s;
			sheet->m_animatedPos.x = sheet->m_basePos.x + fxy * sheet->m_xDist;
			sheet->m_animatedPos.y = sheet->m_basePos.y + fxy * sheet->m_yDist;
			sheet->m_animatedPos.z = (1.0f - t) * sheet->m_basePos.z + t * sheet->m_targetZ + fz * sheet->m_animHeight;
			sheet->m_angle += CTimer_ms_fTimeStep * 0.04f;
			if (sheet->m_angle > 6.28f)
				sheet->m_angle -= 6.28f;
			sheet = sheet->m_next;
		}
		else {
			sheet->m_basePos.x += sheet->m_xDist;
			sheet->m_basePos.y += sheet->m_yDist;
			sheet->m_basePos.z = sheet->m_targetZ;
			sheet->m_state = 1;
			sheet->m_isVisible = sheet->m_targetIsVisible;

			COneSheet* next = sheet->m_next;
			COneSheet_RemoveFromList(sheet);
			COneSheet_AddToList(sheet, &CRubbish_StartStaticsList);
			sheet = next;
		}
	}

	int freq = 0;
	if (CWeather_Wind < 0.1f)
		freq = 31;
	else if (CWeather_Wind < 0.4f)
		freq = 7;
	else if (CWeather_Wind < 0.7f)
		freq = 1;

	if ((CTimer_m_FrameCounter & freq) == 0) {
		int i = GetRandomNumber() % NUM_RUBBISH_SHEETS;
		if (CRubbish_aSheets[i].m_state == 1) {
			CRubbish_aSheets[i].m_moveStart = CTimer_m_snTimeInMilliseconds;
			CRubbish_aSheets[i].m_moveDuration = CWeather_Wind * 1500.0f + 1000.0f;
			CRubbish_aSheets[i].m_animHeight = 0.2f;
			CRubbish_aSheets[i].m_xDist = 3.0f * CWeather_Wind;
			CRubbish_aSheets[i].m_yDist = 3.0f * CWeather_Wind;

			float tx = CRubbish_aSheets[i].m_basePos.x + CRubbish_aSheets[i].m_xDist;
			float ty = CRubbish_aSheets[i].m_basePos.y + CRubbish_aSheets[i].m_yDist;
			float tz = CRubbish_aSheets[i].m_basePos.z + 3.0f;

			CRubbish_aSheets[i].m_targetZ = CWorld_FindGroundZFor3DCoord(tx, ty, tz, &foundGround, NULL) + 0.1f;
            CVector tmp_vec = {tx, ty, CRubbish_aSheets[i].m_targetZ};
			if (CCullZones_FindAttributesForCoors(&tmp_vec) & ATTRZONE_NORAIN)
				CRubbish_aSheets[i].m_targetIsVisible = false;
			else
				CRubbish_aSheets[i].m_targetIsVisible = true;
			if (foundGround) {
				CRubbish_aSheets[i].m_state = 2;
				CRubbish_aSheets[i].m_animationType = 1;
				COneSheet_RemoveFromList(&CRubbish_aSheets[i]);
				COneSheet_AddToList(&CRubbish_aSheets[i], &CRubbish_StartMoversList);
			}
		}
	}

    int i, j;
	for (i = 0; i < NUM_RUBBISH_SHEETS; i++) {
		for (j = 0; j < MAX_NUM_BULLETTRACES; j++) {
			COneSheet* sheet = &CRubbish_aSheets[i];
			CBulletTrace b = CBulletTrace_aTraces[j];

			if (b.m_bExists && CRubbish_aSheets[i].m_state == 1 && DistanceBetweenPoints(&(b.m_vecEnd), &sheet->m_basePos) < 0.8f) {
				sheet->m_state = 0;
				COneSheet_RemoveFromList(sheet);
				COneSheet_AddToList(sheet, &CRubbish_StartEmptyList);
				break;
			}
		}
	}

	for (i = (CTimer_m_FrameCounter % (NUM_RUBBISH_SHEETS / 4)) * 4; i < ((CTimer_m_FrameCounter % (NUM_RUBBISH_SHEETS / 4)) + 1) * 4; i++) {
		if (CRubbish_aSheets[i].m_state == 1 &&
			DistanceBetweenPointsSqr(&CRubbish_aSheets[i].m_basePos, getCameraPosition()) > SQR(RUBBISH_MAX_DIST + 1.0f)) {
			CRubbish_aSheets[i].m_state = 0;
			COneSheet_RemoveFromList(&CRubbish_aSheets[i]);
			COneSheet_AddToList(&CRubbish_aSheets[i], &CRubbish_StartEmptyList);
		}
	}
}

void CRubbish_SetVisibility(bool visible) {
	CRubbish_bRubbishInvisible = (CGame_currArea > 0);
}

void COneSheet_AddToList(COneSheet* this, COneSheet* list) {
	this->m_next = list->m_next;
	this->m_prev = list;
	list->m_next = this;
	this->m_next->m_prev = this;
}

void COneSheet_RemoveFromList(COneSheet* this) {
	this->m_next->m_prev = this->m_prev;
	this->m_prev->m_next = this->m_next;
}

void InjectRubbishPatches() {
    RedirectCall(0x242B9C, CRubbish_Init);

    RedirectCall(0x24368C, CRubbish_Update);

    RedirectCall(0x246EA8, CRubbish_Render);

    RedirectCall(0x247388, CRubbish_Shutdown);

    RedirectCall(0x14ABD0, CRubbish_StirUp);

    RedirectFunction(0x2EB750, CRubbish_SetVisibility);
    
}