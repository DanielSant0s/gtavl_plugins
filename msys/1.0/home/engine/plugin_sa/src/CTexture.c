#include <CTexture.h>
#include <CRGBA.h>

void (*CSprite2D_SetTexture)(CTexture*, const char*) = (void (*)(void*, const char*))0x2B07A0;
void (*CSprite2d_Delete)(CTexture *this) = (void (*)(CTexture *this))0x2B0750;
void (*CSprite2d_Draw)(uint32_t, float, float, float, float, float, float, float, float, RwRGBA*) = (void (*)(uint32_t, float, float, float, float, float, float, float, float, RwRGBA*))0x2B09D0;
void (*CSprite2d_DrawSprite)(CTexture*, float, float, float, float, RwRGBA*) = (void (*)(CTexture*, float, float, float, float, RwRGBA*))0x2B08D0;
void (*CSprite2d_Draw2DPolygon)(float, float, float, float, float, float, float, float, RwRGBA*) = (void (*)(float, float, float, float, float, float, float, float, RwRGBA*))0x2B0A40;