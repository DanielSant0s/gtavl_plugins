#include <CRGBA.h>
#include <CFont.h>

void (*CFont_SetOrientation)(unsigned char) = (void (*)(unsigned char))0x2A8C50;
void (*CFont_SetColor)(RwRGBA*) = (void (*)())0x2A8900;
void (*CFont_SetBackground)(unsigned char, unsigned char) = (void (*)(unsigned char, unsigned char))0x002A8BE0;
void (*CFont_SetWrapx)(float) = (void (*)(float))0x002A8A60;
void (*CFont_SetScale)(float, float) = (void (*)(float, float))0x002A8800;
void (*CFont_SetFontStyle)(unsigned char) = (void (*)(unsigned char))0x002A89F0;
void (*CFont_SetProportional)(unsigned char) = (void (*)(unsigned char))0x002A8BD0;
void (*CFont_SetDropShadowPosition)(char) = (void (*)(char))0x002A8B90;
void (*CFont_SetDropColor)(RwRGBA*) = (void (*)())0x002A8AA0;
void (*CFont_PrintString)(float, float, const char*) = (void (*)(float, float, const char*))0x2A6E20;
void (*CFont_SetJustify)(unsigned char) = (void (*)(unsigned char))0x002A8C40;
void (*CFont_SetEdge)(unsigned char) = (void (*)(unsigned char))0x002A8BB0;

float (*CFont_GetStringWidth)(const char *string, bool unk1, bool unk2) = (float (*)(const char *string, bool unk1, bool unk2))0x2A78A0;

void (*CFont_GetTextRect)(CRect *rect, float x, float y, const char *text) = (void (*)(CRect *rect, float x, float y, const char *text))0x2A7720;

