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

static void (*DrawHud)();

static float zoom;

uint32_t zoomOutTimer = 0;

void updateRadarZoom() {
    WriteWord(0x267E04, (uint16_t)zoom);
    CRadar_m_radarRange = zoom;
}

void showVehicleAreaZOomOutMinimap() {
    CPad* pad = CPad_GetPad(0);

    if(pad->NewState.DPadDown && !pad->OldState.DPadDown) {
        WriteDword(0x66BC14, 1);
        WriteDword(0x66BC0C, 1500);
        zoom = 320.0f;
        zoomOutTimer = CTimer_m_snTimeInMilliseconds;
    }

    if ( zoom > 160.0f ) {
        if ( CTimer_m_snTimeInMilliseconds - zoomOutTimer > 1000 ) {
            zoom -= 10.0f;
            if ( 160.0f > zoom ) {
                zoom = 160.0f;
            }
        }
    }

    DrawHud();
}

int _start()
{
    zoom = 160.0f;
    
    DrawHud = (void (*)())ReadCall(0x2471E4);
    RedirectCall(0x2471E4, showVehicleAreaZOomOutMinimap);
    MakeBranch(0x267D80, 0x27);
    MakeNop(0x267D84);

    RedirectCall(0x267E20, updateRadarZoom);
    MakeNop(0x267E24);

    return 0;
}
