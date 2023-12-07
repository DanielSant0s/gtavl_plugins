#include <stdint.h>
#include <injector.h>
#include <CRGBA.h>
#include <CTexture.h>
#include "hooks.h"

void hooked_draw_rotated(uint32_t texture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, RwRGBA* color)
{
    x1 *= 0.85f;
    x2 *= 0.85f;
    x3 *= 0.85f;
    x4 *= 0.85f;

    x1 += 48.0f;
    x2 += 48.0f;
    x3 += 48.0f;
    x4 += 48.0f;

    CSprite2d_Draw(texture, x1, y1, x2, y2, x3, y3, x4, y4, color);
}

void setupRotatedDrawPatch()
{
    RedirectCall(0x1F8A58, hooked_draw_rotated);
}
