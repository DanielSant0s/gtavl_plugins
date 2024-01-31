#include <stdint.h>
#include <CVector.h>

extern void (*CCustomRadar_LimitRadarPoint)(CVector*);

void DrawRadarCop(void* radar);

void loadHUDTexDict();

void RadioHud_process(int this);
void RadioHud_render(uint32_t this);

void setupFOVPatches();
void setupCameraPatches();
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

