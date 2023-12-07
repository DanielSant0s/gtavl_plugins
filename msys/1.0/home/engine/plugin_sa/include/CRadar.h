#ifndef CRADAR_H_
#define CRADAR_H_

#include <CVector.h>
#include <CTexture.h>

typedef struct {
    unsigned int   m_nColour; // see eBlipColour
    unsigned int   m_nEntityHandle;
    CVector        m_vecPos;
    unsigned short m_nCounter;
    float          m_fSphereRadius;
    unsigned short m_nBlipSize;
    unsigned int  *m_pEntryExit;
    unsigned char  m_nRadarSprite; // see eRadarSprite
    unsigned char  m_bBright : 1; // It makes use of bright colors. Always set.
    unsigned char  m_bInUse : 1; // It is available.
    unsigned char  m_bShortRange : 1; // It doesn't show permanently on the radar.
    unsigned char  m_bFriendly : 1; // It is affected by BLIP_COLOUR_THREAT.   
    unsigned char  m_bBlipRemain : 1; // It has the priority over the entity (it will still appear after the entity's deletion).
    unsigned char  m_bBlipFade : 1; // Possibly a leftover. Always unset (unused).
    unsigned char  m_nCoordBlipAppearance : 2; // see eBlipAppearance
    unsigned char  m_nBlipDisplay : 2; // see eBlipDisplay
    unsigned char  m_nBlipType : 4; // see eBlipType
} tRadarTrace;

tRadarTrace* CRadar_ms_RadarTrace;

extern void* (*CRadar_SetEntityBlip)(int, void *, int, int);
extern void (*CRadar_ChangeBlipScale)(void *, int);
extern void (*CRadar_ChangeBlipDisplay)(void *, int);
extern void (*CRadar_DrawRotatingRadarSprite)(CTexture *, float, float, float, uint32_t, uint32_t, RwRGBA);
extern void (*CRadar_TransformRealWorldPointToRadarSpace)(CVector*, CVector*);
extern void (*CRadar_TransformRadarPointToScreenSpace)(CVector*, CVector*);
extern void (*CRadar_LimitRadarPoint)(CVector*);
extern void (*CRadar_LimitToMap)(float *, float *);
extern void (*CRadar_ClearBlip)(int);
extern void (*drawRadarMap)();
extern void (*changeBlipColour)(void*, int);
extern CVector* RadarOrigin;

#endif
