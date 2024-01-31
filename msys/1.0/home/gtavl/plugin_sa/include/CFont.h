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

void CFont_SetOrientation(unsigned char);
void CFont_SetColor(RwRGBA*);
void CFont_SetBackground(unsigned char, unsigned char);
void CFont_SetWrapx(float);
void CFont_SetScale(float, float);
void CFont_SetFontStyle(unsigned char);
void CFont_SetProportional(unsigned char);
void CFont_SetDropShadowPosition(char);
void CFont_SetDropColor(RwRGBA*);
void CFont_PrintString(float, float, const char*);
void CFont_SetJustify(unsigned char);
void CFont_SetEdge(unsigned char);

float CFont_GetStringWidth(const char *string, bool unk1, bool unk2);
void CFont_GetTextRect(CRect *rect, float x, float y, const char *text);
