
#include <injector.h>
#include <CCam.h>
#include "hooks.h"

extern float CDraw_ms_fFOV;

static float VehicleMaxFOV = 78.0f;
static float EmergencyVehicleMinFOV = 80.0f;
static float skyMultitude = 10.0f;

static void SetFOV(float fov)
{
    CDraw_ms_fFOV = fov * 1.22656f;
}

void setupFOVPatches() {
    RedirectCall(0x21DF84, &SetFOV);
    RedirectCall(0x242D54, &SetFOV);
    RedirectCall(0x21DF84, &SetFOV);

    *(uint32_t*)0x20A484 = 0x3C020000 + (*(uint32_t*)&VehicleMaxFOV >> 16); // Change max vehicle FOV
    *(uint32_t*)0x21DFE0 = 0x3C020000 + (*(uint32_t*)&EmergencyVehicleMinFOV >> 16); // Change emergency vehicle min FOV
    *(uint32_t*)0x2EE0E0 = 0x3C020000 + (*(uint32_t*)&skyMultitude >> 16); // Change sky multitude
    *(uint32_t*)0x2EE0E8 = 0x34420000 + (*(uint32_t*)&skyMultitude << 16); // Change sky multitude

    float vehRadarZoomMultiplier = 70.0f;
    float vehRadarMaxZoom = 230.0f;
    WriteDword(0x267DA0, 0x3C030000 + (*(uint32_t*)&vehRadarZoomMultiplier >> 16));
    WriteDword(0x267D98, 0x3C020000 + (*(uint32_t*)&vehRadarMaxZoom >> 16));
}
