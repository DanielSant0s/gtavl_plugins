#include <injector.h>
#include <CVehicle.h>
#include <CPad.h>
#include <CRadar.h>
#include <CPool.h>
#include <CRGBA.h>
#include <string.h>
#include <common.h>
#include <stdio.h>
#include <math.h>
#include <bit.h>

#include "hooks.h"

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
        zoom = 320.0f;
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

    zoom = 160.0f;

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
