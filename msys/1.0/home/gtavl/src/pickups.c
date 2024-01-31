#include <stdint.h>
#include <injector.h>
#include <CVector.h>
#include <common.h>

#include "hooks.h"

static void (*updatePickup)(uint32_t pickup, void *player, void *vehicle, int playerId) = (void (*)(uint32_t pickup, void *player, void *vehicle, int playerId))0x2D9D50;
static void (*ClearSpaceForMissionEntity)(CVector* a1, uint32_t object) = (void (*)(CVector*, uint32_t))0x30AA10;
static void (*CWorld_Remove)(uint32_t) = (void (*)(uint32_t))0x27DF60;
static void (*CEntity_UpdateRwFrame)(uint32_t) = (void (*)(uint32_t))0x22F470;
static void (*CWorld_Add)(uint32_t) = (void (*)(uint32_t))0x27DEA0;
static void (*CSimpleTransform_UpdateRwMatrix)(int a1, int a2) = (void (*)(int, int))0x25FA20;
static void (*CMatrix_UpdateRwMatrix)(uint32_t *a1, uint32_t *a2) = (void (*)(uint32_t *, uint32_t *))0x10ED80;
static void (*CEntity_SetOrientation)(uint32_t, float, float, float) = (void (*)(uint32_t, float, float, float))0x10F200;

static float VectorSqrMagnitude(CVector *vector) { return vector->x * vector->x + vector->y * vector->y + vector->z * vector->z; }

static void setObjectRotation(uint32_t object, float x, float y, float z) {
    int v16, v17, v18, v19;
    float v11, v12, v13, v15, v94, v95, v96;
    float *v10, *v14;

    CWorld_Remove(object);

    v10 = *(float **)(object + 20);
    v11 = (float)(3.1415927f * z) / 180.0f;
    if ( v10 )
    {
        v12 = v10[13];
        v13 = v10[14];
        v94 = v10[12];
        v95 = v12;
        v96 = v13;
        CEntity_SetOrientation(*(uint32_t *)(object + 20), (float)(3.1415927f * x) / 180.0f, (float)(3.1415927f * y) / 180.0f, v11);
        v14 = *(float **)(object + 20);
        v15 = v96;
        v16 = v95;
        v14[12] = v14[12] + v94;
        v14[13] = v14[13] + v16;
        v14[14] = v14[14] + v15;
    } else {
        *(float *)(object + 16) = v11;
    }

    v17 = *(uint32_t *)(object + 24);
    if ( v17 )
    {
        v18 = *(uint32_t **)(object + 20);
        v19 = *(uint32_t *)(v17 + 4) + 16;
        if ( v18 )
            CMatrix_UpdateRwMatrix(v18, v19);
        else
            CSimpleTransform_UpdateRwMatrix(object + 4, v19);
    }

    CEntity_UpdateRwFrame(object);
    CWorld_Add(object);
}

static void pickups_on_ground(uint32_t pickup, void *player, void *vehicle, int playerId) {
    uint32_t pickup_type, x, pObject, wModelId, v17;
    CVector* coords, player_coords, tmp;
    float min_radius = 45.0f;
    float ground_z;
    char b;

    updatePickup(pickup, player, vehicle, playerId);

    pickup_type = *(uint8_t*)(pickup+0x1C);

    if (pickup_type == 2 || pickup_type == 3 || pickup_type == 4 || pickup_type == 5 ||
        pickup_type == 8 || pickup_type == 15 || pickup_type == 19 ) {

        x = *(uint16_t*)(pickup+0x10);
        pObject = *(uint32_t*)(pickup+0x4);
        wModelId = *(uint16_t*)(pickup+0x18);

        if (x != 0 && pObject != 0) {

            v17 = *(uint32_t *)(pObject + 20);
            if ( v17 )
                coords = (CVector *)(v17 + 48);
            else
                coords = (CVector *)(pObject + 4);

            FindPlayerCoors(&player_coords, -1);

            tmp.x = player_coords.x - coords->x;
            tmp.y = player_coords.y - coords->y;
            tmp.z = player_coords.z - coords->z;

            if ((VectorSqrMagnitude(&tmp) <= min_radius * min_radius)) {
                ground_z = CWorld_FindGroundZFor3DCoord(coords->x, coords->y, coords->z, &b, 0);
                ground_z += 0.05f;
                
                coords->z = ground_z;
                (*(void (**)(int, CVector *, uint32_t))(*(uint32_t *)pObject + 64))(pObject, coords, 0);

                ClearSpaceForMissionEntity(coords, pObject);
                
                if (wModelId != 1212) {
                    setObjectRotation(pObject, 90.0f, 0.0f, 134.0f);
                } else {
                    setObjectRotation(pObject, 0.0f, 0.0f, 5.0f);
                }
            }

        }

    }
}

void injectPickupPatches()
{  
    *(uint32_t*)0x2D9E10 = 0; //object->__parent.physical.entity.placeable.m_pCoords->pos.z = outVec.z;
    *(uint32_t*)0x22FD90 = 0x2403FFFF; // movzx edx,word ptr [008CD59C]

    *(uint32_t*)0x2DD3B0 = 0x100000B7;
    *(uint32_t*)0x2DD3B4 = 0;
    *(uint32_t*)0x2DCA10 = 0; //coronas

    RedirectCall(0x2D9674, &pickups_on_ground);
    RedirectCall(0x2D96E4, &pickups_on_ground);
}
