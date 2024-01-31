#include <injector.h>
#include <CRadar.h>
#include <CPed.h>
#include <CPool.h>
#include <common.h>
#include <CVehicle.h>
#include <CTxdStore.h>
#include <CRGBA.h>
#include <math.h>

static void (*drawBlips)(void*) = (void (*)(void*))0x268080;
static char (*CWanted_SetPursuitCop)(void *, void *) = (char (*)(void *, void *))0x27D7E0;
static int (*GetPedHandle)(CPool*, int) = (int (*)(CPool*, int))0x0019FA20;
static float (*CGeneral_LimitRadianAngle)(float) = (float (*)(float))0x245130;

#define get_entity_model(entity) *(short *)((char*)entity + 34)

void hookedRadarCentre() {
    float radarShift = 25.0f;
    radarShift *= (*(float*)0x66B958 / 50.0f);
    float orientation = *(float*)0x66B95C;

    float a = orientation - 1.570796f;

    RadarOrigin.x -= cosf(a) * radarShift;
    RadarOrigin.y -= sinf(a) * radarShift;

    drawRadarMap();
}

int _start()
{
    RedirectCall(0x267F9C, hookedRadarCentre);
    return 0;
}
