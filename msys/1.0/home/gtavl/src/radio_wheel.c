#include <CRadar.h>
#include <CPed.h>
#include <CPool.h>
#include <common.h>
#include <CVehicle.h>
#include <CTxdStore.h>
#include <CPad.h>
#include <CRGBA.h>
#include <math.h>
#include "hooks.h"

int (*CAudioEngine_GetCutsceneTrackStatus)() = (int (*)())0x581300;
uint32_t *(*CAEVehicleAudioEntity_StaticGetPlayerVehicleAudioSettingsForRadio)() = (uint32_t *(*)())0x56F160;
void (*CAERadioTrackManager_StopRadio)(int, uint32_t, char) = (void (*)(int, uint32_t, char))0x5653B0;
void (*CAudioEngine_ReportFrontendAudioEvent)(uint32_t *, int, float, float) = (void (*)(uint32_t *, int, float, float))0x580D70;
void (*CAERadioTrackManager_StartRadio)(int, int8_t, int8_t, int) = (void (*)(int, int8_t, int8_t, int))0x564FC0;
int (*CPad_ChangeStationUpJustUp)(CPad *) = (int (*)(CPad *))0x24A3A0;
int (*CPad_ChangeStationDownJustUp)(CPad *) = (int (*)(CPad *))0x24A3E0;

void RadioHud_process(int this)
{
    uint32_t *v8;
    uint8_t radioType;
    CPad *v9;
    CPad *v10;
    char v12;
    int v13;
    char stationID;

    uint32_t CTimer_m_snTimeInMilliseconds = *(uint32_t*)0x0066BA14;
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
                    if ( *(int *)(this + 120) < 0 )
                    {
                        v9 = CPad_GetPad(0);
                        if (v9->NewState.DPadLeft) {
                            if ( v9->NewState.LeftStickX < -30 && v9->OldState.LeftStickX != v9->NewState.LeftStickX)
                            {
                                *(uint32_t *)(this + 108) = (*(uint32_t *)(this + 104))++;
                                *(uint32_t *)(this + 76) = CTimer_m_snTimeInMilliseconds;
                                *(uint8_t *)(this + 1) = 1;
                                *(uint8_t *)(this + 6) = 1;
                            }
                            else
                            {
                                v10 = CPad_GetPad(0);
                                if (  v10->NewState.LeftStickX > 30 && v10->OldState.LeftStickX != v10->NewState.LeftStickX )
                                {
                                    *(uint32_t *)(this + 108) = (*(uint32_t *)(this + 104))--;
                                    *(uint32_t *)(this + 76) = CTimer_m_snTimeInMilliseconds;
                                    *(uint8_t *)(this + 1) = 1;
                                    *(uint8_t *)(this + 6) = 1;
                                }
                            }
                        }

                    }
                    else
                    {
                        *(uint32_t *)(this + 108) = *(uint32_t *)(this + 104);
                        *(uint32_t *)(this + 104) = *(uint32_t *)(this + 120) - *(char *)(this + 169);
                        *(uint32_t *)(this + 120) = -1;
                        *(uint32_t *)(this + 76) = CTimer_m_snTimeInMilliseconds;
                        *(uint8_t *)(this + 1) = 1;
                        *(uint8_t *)(this + 6) = 1;
                    }

                }
            }
        }
        if ( !*(uint32_t *)(this + 104) && !*(uint32_t *)(this + 108) )
            return;

        v12 = *(uint32_t *)(this + 104) + *(char *)(this + 169);
        stationID = v12;
        if ( v12 > 0 )
        {
            if ( v12 < 14 )
                goto LABEL_30;
            v12 -= 12;
        }
        else
        {
            v12 += 12;
        }
        stationID = v12;
    LABEL_30:
        if ( v12 == 12 )
        {
            CAERadioTrackManager_StopRadio(this, 0, 0);
            CAudioEngine_ReportFrontendAudioEvent(AudioEngine, 38, 0.0f, 1.0f);
            CAudioEngine_ReportFrontendAudioEvent(AudioEngine, 35, 0.0f, 1.0f);

            CAERadioTrackManager_StartRadio(this, stationID, *(uint8_t *)(this + 226), 0);
            *(uint32_t *)(this + 104) = 0;
            *(uint32_t *)(this + 108) = 0;
            return;
        }

        if ( *(uint8_t *)(this + 225) == 12 )
        {
            CAudioEngine_ReportFrontendAudioEvent(AudioEngine, 37, 0.0f, 1.0f);
            *(uint8_t *)(this + 225) = -1;
        }
        else
        {
            CAERadioTrackManager_StopRadio(this, 0, 0);
        }

        CAudioEngine_ReportFrontendAudioEvent(AudioEngine, 34, 0.0f, 1.0f);

        v13 = 4000;
        if ( flt_6FE654 <= 0.9f )
            v13 = 2000;

        if ( CTimer_m_snTimeInMilliseconds > *(uint32_t *)(this + 76) + 1500
          && CTimer_m_snTimeInMilliseconds > v13 + *(uint32_t *)(this + 92) )
        {
            CAERadioTrackManager_StartRadio(this, stationID, *(uint8_t *)(this + 226), 0);
            *(uint32_t *)(this + 104) = 0;
            *(uint32_t *)(this + 108) = 0;
        }
    }
}
