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
//#include "utility.h"

#include "virtual_mem.h"

#include "bios_alloc.h"

enum RwTextureAddressMode
{
    rwTEXTUREADDRESSNATEXTUREADDRESS = 0,   /**<Invalid addressing mode */
    rwTEXTUREADDRESSWRAP,                   /**<UV wraps (tiles) */
    rwTEXTUREADDRESSMIRROR,                 /**<Alternate UV is flipped */
    rwTEXTUREADDRESSCLAMP,                  /**<UV is clamped to 0-1 */
    rwTEXTUREADDRESSBORDER,                 /**<Border colour takes effect outside of 0-1 */
    rwTEXTUREADDRESSMODEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwTextureAddressMode RwTextureAddressMode;

void CRadar_DrawBlips(void*);
char CWanted_SetPursuitCop(void *, void *);

int GetPedHandle(CPool*, int);

float CGeneral_LimitRadianAngle(float);

static float angle = 0.0f;

#define get_entity_model(entity) *(short *)((char*)entity + 34)

static CTexture hud_textures[10];

RwTexture dummy_tex = { 0 };
RwRaster dummy_raster = { 0 };

RwRaster *base_raster;

VirtualMemoryBlock hud_textures_block;

typedef struct {
    size_t texture;
    size_t raster;
    size_t pixels;
    size_t palette;
} TextureOffsets;

TextureOffsets radar_cop_offsets;

static void (*CGame_Init1)(const char*);

static RwTexture* (*RwTextureRead)(const char* name, const char* mask) = (RwTexture* (*)(const char* name, const char* mask))0x345A80;

void preload_hud_textures(const char* a1) {
    CGame_Init1(a1);

    RwTexture* tmp_tex = NULL;

    dummy_tex.refCount = 1;
    dummy_tex.raster = NULL;

    register_block(&hud_textures_block, 2*(sizeof(RwTexture) + sizeof(RwRaster) + 16*sizeof(RwRGBA) + (32*32/2)) + 1024/*security margin*/, false);

    CTxdStore_PushCurrentTxd();
    
    int v7 = CTxdStore_FindTxdSlot("vlhud");

    if ( v7 == -1 )
        v7 = CTxdStore_AddTxdSlot("vlhud");

    CTxdStore_LoadTxd(v7, "MODELS\\VLHUD.TXD");
    CTxdStore_AddRef(v7);

    CTxdStore_SetCurrentTxd(v7);

    //CSprite2d_SetTexture(&hud_textures[slot], name, 0);

    tmp_tex = RwTextureRead("radar_cop", NULL);

    dummy_tex.filterAddressing = tmp_tex->filterAddressing;

    size_t current_offset = 0;

    write_block(&hud_textures_block, &dummy_tex, current_offset, sizeof(RwTexture));
    radar_cop_offsets.texture = current_offset;
    current_offset += sizeof(RwTexture);

    dummy_raster.parent = NULL; /* Top level raster if a sub raster */
    dummy_raster.cpPixels = NULL; /* Pixel pointer when locked */
    dummy_raster.palette = NULL; /* Raster palette */
    dummy_raster.width = tmp_tex->raster->width; /* Dimensions of raster */
    dummy_raster.height = tmp_tex->raster->height; /* Dimensions of raster */
    dummy_raster.depth = tmp_tex->raster->depth; /* Dimensions of raster */
    dummy_raster.stride = tmp_tex->raster->stride; /* Lines bytes of raster */
    dummy_raster.cType = tmp_tex->raster->cType;  /* Type of raster */
    dummy_raster.cFlags = tmp_tex->raster->cFlags; /* Raster flags */
    dummy_raster.privateFlags = tmp_tex->raster->privateFlags; /* Raster private flags */
    dummy_raster.cFormat = tmp_tex->raster->cFormat; /* Raster format */

    dummy_raster.originalPixels = NULL;
    dummy_raster.originalWidth = tmp_tex->raster->originalWidth;
    dummy_raster.originalHeight = tmp_tex->raster->originalHeight;
    dummy_raster.originalStride = tmp_tex->raster->originalStride;

    write_block(&hud_textures_block, &dummy_raster, current_offset, sizeof(RwRaster));
    radar_cop_offsets.raster = current_offset;
    current_offset += sizeof(RwRaster);

    write_block(&hud_textures_block, tmp_tex->raster->cpPixels, current_offset, (32*32/2));
    radar_cop_offsets.pixels = current_offset;
    current_offset += (32*32/2);

    write_block(&hud_textures_block, tmp_tex->raster->palette, current_offset, 16*sizeof(RwRGBA));
    radar_cop_offsets.palette = current_offset;
    current_offset += 16*sizeof(RwRGBA);

    RwTextureDestroy(tmp_tex);

    CTxdStore_PopCurrentTxd();

    dump_block(&hud_textures_block);

    //free_virtual_memory();
}

static void load_vhud_texture(const char* name, int slot) {
    //CStreaming_MakeSpaceFor(306464);
    //CStreaming_ImGonnaUseStreamingMemory();
    //CGame_TidyUpMemory(false, true);

    CTxdStore_PushCurrentTxd();
    
    int v7 = CTxdStore_FindTxdSlot("vlhud");
    if ( v7 == -1 )
        v7 = CTxdStore_AddTxdSlot("vlhud");
    CTxdStore_LoadTxd(v7, "MODELS\\VLHUD.TXD");
    CTxdStore_AddRef(v7);

    CTxdStore_SetCurrentTxd(v7);

    CSprite2d_SetTexture(&hud_textures[slot], name, 0);

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(v7);
    //CTxdStore_RemoveTxdSlot(v7);

    //CStreaming_IHaveUsedStreamingMemory();

    memset(&dummy_raster, 0, sizeof(RwRaster));
}

static bool bRedOrBlue = false;
static bool bActivateRadarColorChange = false;
static uint32_t bRadarColorTimer = 0;

extern uint32_t AudioEngine;
void (*CAudioEngine_PreloadCutsceneTrack)(uint32_t*, short, char) = (void (*)(uint32_t*, short , char))0x581010;
void (*CAudioEngine_PlayPreloadedCutsceneTrack)(uint32_t*) = (void (*)(uint32_t*))0x5811A0;
void (*CAudioEngine_StopCutsceneTrack)(uint32_t*) = (void (*)(uint32_t*))0x5814E0;

static uint32_t* CAEAudioHardware = (uint32_t*)0x876990;
uint32_t AECutsceneTrackManager = 0x880260;

void (*CAEAudioHardware_SetChannelVolume)(uint32_t*, short channel, uint16_t channelId, float volume, uint8_t a4) = (void (*)(uint32_t*, short channel, uint16_t channelId, float volume, uint8_t a4))0x5582B0;
void (*CAEAudioHardware_SetChannelFlags)(uint32_t*, short channel, uint16_t channelId, int, int, int, short flags) = (void (*)(uint32_t*, short channel, uint16_t channelId, int, int, int, short flags))0x558140;
bool playingOST = false;

void DrawRadarCop(void* radar) {
    CVector screen_coords, radar_coords, *ent_coords;
    RwRGBA blip_colour;

    CPool* pedPool = *(CPool**)0x0066B918;
    CPool* vehPool = *(CPool**)0x0066B91C;
    
    uint32_t paused_timer = *(uint32_t*)0x0066BA14;

    int i = 0;

    if (FindPlayerWanted(0)->m_nWantedLevel > 2) {
        if (!playingOST) {
            playingOST = true;
            CAudioEngine_PreloadCutsceneTrack(&AudioEngine, 0x272+randint(0, 12), 1);
            CAudioEngine_PlayPreloadedCutsceneTrack(&AudioEngine);
        }
    } else {
        if (playingOST) {
            playingOST = false;
            CAudioEngine_StopCutsceneTrack(&AudioEngine);
        }
    }

    if (FindPlayerWanted(0)->m_nWantedLevel > 0) {

        if (!bActivateRadarColorChange) {
            bActivateRadarColorChange = true;

        }

        if (bRadarColorTimer < CTimer_m_snTimeInMilliseconds) {
            if (bRedOrBlue) {
                WriteByte(0x26E29C, 0xFF);
                WriteByte(0x26E2A4, 0xA5);
                WriteByte(0x26E2A8, 0xA5);
                WriteByte(0x26E2B0, 0xFF);
            } else {
                WriteByte(0x26E29C, 0xA5);
                WriteByte(0x26E2A4, 0xA5);
                WriteByte(0x26E2A8, 0xFF);
                WriteByte(0x26E2B0, 0xFF);
            }
            bRedOrBlue ^= 1;
            bRadarColorTimer = CTimer_m_snTimeInMilliseconds + 1000;
        }

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
                    if (!hud_textures[0].texture) {
                        //load_vhud_texture("radar_cop", 0);
                        hud_textures[0].texture = &dummy_tex;

                        base_raster = RwRasterCreate(32, 32, 4, rwRASTERTYPETEXTURE | rwRASTERFORMAT8888 | rwRASTERFORMATPAL4);

                        read_block(&hud_textures_block, &dummy_tex, radar_cop_offsets.texture, sizeof(RwTexture));
                        //read_block(&hud_textures_block, &dummy_raster, radar_cop_offsets.raster, sizeof(RwRaster));
 //
                        //dummy_raster.originalPixels = br_alloc((32*32)/2);
                        //dummy_raster.cpPixels = dummy_raster.originalPixels;
                        //dummy_raster.palette = br_alloc(16*sizeof(RwRGBA));

                        dummy_tex.raster = base_raster;
                        RwRasterLock(base_raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH);
                        RwRasterLockPalette(base_raster, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH);
                        read_block(&hud_textures_block, base_raster->cpPixels, radar_cop_offsets.pixels, (32*32/2));
                        read_block(&hud_textures_block, base_raster->palette, radar_cop_offsets.palette, 16*sizeof(RwRGBA));

                        //memcpy(base_raster, &dummy_raster, sizeof(RwRaster));
                    }

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
        if (bActivateRadarColorChange) {
            bActivateRadarColorChange = false;
            WriteByte(0x26E29C, 0xFF);
            WriteByte(0x26E2A4, 0xFF);
            WriteByte(0x26E2A8, 0xFF);
            WriteByte(0x26E2B0, 0xFF);
        }

        if (hud_textures[0].texture) {
            //CSprite2d_Delete(&hud_textures[0]);
            RwRasterDestroy(base_raster);
            hud_textures[0].texture = NULL;
        }

        if (hud_textures[1].texture) {
            CSprite2d_Delete(&hud_textures[1]);
            hud_textures[1].texture = NULL;
        }
            
    }


    CRadar_DrawBlips(radar);
}

int sub_10001600(float *a1, float *a2, float *a3, float *a4)
{
    int result;
    float v5;
    float v6;
    float v7;
    float v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    int v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;

    if ( 0.0f == a2[1] )
    {
        a1[1] = 0.0f;
        if ( 0.0f == *a2 )
        {
            *a1 = 0.0f;
            result = 0;
            *a3 = 0.0f;
            *a4 = 0.0f;
            return result;
        }
        if ( *a2 <= 0.0f )
        {
            v7 = -1.0f;
            *a1 = -1.0f;
            v6 = v7 - *a2;
        }
        else
        {
            v5 = 1.0f;
            *a1 = 1.0f;
            v6 = *a2 - v5;
        }
        v27 = v6;
        v35 = fabsf(v27);
        *a3 = v35;
        v36 = fabsf(*a2);
        *a4 = v36;
        return v27 >= 0.0f;
    }
    if ( a2[1] <= 0.0f )
    {
        v26 = -1.0f;
        v9 = -1.0f;
        v8 = -1.0f - a2[1];
    }
    else
    {
        v26 = 1.0f;
        v8 = a2[1] - 1.0f;
        v9 = 1.0f;
    }
    v30 = v8;
    if ( 0.0f == *a2 )
    {
        *a1 = 0.0f;
        a1[1] = v9;
        v37 = fabsf(v30);
        *a3 = v37;
        v38 = fabsf(a2[1]);
        *a4 = v38;
        if ( v30 < 0.0f )
            return 0;
        result = 1;
    }
    else
    {
        v10 = v30;
        v43 = 1.0f * v30;
        v11 = v43;
        v44 = 1.0f + v43;
        v39 = fabsf(*a2);
        if ( v44 <= (float)v39 )
        {
            if ( *a2 <= 0.0f )
            {
                v25 = -1.0f;
                v18 = -1.0f;
                v17 = -1.0f - *a2;
            }
            else
            {
                v25 = 1.0f;
                v17 = *a2 - 1.0f;
                v18 = 1.0f;
            }
            v29 = v17;
            v45 = 1.0f * v29;
            v19 = v45;
            v46 = 1.0f + v45;
            v32 = fabsf(a2[1]);
            if ( v46 <= (float)v32 )
            {
                *a1 = v25;
                a1[1] = v26;
                v21 = v30;
                v20 = v29;
            }
            else
            {
                v20 = v29;
                v31 = a2[1] / v46 * v19;
                *a1 = v18;
                v21 = v31;
                a1[1] = a2[1] - v31;
            }
            v22 = v21;
            if ( v20 < 0.0f )
            {
                v23 = -1.0f;
                v15 = 0;
            }
            else
            {
                v23 = v39;
                v15 = 1;
            }
            v42 = v23;
            v41 = v20 / v42;
            v24 = v22;
            v13 = v20;
            v14 = v24;
        }
        else
        {
            v28 = *a2 / v44 * v11;
            *a1 = *a2 - v28;
            v12 = v9;
            v13 = v28;
            a1[1] = v12;
            v14 = v30;
            if ( v10 < 0.0f )
            {
                v15 = 0;
                v16 = v10 / -1.0f;
            }
            else
            {
                v15 = 1;
                v40 = fabsf(a2[1]);
                v16 = v10 / v40;
            }
            v41 = v16;
        }
        v33 = v14 * v14 + v13 * v13;
        v34 = sqrtf(v33);
        result = v15;
        *a3 = v34;
        *a4 = v34 * v41;
    }
    return result;
}

float rectLimitRadarPoint(float *a1)
{
    float result; // st7
    float v2; // st7
    int v3; // ecx
    float v4; // [esp+10h] [ebp-10h] BYREF
    float v5; // [esp+14h] [ebp-Ch] BYREF
    int v6[2]; // [esp+18h] [ebp-8h] BYREF

    if ( *(unsigned char*)(&FrontEndMenuManager + 101) )
    {
        v5 = a1[1] * a1[1] + *a1 * *a1;
        v5 = sqrtf(v5);
        result = v5;
    } else {
        if ( sub_10001600((float *)v6, a1, &v4, &v5) )
        {
            v2 = v4 + 1.0f;
            v3 = v6[1];
            *a1 = *(float *)v6;
            *((uint32_t *)a1 + 1) = v3;
            v4 = v2;
        }
        else
        {
            v4 = (v5 - v4) / v5;
        }
        result = v4;
    }
    return result;
}

static void hookedRadarCentre() {
    float radarShift = 25.0f;
    radarShift *= (*(float*)0x66B958 / 50.0f);
    float orientation = *(float*)0x66B95C;

    float a = orientation - 1.570796f;

    RadarOrigin.x -= cosf(a) * radarShift;
    RadarOrigin.y -= sinf(a) * radarShift;

    drawRadarMap();
}


void injectRadarPatches() {
    hud_textures[0].texture = NULL;
    hud_textures[1].texture = NULL;

    
    CGame_Init1 = (void (*)())ReadCall(0x242B9C);

    RedirectCall(0x242B9C, preload_hud_textures);

    WriteWord(0x559B80, 0xc140);
    WriteWord(0x559BE4, 0xc140);
    WriteByte(0x559BDC, 55); // Change cutscene audio flags to accept volume changes
    WriteByte(0x559C3C, 55); // Change cutscene audio flags to accept volume changes

    RedirectCall(0x238974, &rectLimitRadarPoint);
    RedirectCall(0x23BE14, &rectLimitRadarPoint);
    RedirectCall(0x23BECC, &rectLimitRadarPoint);
    RedirectCall(0x23BF18, &rectLimitRadarPoint);
    RedirectCall(0x23BF80, &rectLimitRadarPoint);
    RedirectCall(0x23BFE4, &rectLimitRadarPoint);
    RedirectCall(0x23C048, &rectLimitRadarPoint);
    RedirectCall(0x23C134, &rectLimitRadarPoint);
    RedirectCall(0x23C400, &rectLimitRadarPoint);
    RedirectCall(0x23C44C, &rectLimitRadarPoint);
    RedirectCall(0x23C5B0, &rectLimitRadarPoint);
    RedirectCall(0x23C5FC, &rectLimitRadarPoint);
    RedirectCall(0x23C76C, &rectLimitRadarPoint);
    RedirectCall(0x23C7B4, &rectLimitRadarPoint);
    RedirectCall(0x23C914, &rectLimitRadarPoint);
    RedirectCall(0x23C95C, &rectLimitRadarPoint);
    RedirectCall(0x268164, &rectLimitRadarPoint);
    RedirectCall(0x2681C4, &rectLimitRadarPoint);
    RedirectCall(0x268488, &rectLimitRadarPoint);
    RedirectCall(0x268534, &rectLimitRadarPoint);
    RedirectCall(0x268818, &rectLimitRadarPoint);
    RedirectCall(0x268FB8, &rectLimitRadarPoint);
    RedirectCall(0x269118, &rectLimitRadarPoint);
    RedirectCall(0x2696D0, &rectLimitRadarPoint);
    RedirectCall(0x26A5D0, &rectLimitRadarPoint);

    *(uint32_t*)(0x26F0AC) = 0; // Disable radar mask

    RedirectCall(0x267F9C, &hookedRadarCentre);

    RedirectCall(0x2ACD64, &DrawRadarCop);
}
