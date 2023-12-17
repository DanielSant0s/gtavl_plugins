#include <CRadar.h>
#include <CPed.h>
#include <CPool.h>
#include <common.h>
#include <CVehicle.h>
#include <CTxdStore.h>
#include <injector.h>
#include <CRGBA.h>
#include <math.h>
#include "hooks.h"

void (*drawBlips)(void*) = (void (*)(void*))0x268080;

char (*CWanted_SetPursuitCop)(void *, void *) = (char (*)(void *, void *))0x27D7E0;

int (*GetPedHandle)(CPool*, int) = (int (*)(CPool*, int))0x0019FA20;

float (*CGeneral_LimitRadianAngle)(float) = (float (*)(float))0x245130;

static float angle = 0.0f;

#define get_entity_model(entity) *(short *)((char*)entity + 34)

static CTexture hud_textures[10];

void load_vhud_texture(const char* name, int slot) {
    int v7 = CTxdStore_AddTxdSlot("v_hud");
    CTxdStore_LoadTxd(v7, "MODELS\\V_HUD.TXD");
    CTxdStore_AddRef(v7);
    CTxdStore_PushCurrentTxd();
    CTxdStore_SetCurrentTxd(v7);

    CSprite2D_SetTexture(&hud_textures[slot], name);

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(v7);
    CTxdStore_RemoveTxdSlot(v7);
}

void DrawRadarCop(void* radar) {
    CVector screen_coords, radar_coords, *ent_coords;
    RwRGBA blip_colour;

    CPool* pedPool = *(CPool**)0x0066B918;
    CPool* vehPool = *(CPool**)0x0066B91C;
    
    uint32_t paused_timer = *(uint32_t*)0x0066BA14;

    int i = 0;

    if (FindPlayerWanted(0)->m_nWantedLevel > 0) {

        for (i = pedPool->size; i; i--) {
            uint32_t* ped = (uint8_t*)pedPool->objects + (i - 1)*sizeof_CPed;

            if (!ped || IsPlayer(ped) || *(float *)((char*)ped + 1408) <= 0.0f)
                continue; 

            // Is ped
            if (   get_entity_model(ped) == MODEL_LAPD1
                || get_entity_model(ped) == MODEL_SFPD1
                || get_entity_model(ped) == MODEL_LVPD1
                || get_entity_model(ped) == MODEL_CSHER
                || get_entity_model(ped) == MODEL_LAPDM1
                || get_entity_model(ped) == MODEL_SWAT
                || get_entity_model(ped) == MODEL_FBI
                || get_entity_model(ped) == MODEL_ARMY
                || get_entity_model(ped) == MODEL_DSHER) {             

                ent_coords = getCharCoordinates(ped);

                if (ent_coords->x != 0.0f && ent_coords->y != 0.0f ) {
                    if (!hud_textures[0].texture)
                        load_vhud_texture("radar_cop", 0);

                    CRadar_TransformRealWorldPointToRadarSpace(&radar_coords, ent_coords);

                    CCustomRadar_LimitRadarPoint(&radar_coords);
                    CRadar_TransformRadarPointToScreenSpace(&screen_coords, &radar_coords);

                    CRadar_DrawRotatingRadarSprite(&hud_textures[0], screen_coords.x, screen_coords.y, 0.0f, 5, 6, paused_timer % 800 < 400 ? CRGBA_CRGBA(&blip_colour, 112, 25, 25, 255) : CRGBA_CRGBA(&blip_colour, 47, 92, 115, 255));
                }

            }
        }

        for (i = vehPool->size; i; i--) {
            uint32_t* veh = (uint8_t*)vehPool->objects + (i - 1)*sizeof_CVehicle;

            if (!veh || (veh && getVehicleHealth(veh) <= 0.0f) || veh == FindPlayerVehicle(-1, 0))
                continue;

            // Is heli
            if (get_entity_model(veh) == 497/*MODEL_POLMAV*/) {
                angle += 0.02f * (3.14159265359f * 1.5f);
                angle = CGeneral_LimitRadianAngle(angle);

                ent_coords = getVehCoordinates(veh);

                if (ent_coords->x != 0.0f && ent_coords->y != 0.0f ) {

                    if (!hud_textures[1].texture)
                        load_vhud_texture("radar_cop_heli", 1);

                    CRadar_TransformRealWorldPointToRadarSpace(&radar_coords, ent_coords);

                    CCustomRadar_LimitRadarPoint(&radar_coords);
                    CRadar_TransformRadarPointToScreenSpace(&screen_coords, &radar_coords);

                    CRadar_DrawRotatingRadarSprite(&hud_textures[1], screen_coords.x, screen_coords.y, angle, 5, 6, paused_timer % 800 < 400 ? CRGBA_CRGBA(&blip_colour, 112, 25, 25, 255) : CRGBA_CRGBA(&blip_colour, 47, 92, 115, 255));
                }
            }
        }

    } else {
        if (hud_textures[0].texture) {
            CSprite2d_Delete(&hud_textures[0]);
            hud_textures[0].texture = NULL;
        }

        if (hud_textures[1].texture) {
            CSprite2d_Delete(&hud_textures[1]);
            hud_textures[1].texture = NULL;
        }
            
    }


    drawBlips(radar);
}

void injectRadarPatches() {
    hud_textures[0].texture = NULL;
    hud_textures[1].texture = NULL;

    RedirectCall(0x2ACD64, DrawRadarCop);
}
