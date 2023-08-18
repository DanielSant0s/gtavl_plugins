#include <stdint.h>
#include <CVector.h>

void hookedRadarCentre();
void DrawRadarCop(void* radar);

void loadHUDTexDict();

void RadioHud_process(int this);

void setupVehiclePatches();
void setupFOVPatches();
void setupGPSPatches();
void setupPickupPatches();
void setupCameraPatches();
void setupAimPatches();
void setupRotatedDrawPatch();
void setupRectangularRadar();
void setupWastedBustedScreen();
void setupDebugStatsMenu();
void setupLoadingScreenPatches();
void setupColourFilterPatches();
