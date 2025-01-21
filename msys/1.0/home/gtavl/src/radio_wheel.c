#include <CRadar.h>
#include <CPed.h>
#include <CPool.h>
#include <stdio.h>
#include <common.h>
#include <CTxdStore.h>
#include <CTexture.h>
#include <CPad.h>
#include <CRGBA.h>
#include <math.h>
#include <injector.h>
#include <string.h>
#include "hooks.h"
#include "colour_filter.h"

int CAudioEngine_GetCutsceneTrackStatus();
uint32_t *CAEVehicleAudioEntity_StaticGetPlayerVehicleAudioSettingsForRadio();
void CAERadioTrackManager_StopRadio(int, uint32_t, char);
void CAudioEngine_ReportFrontendAudioEvent(uint32_t *, int, float, float);
void CAERadioTrackManager_StartRadio(int, int8_t, int8_t, int);
void CAERadioTrackManager_DisplayRadioStationName(void* this);

bool allow_radiowheel_drawing = true;

bool radiowheel_visible = false;
static bool norm_timescale = true;

static CTexture radio_icon[12];
static CTexture radio_selector;

static const char* radio_names[12] = {
    "playback",
    "krose",
    "KDST",
    "bounce",
    "SFUR",
    "RLS",
    "RADIOX",
    "csr",
    "kjah",
    "mastersounds",
    "WCTR",
    "off"
};

static void RadioHud_loadTextures() {
    char radio_name[32] = "radio_";
    //CStreaming_MakeSpaceFor(66328);
    //CStreaming_ImGonnaUseStreamingMemory();
    //CGame_TidyUpMemory(false, true);
    CTxdStore_PushCurrentTxd();
    int v7 = CTxdStore_FindTxdSlot("fronten1");
    if ( v7 == -1 )
        v7 = CTxdStore_AddTxdSlot("fronten1");
    CTxdStore_LoadTxd(v7, "MODELS\\FRONTEN1.TXD");
    CTxdStore_AddRef(v7);
    CTxdStore_SetCurrentTxd(v7);

    int i;
    for (i = 0; i < 12; i++) {
        strcat(radio_name, radio_names[i]);
        CSprite2d_SetTexture(&radio_icon[i], radio_name, 0);
        radio_name[6] = '\0';
    }

    CSprite2d_SetTexture(&radio_selector, "radio_selector", 0);

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(v7);
    //CTxdStore_RemoveTxdSlot(v7);
    //CStreaming_IHaveUsedStreamingMemory();
}

static void RadioHud_unloadTextures() {
    int i;
    for (i = 0; i < 12; i++) {
        CSprite2d_Delete(&radio_icon[i]);
    }

    CSprite2d_Delete(&radio_selector);
}

RwRGBA radio_wheel_bgcolor;
RwRGBA radio_wheel_texcolor;

//int (*CAudioEngine_GetCurrentRadioStationID)() = (int (*)())0x580FF0;

static int pointID;
static int oldPointID;

static inline int calculatePointID(float x, float y, int maxID) {
    float angle = atan2f(y, x);

    if (angle < 0)
        angle += 2 * 3.14159265359f;

    int pointID = (int)((angle / (2 * 3.14159265359f)) * maxID) + 1;

    return pointID;
}

static bool change_station = false;

extern float RightStickSensValue;
static float SensibilityBackup = 0.0f;

static void RadioHud_process(int this)
{
    uint32_t *v8;
    uint8_t radioType;
    CPad *v9;
    char v12;
    int v13;
    char stationID;

    float flt_6FE654 = *(float*)0x6FE654;
    uint32_t* AudioEngine = (uint32_t*)0x889C80;


    if ( *(uint8_t *)(this + 225) )
    {
        *(uint8_t *)(this + 6) = 0;
        if ( (*(uint32_t *)(this + 100) < 3 || *(uint8_t *)this || *(uint32_t *)(this + 104) || *(uint32_t *)(this + 108) || *(uint8_t *)(this + 225) == 12)
        && !CAudioEngine_GetCutsceneTrackStatus(AudioEngine))
        {
            v8 = CAEVehicleAudioEntity_StaticGetPlayerVehicleAudioSettingsForRadio();
            if ( v8 )
            {
                radioType = *(uint8_t *)((uint32_t)v8 + 23);
                if (!radioType || radioType == 3 || radioType == 2) {
                    if ( *(uint8_t *)((uint32_t)CAEVehicleAudioEntity_StaticGetPlayerVehicleAudioSettingsForRadio() + 23) )
                        return;

                    v9 = CPad_GetPad(0);
                    if (v9->NewState.DPadLeft && allow_radiowheel_drawing) {
                        if (!radiowheel_visible ) {
                            radiowheel_visible = true;
                            SensibilityBackup = RightStickSensValue;
                            RightStickSensValue = 0.0f;
                            pointID = *(uint8_t *)(this + 225);
                        }

                        oldPointID = pointID;
                        if (v9->NewState.RightStickX || v9->NewState.RightStickY) {
                            pointID = calculatePointID(((float)v9->NewState.RightStickX / 128.0f), ((float)v9->NewState.RightStickY / 128.0f), 12);
                        }

                        if (pointID != oldPointID) {
                            CAERadioTrackManager_StopRadio(this, 0, 0);
                            CAudioEngine_ReportFrontendAudioEvent(AudioEngine, 38, 0.0f, 1.0f);
                            CAudioEngine_ReportFrontendAudioEvent(AudioEngine, 35, 0.0f, 1.0f);

                            CAERadioTrackManager_StartRadio(this, pointID, *(uint8_t *)(this + 226), 0);
                            *(uint32_t *)(this + 104) = 0;
                            *(uint32_t *)(this + 108) = 0;
                        } 

                    } else {
                        if (radiowheel_visible) {
                            radiowheel_visible = false;
                            RightStickSensValue = SensibilityBackup;
                        }
                    }

                }
            }
        }

    }


}

static void CSprite2d_DrawSpriteFixed(CTexture* texture, float x, float y, float w, float h, RwRGBA* color)
{
    w *= 0.85f;
    //x += 48.0f;

    CSprite2d_DrawSprite(texture, x, y, w, h, color);
}


static void RadioHud_render(uint32_t this) {
    CAERadioTrackManager_DisplayRadioStationName(this);
    if (radiowheel_visible) {
        if (norm_timescale) {
            CTimer_ms_fTimeScale = 0.05f;
            norm_timescale = false;
            enable_blur = true;
            RadioHud_loadTextures();
        }

        int i;
        for (i = 0; i < 12; i++) {
            float angle = (2 * 3.14159265359f / 12) * i;
            float textureX = 0.85f * 120.0f * cosf(angle);
            float textureY = 120.0f * sinf(angle);

            if (i == pointID-1) CSprite2d_DrawSpriteFixed(&radio_selector, (320.0f-26.0f) + textureX, (224.0f-26.0f) + textureY, 52.0f, 52.0f, &radio_wheel_bgcolor);
            CSprite2d_DrawSpriteFixed(&radio_icon[i], (320.0f-24.0f) + textureX, (224.0f-24.0f) + textureY, 48.0f, 48.0f, &radio_wheel_texcolor);
        }

    } else if (!norm_timescale && allow_radiowheel_drawing) {
        CTimer_ms_fTimeScale = 1.0f;
        norm_timescale = true;
        enable_blur = false;
        RadioHud_unloadTextures();
    }
}

void injectRadioWheelPatches() {
    CRGBA_CRGBA(&radio_wheel_bgcolor, 255, 255, 255, 255);
    CRGBA_CRGBA(&radio_wheel_texcolor, 255, 255, 255, 255);
    RedirectCall(0x564870, &RadioHud_process);
    RedirectJump(0x580FE4, &RadioHud_render);
}
