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

void injectPatches() {
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
