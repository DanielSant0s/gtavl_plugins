#include <injector.h>
#include <CVehicle.h>
#include <CPad.h>
#include <CRadar.h>
#include <CPool.h>
#include <CRGBA.h>
#include <common.h>
#include <math.h>
#include <bit.h>

#include "hooks.h"

uint16_t* CHud_m_ItemToFlash = (uint16_t*)0x66BBFC;

uint8_t* MaxHealth = (uint8_t*)0x70974B;
uint32_t* CWorld_PlayerInFocus = (uint32_t*)0x7095D0;
uint32_t* CTimer_m_FrameCounter = (uint32_t*)0x66BA38;

float (*CStats_GetFatAndMuscleModifier)(unsigned int) = (float (*)(unsigned int))0x2D8C20;
void (*CSprite2d_DrawBarChart)(float, float, unsigned short, unsigned char, float, signed char, unsigned char, unsigned char, RwRGBA*, RwRGBA*) = (void (*)(float, float, unsigned short, unsigned char, float, signed char, unsigned char, unsigned char, RwRGBA*, RwRGBA*))0x2B0B00;

void CHud_PrintHealthForPlayer(int playerID, float posX, float posY)
{
    float width;
    float fPercentage;
    float fPercentagea;
    RwRGBA v10;
    RwRGBA color;

    if (*CHud_m_ItemToFlash != 4)
    {
        fPercentage = 100.0f * 109.0f;
        width = fPercentage / CStats_GetFatAndMuscleModifier(10);
        fPercentagea = (*(float*)(CWorld_PlayerInFocus[111 * playerID] + 1408));
        CRGBA_CRGBA(&v10, 0, 0, 0, 255);

        if (*(float*)(CWorld_PlayerInFocus[111 * playerID] + 1408) >= 30.0f) {
            CRGBA_CRGBA(&color, 0x3e, 0x9b, 0x01, 255);
        } else {
            if ((*CTimer_m_FrameCounter & 8) != 0) {
                CRGBA_CRGBA(&color, 255, 0, 0, 255);
            } else {
                CRGBA_CRGBA(&color, 128, 0, 0, 255);
            }
            
        }

        CSprite2d_DrawBarChart(40.0f, 420.0f, width, 9.0f, fPercentagea, 0, 0, 1, &color, &v10);
    }
}

static void (*DrawHud)();

static float zoom;

float* CRadar_m_radarRange = (float*)0x66B958;
uint32_t zoomOutTimer = 0;

void updateRadarZoom() {
    WriteWord(0x267E04, (uint16_t)zoom);
    *CRadar_m_radarRange = zoom;
}

void showVehicleAreaZOomOutMinimap() {
    CPad* pad = CPad_GetPad(0);

    if(pad->NewState.DPadDown && !pad->OldState.DPadDown) {
        WriteDword(0x66BC14, 1);
        WriteDword(0x66BC0C, 1500);
        zoom = 350.0f;
        zoomOutTimer = *CTimer_m_snTimeInMilliseconds;
    }

    if ( zoom > 180.0f ) {
        if ( *CTimer_m_snTimeInMilliseconds - zoomOutTimer > 1000 ) {
            zoom -= 10.0f;
            if ( 180.0f > zoom ) {
                zoom = 180.0f;
            }
        }
    }

    DrawHud();
}

int _start()
{
    DrawHud = (void (*)())ReadCall(0x2471E4);
    RedirectCall(0x2471E4, showVehicleAreaZOomOutMinimap);
    MakeBranch(0x267D80, 0x27);
    MakeNop(0x267D84);

    zoom = 180.0f;

    RedirectCall(0x267E20, updateRadarZoom);
    MakeNop(0x267E24);

    MakeRetn(0x4FB520); // Fix heap corruption

    MakeNop(0x5157AC); // Disable motion blur
    MakeNop(0x5157C0); // Disable motion blur

    MakeNop(0x14EB50); // No Wheel Turn Back
    MakeNop(0x14ECF0); // No Wheel Turn Back
    MakeNop(0x14ED84); // No Wheel Turn Back

    MakeNop(0x262804); // No flip explosions - player
    MakeNop(0x146790); // No flip explosions - ped

    WriteByte(0x6FF98F, 1); // Widescreen by default
	WriteByte(0x6FF99A, 0); // Enable Radio EQ
	WriteByte(0x66B8B8, 0); // Enable Invert Look

    WriteWord(0x242DFC, 2000);  // Make a day pass in 48 minutes
    WriteDword(0x66B79C, 2000); // Make a day pass in 48 minutes

    MakeRetn(0x516D80); // Disable heat haze

    MakeBranch(0x4BB188, 0x15B); // Disable enex

    MakeBranch(0x4BC998, 0x3F); // Disable Door Camera

    setupRectangularRadar();

    RedirectCall(0x2AB4B0, CHud_PrintHealthForPlayer);
    RedirectCall(0x2AB4F8, CHud_PrintHealthForPlayer);
    
    setupVehiclePatches();

    loadHUDTexDict();
    RedirectCall(0x2ACD64, DrawRadarCop);
    RedirectCall(0x267F9C, hookedRadarCentre);

    RedirectCall(0x564870, RadioHud_process);

    setupAimPatches();
    setupFOVPatches();
    setupGPSPatches();
    setupPickupPatches();
    setupCameraPatches();
    setupRotatedDrawPatch();
    setupWastedBustedScreen();
    setupDebugStatsMenu();
    setupLoadingScreenPatches();
    setupColourFilterPatches();

    float vehRadarZoomMultiplier = 70.0f;
    float vehRadarMaxZoom = 230.0f;
    WriteDword(0x267DA0, 0x3C030000 + (*(uint32_t*)&vehRadarZoomMultiplier >> 16));
    WriteDword(0x267D98, 0x3C020000 + (*(uint32_t*)&vehRadarMaxZoom >> 16));

    return 0;
}
