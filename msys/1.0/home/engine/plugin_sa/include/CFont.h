#include <CRGBA.h>
#include <common.h>

enum eFontAlignment {
    ALIGN_CENTER,
    ALIGN_LEFT,
    ALIGN_RIGHT
};

enum eFontStyle {
    FONT_GOTHIC,
    FONT_SUBTITLES,
    FONT_MENU,
    FONT_PRICEDOWN
};

extern void (*CFont_SetOrientation)(unsigned char);
extern void (*CFont_SetColor)(RwRGBA*);
extern void (*CFont_SetBackground)(unsigned char, unsigned char);
extern void (*CFont_SetWrapx)(float);
extern void (*CFont_SetScale)(float, float);
extern void (*CFont_SetFontStyle)(unsigned char);
extern void (*CFont_SetProportional)(unsigned char);
extern void (*CFont_SetDropShadowPosition)(char);
extern void (*CFont_SetDropColor)(RwRGBA*);
extern void (*CFont_PrintString)(float, float, const char*);
extern void (*CFont_SetJustify)(unsigned char);
extern void (*CFont_SetEdge)(unsigned char);

extern float (*CFont_GetStringWidth)(const char *string, bool unk1, bool unk2);
extern void (*CFont_GetTextRect)(CRect *rect, float x, float y, const char *text);
