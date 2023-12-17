#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CRGBA.h>
#include <CVector.h>
#include <CVehicle.h>
#include <CTexture.h>
#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <CCam.h>
#include <math.h>

#include "hooks.h"

void (*CCustomRadar_LimitRadarPoint)(CVector*);

void injectPatches() {
    CCustomRadar_LimitRadarPoint = (void (*)(CVector*))ReadCall(0x268164);

    RedirectCall(0x564870, RadioHud_process);
    RedirectJump(0x580FE4, RadioHud_render);

    injectMiscPatches();
    setupAimPatches();
    setupFOVPatches();
    setupCameraPatches();
    setupRotatedDrawPatch();
    setupWastedBustedScreen();
    setupLoadingScreenPatches();
    setupColourFilterPatches();
    setupInGameMenuPatches();
    injectRadarPatches();
    injectHUDPatches();
}

int _start()
{
    injectPatches();
    return 0;
}
