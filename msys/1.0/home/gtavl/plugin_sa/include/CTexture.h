#ifndef CTEXTURE_H_
#define CTEXTURE_H_

#include <rwcore.h>

typedef struct
{
    RwTexture* texture;
} CTexture;

void CSprite2d_SetTexture(CTexture*, const char*, const char*);
void CSprite2d_Delete(CTexture *);
void CSprite2d_Draw(uint32_t, float, float, float, float, float, float, float, float, RwRGBA*);
void CSprite2d_DrawSprite(CTexture*, float, float, float, float, RwRGBA*);
void CSprite2d_Draw2DPolygon(float, float, float, float, float, float, float, float, RwRGBA*);
void CSprite2d_SetAddressing(CTexture *, uint8_t addressing);

#endif
