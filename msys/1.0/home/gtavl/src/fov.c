
#include <injector.h>
#include "hooks.h"

float* CDraw_ms_fFOV = (float*)0x664B80;
float VehicleMaxFOV = 78.0f;
float EmergencyVehicleMinFOV = 80.0f;
float skyMultitude = 10.0f;

void SetFOV(float fov)
{
    *(float *)CDraw_ms_fFOV = fov * 1.22656f;
}

void setupFOVPatches() {
    RedirectCall(0x21DF84, SetFOV);
    RedirectCall(0x242D54, SetFOV);
    RedirectCall(0x21DF84, SetFOV);

    *(uint32_t*)0x20A484 = 0x3C020000 + (*(uint32_t*)&VehicleMaxFOV >> 16); // Change max vehicle FOV
    *(uint32_t*)0x21DFE0 = 0x3C020000 + (*(uint32_t*)&EmergencyVehicleMinFOV >> 16); // Change emergency vehicle min FOV
    *(uint32_t*)0x2EE0E0 = 0x3C020000 + (*(uint32_t*)&skyMultitude >> 16); // Change sky multitude
    *(uint32_t*)0x2EE0E8 = 0x34420000 + (*(uint32_t*)&skyMultitude << 16); // Change sky multitude
}
