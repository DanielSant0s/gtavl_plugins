#ifndef CCORONAS_H_
#define CCORONAS_H_

#include <stdint.h>

enum eCoronaType
{
    CORONATYPE_SHINYSTAR,
    CORONATYPE_HEADLIGHT,
    CORONATYPE_MOON,
    CORONATYPE_REFLECTION,
    CORONATYPE_HEADLIGHTLINE,
    CORONATYPE_HEX,
    CORONATYPE_CIRCLE,
    CORONATYPE_RING,
    CORONATYPE_STREAK,
    CORONATYPE_TORUS,
    CORONATYPE_NONE
};

enum eCoronaFlareType
{
    FLARETYPE_NONE,
    FLARETYPE_SUN,
    FLARETYPE_HEADLIGHTS
};

void (*CCoronas_RegisterCorona)(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t, CVector*,
 float, float, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, float, int, float, int, float, int, int) = (
	void (*)(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t, CVector*,
 float, float, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, float, int, float, int, float, int, int)
 )0x299380;

#endif
