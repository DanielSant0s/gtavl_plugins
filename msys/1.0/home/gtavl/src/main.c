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

#include <sifdev.h>

#include "virtual_mem.h"

uint32_t LookBehindTimeLimit = 0;

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


inline void nopdelay(void)
{
    int i = 0xfffff;

    do {
        __asm__("nop\nnop\nnop\nnop\nnop\n");
    } while (i-- != -1);
}

bool wait_device(char *path) {
    struct sce_stat buffer;
    int ret = -1;
    int retries = 500;

    while(ret != 0 && retries > 0) {
        ret = sceGetstat(path, &buffer);
        /* Wait untill the device is ready */
        nopdelay();

        retries--;
    }

    return ret == 0;
}

void injectPatches() {
    CCustomRadar_LimitRadarPoint = (void (*)(CVector*))ReadCall(0x268164);

    installOilSystem();
    injectLimitHooks();
    setupColourFilterPatches();
    injectHUDPatches();

    injectPickupPatches();
    installSkyEdgeEngine();
    injectRadioWheelPatches();
    injectMiscPatches();
    setupAimPatches();
    setupFOVPatches();
    setupCloudsPatches();
    setupRotatedDrawPatch();
    setupWastedBustedScreen();
    setupLoadingScreenPatches();
    setupInGameMenuPatches();
    injectRadarPatches();
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
    if (sceSifSearchModuleByName("usbd") < 0) {
        sceSifLoadModule("cdrom0:\\SYSTEM\\USBD.IRX", 0, 0);
    }

    if (sceSifSearchModuleByName("bdm") < 0) {
        //sceSifLoadModule("cdrom0:\\SYSTEM\\USBHDFSD.IRX", 0, 0);
        sceSifLoadModule("cdrom0:\\SYSTEM\\BDM.IRX", 0, 0);
        sceSifLoadModule("cdrom0:\\SYSTEM\\BDMFS_FATFS.IRX", 0, 0);
        sceSifLoadModule("cdrom0:\\SYSTEM\\USBMASS_BD.IRX", 0, 0);
    }

    br_init(true);

    wait_device("mass:");

    init_virtual_memory("mass:gtavl.vmem", MB(32)); 

    injectPatches();

    return 0;
}
