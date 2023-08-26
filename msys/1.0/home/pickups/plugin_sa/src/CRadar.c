#include <CRadar.h>

tRadarTrace* CRadar_ms_RadarTrace = (tRadarTrace*)0x701F00;

void* (*CRadar_SetEntityBlip)(int, void *, int, int) = (void* (*)(int, void *, int, int))0x26B510;
void (*CRadar_ChangeBlipScale)(void *, int) = (void (*)(void *, int))0x26BD30;
void (*CRadar_ChangeBlipDisplay)(void *, int) = (void (*)(void *, int))0x26BDE0;
void (*CRadar_DrawRotatingRadarSprite)(CTexture *, float, float, float, uint32_t, uint32_t, RwRGBA) = (void (*)(CTexture *, float, float, float, uint32_t, uint32_t, RwRGBA))0x26D000;
void (*CRadar_TransformRealWorldPointToRadarSpace)(CVector*, CVector*) = (void (*)(CVector*, CVector*))0x26AEC0;
void (*CRadar_TransformRadarPointToScreenSpace)(CVector*, CVector*) = (void (*)(CVector*, CVector*))0x0026AE00;
void (*CRadar_LimitRadarPoint)(CVector*) = (void (*)(CVector*))0x0026ABA0;
void (*CRadar_LimitToMap)(float *, float *) = (void (*)(float *, float *))0x0026AC30;
void (*drawRadarMap)() = (void (*)())0x26F090;
void (*changeBlipColour)(void*, int) = (void (*)(void*, int))0x26BA30;
void (*CRadar_ClearBlip)(int) = (void (*)(int))0x26B9A0;
CVector* RadarOrigin = (CVector*)0x66B950;

