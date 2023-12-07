#ifndef CTEXTURE_H_
#define CTEXTURE_H_

#include <rwcore.h>

typedef struct
{
    RwTexture* texture;
} CTexture;

extern void (*CSprite2D_SetTexture)(CTexture*, const char*);
extern void (*CSprite2d_Delete)(CTexture *this);
extern void (*CSprite2d_Draw)(uint32_t, float, float, float, float, float, float, float, float, RwRGBA*);
extern void (*CSprite2d_DrawSprite)(CTexture*, float, float, float, float, RwRGBA*);
extern void (*CSprite2d_Draw2DPolygon)(float, float, float, float, float, float, float, float, RwRGBA*);


#endif
