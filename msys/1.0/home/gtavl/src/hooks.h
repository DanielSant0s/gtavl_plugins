#include <stdint.h>
#include <CVector.h>

extern void (*CCustomRadar_LimitRadarPoint)(CVector*);

void setupFOVPatches();
void setupCloudsPatches();
void setupAimPatches();
void setupRotatedDrawPatch();
void setupWastedBustedScreen();
void setupLoadingScreenPatches();
void setupColourFilterPatches();
void setupInGameMenuPatches();
void injectHUDPatches();
void injectRadarPatches();
void injectMiscPatches();
void injectRadioWheelPatches();
void setupPauseMenuPatches();
void injectLimitHooks();
void injectGPSPatches();
void injectVehiclePatches();
void installSkyEdgeEngine();
void injectPickupPatches();
void installOilSystem();
void injectPedRenderPatches();
void injectWeaponPatches();

