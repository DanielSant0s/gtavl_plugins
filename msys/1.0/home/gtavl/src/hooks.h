#include <stdint.h>
#include <CVector.h>

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

