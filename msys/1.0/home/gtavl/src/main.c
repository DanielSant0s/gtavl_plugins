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
#include <CPad.h>
#include <math.h>

#include "hooks.h"

static uint32_t LookBehindTimeLimit = 0;

bool CPad_GetLookBehindForCar(CPad *this)
{
    if ( this->DisablePlayerControls )
        return false;

    if ( this->NewState.ShockButtonR || this->OldState.ShockButtonR ) {
        if (!LookBehindTimeLimit) {
            LookBehindTimeLimit = CTimer_m_snTimeInMilliseconds + 500;
        } else {
            if ( CTimer_m_snTimeInMilliseconds > LookBehindTimeLimit ) {
                return true;
            }
        }
    } else {
        if (LookBehindTimeLimit) {
            LookBehindTimeLimit = 0;
        }
    }

    return false;
}

void (*CCustomRadar_LimitRadarPoint)(CVector*);

void injectPatches() {
    CCustomRadar_LimitRadarPoint = (void (*)(CVector*))ReadCall(0x268164);

    installOilSystem();
    injectPickupPatches();
    injectLimitHooks();
    installSkyEdgeEngine();
    injectRadioWheelPatches();
    injectMiscPatches();
    setupAimPatches();
    setupFOVPatches();
    setupCloudsPatches();
    setupRotatedDrawPatch();
    setupWastedBustedScreen();
    setupLoadingScreenPatches();
    setupColourFilterPatches();
    setupInGameMenuPatches();
    injectRadarPatches();
    injectHUDPatches();
    injectGPSPatches();
    setupPauseMenuPatches();
    injectVehiclePatches();
    injectPedRenderPatches();
    injectWeaponPatches();

    // Disable weapon icon drawing
    MakeNop(0x2ABA48);
    MakeNop(0x2ABA84);

    RedirectFunction(0x249470, CPad_GetLookBehindForCar);

    // Disable clock drawing
    WriteDword(0x2A8F18, 0xAF800BB0);
    WriteDword(0x66BCA0, 0);
}

int _start()
{
    injectPatches();
    return 0;
}
