#include <stdint.h>
#include <common.h>
#include <injector.h>
#include <CRGBA.h>
#include <CTexture.h>
#include <CFont.h>
#include "hooks.h"

float* dword_7C3298 = (float*)0x7C3298;

unsigned int* CHud_m_VehicleState = (unsigned int*)0x66BC58;
unsigned int* CHud_m_ZoneState = (unsigned int*)0x66BC14;
const char* _StyledText_3 = (const char*)0x7C2990;

static float death_timestep = 0.0f;

static int grain_backup = 0;

static int death_state = 0;

uint8_t* CPostEffects_m_bGrainEnable  = (uint8_t*)0x66C70C;
uint32_t* grainLevel = (uint32_t*)0x6684E4;

float* timeScale = (float*)0x66BA28;

uint8_t* displayHUD = (uint8_t*)0x6FF965;
static uint8_t hud_backup = 0;

uint32_t* radarMode = (uint32_t*)0x6FF968;
static uint32_t backup_radar = 0;

void CHud_DrawBustedWastedMessage()
{
    RwRGBA text_color;
    RwRGBA shadow_color;
    RwRGBA quad_color;

    if ( *_StyledText_3 )
    {
        if ( 0.0f == *dword_7C3298 )
        {
            death_timestep = 0.0f;
            *dword_7C3298 = 1.0f;
            if ( *CHud_m_VehicleState )
                *CHud_m_VehicleState = 0;
            if ( *CHud_m_ZoneState )
                *CHud_m_ZoneState = 0;
            if (death_state == 0) {
                death_state = 1;
                *CPostEffects_m_bGrainEnable = 1;
                grain_backup = *grainLevel;
                *grainLevel = 64;
                *timeScale = 0.1f;
                hud_backup = *displayHUD;
                *displayHUD = 0;
                backup_radar = *radarMode;
                *radarMode = 2;
            }
        } else {
            death_timestep += 0.4f * (*CTimer_ms_fTimeStep * 0.02f * 1000.0f);

            if (death_timestep > 1000.f && death_state == 1) {
                asm volatile ("nop\n");
            }

            CFont_SetBackground(0, 0);
            CFont_SetScale(1.25f, 3.0f);
            CFont_SetProportional(1);
            CFont_SetJustify(0);
            CFont_SetOrientation(0);
            CFont_SetFontStyle(3);
            CFont_SetEdge(0);

            CRGBA_CRGBA(&shadow_color, 0, 0, 0, 0);
            CFont_SetDropColor(&shadow_color);

            CRGBA_CRGBA(&text_color, 220, 0, 0, 180);
            CFont_SetColor(&text_color);

            CRGBA_CRGBA(&quad_color, 0, 0, 0, 150);
            CSprite2d_Draw2DPolygon(0.0f, 175.0f, 0.0f, 259.0f, 640.0f, 160.0f, 640.0f, 278.0f, &quad_color);

            CFont_PrintString(320.0f, 190.0f, _StyledText_3);
        }
    } else {
        if (death_state > 0) {
            death_state = 0;
            *CPostEffects_m_bGrainEnable = 0;
            *grainLevel = grain_backup;
            *timeScale = 1.0f;
            *displayHUD = hud_backup;
            *radarMode = backup_radar;
            
        }

        *dword_7C3298 = 0.0f;
    }
}

void setupWastedBustedScreen()
{
    RedirectCall(0x2B0218, CHud_DrawBustedWastedMessage);
    MakeRetn(0x2108B0); // Disable Wasted camera
}
