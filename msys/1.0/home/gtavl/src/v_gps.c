/*
    Plugin-SDK (Grand Theft Auto) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include <stdio.h>
#include <math.h>
#include <injector.h>
#include <CVehicle.h>
#include <CRGBA.h>
#include <rwcore.h>
#include <CRadar.h>
#include <CFont.h>
#include <common.h>
#include "hooks.h"

typedef struct
{
    short m_nAreaId;
    short m_nNodeId;
} CNodeAddress;

#define MAX_NODE_POINTS 2000
#define GPS_LINE_WIDTH  2.5f
#define GPS_LINE_R  180
#define GPS_LINE_G  24
#define GPS_LINE_B  24
#define GPS_LINE_A  255
#define MAX_TARGET_DISTANCE 20.0f

static void Setup2dVertex(RwIm2DVertex *vertex, float x, float y) {
    vertex->u.els.scrVertex.x = x;
    vertex->u.els.scrVertex.y = y;
    vertex->u.els.v = 0.0f;
    vertex->u.els.u = 0.0f;
    vertex->u.els.camVertex_z = *NearScreenZ + 0.0001f;
    vertex->u.els.recipZ = *RecipNearClip;
    vertex->u.els.color.r = 168.0f;
    vertex->u.els.color.g = 0.0f;
    vertex->u.els.color.b = 140.0f;
    vertex->u.els.color.a = 255.0f;
}

static void (*DrawHud)();

void (*CHud_DrawGangOverlay)(int) = (void (*)(int))0x0026E830;
float (*CWorld_FindGroundZForCoord)(float, float) = (float (*)(float, float))0x002869C0;

static int gpsShown;
static float gpsDistance;
static CNodeAddress resultNodes[MAX_NODE_POINTS];
static CVector nodePoints[MAX_NODE_POINTS];
static RwIm2DVertex lineVerts[MAX_NODE_POINTS * 4];

unsigned int* FrontEndMenuManager_m_nTargetBlipIndex = (unsigned int*)0x6FF970;

void (*PreRenderWater)() = (void (*)())0x182130;

void autoDisableTarget() {
    CVector player_coords;

    PreRenderWater();

    FindPlayerCoors(&player_coords, 0);

    if (*FrontEndMenuManager_m_nTargetBlipIndex
        && CRadar_ms_RadarTrace[LOWORD(*FrontEndMenuManager_m_nTargetBlipIndex)].m_nCounter == HIWORD(*FrontEndMenuManager_m_nTargetBlipIndex)
        && CRadar_ms_RadarTrace[LOWORD(*FrontEndMenuManager_m_nTargetBlipIndex)].m_nBlipDisplay
        && FindPlayerPed(-1)
        && DistanceBetweenPoints(&player_coords, &CRadar_ms_RadarTrace[LOWORD(*FrontEndMenuManager_m_nTargetBlipIndex)].m_vecPos) < MAX_TARGET_DISTANCE)
    {
        CRadar_ClearBlip(*FrontEndMenuManager_m_nTargetBlipIndex);
        *FrontEndMenuManager_m_nTargetBlipIndex = 0;
    }
}

int (*CPathFind_DoPathSearch)(unsigned int*, unsigned char, CVector, CNodeAddress, CVector, CNodeAddress*, short *, int, float *, float, CNodeAddress*, float, int, CNodeAddress, int, int) = (int (*)(unsigned int, unsigned char, CVector, CNodeAddress, CVector, CNodeAddress*, short *, int, float *, float, CNodeAddress*, float, int, CNodeAddress, int, int))0x1E5930;

unsigned int* ThePaths = (unsigned int*)0x006A6180;

unsigned int CPathFind_FindNodePointer(void *this, CNodeAddress a2)
{
    return *((unsigned int *)this + a2.m_nAreaId + 513) + 28 * a2.m_nNodeId;
}

CVector CPathNode_GetCoors(unsigned int this)
{
    CVector out;

    out.x = (float)*(short *)(this + 8) * 0.125f;
    out.y = (float)*(short *)(this + 10) * 0.125f;
    out.z = (float)*(short *)(this + 12) * 0.125f;
    return out;
}


#define GS_SCISSOR_1    0x40

#   define MAKE128(RES, MSB, LSB) \
    __asm volatile ( "pcpyld %0, %1, %2" : "=r" (RES) : "r" (MSB), "r" (LSB))

#define _rwDMAPktPtr (*(__uint128_t**)0x66C170)

int (*_rwDMAOpenGIFPkt)(RwUInt32, RwUInt32) = (int (*)(RwUInt32, RwUInt32))0x34E378;
int (*_rwDMAOpenVIFPkt)(RwUInt32, RwUInt32) = (int (*)(RwUInt32, RwUInt32))0x34E878;

#define RWDMA_FIXUP     0x80000000

#define GIF_TAG(NLOOP,EOP,PRE,PRIM,FLG,NREG) \
                ((unsigned long long int)(NLOOP)	<< 0)	| \
                ((unsigned long long int)(EOP)	<< 15)	| \
                ((unsigned long long int)(PRE)	<< 46)	| \
                ((unsigned long long int)(PRIM)	<< 47)	| \
                ((unsigned long long int)(FLG)	<< 58)	| \
                ((unsigned long long int)(NREG)	<< 60);

#define GIF_AD 0x0E

#define ADDTOPKT(_a, _b) { __uint128_t _c; MAKE128(_c, _a, _b); *_rwDMAPktPtr++ = _c; }

unsigned int* current_buf = (unsigned int*)0x66C1B4;

void set_scissor(int x, int width, int y, int height){
    unsigned long long int tmp;

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = (x & 0x7ffl) | ((x+width) << 16) | ((y & 0x7ffl) <<32) | (((long)(y+height)) << 48);
    ADDTOPKT(GS_SCISSOR_1, tmp);
}

void (*RwIm2DRenderPrimitive)(RwInt32, RwIm2DVertex*, RwInt32) = (void (*)(RwInt32, RwIm2DVertex*, RwInt32))0x3669E8;
int (*RwRenderStateSet)(RwInt32, void*) = (int (*)(RwInt32, void*))0x34FD68;

void processGPS(int b) {
    CVector player_coords;
    CNodeAddress dummy;
    short i;
    
    CHud_DrawGangOverlay(b);

    gpsShown = 0;
    unsigned int playa = FindPlayerPed(0);

    if (playa
        && *(unsigned int *)(playa + 1484)
        && (*(unsigned char *)(playa + 0x4A5) & 1)
        && *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) != VEHICLE_PLANE
        && *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) != VEHICLE_HELI
        && *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) != VEHICLE_BMX
        && *FrontEndMenuManager_m_nTargetBlipIndex
        && CRadar_ms_RadarTrace[LOWORD(*FrontEndMenuManager_m_nTargetBlipIndex)].m_nCounter == HIWORD(*FrontEndMenuManager_m_nTargetBlipIndex)
        && CRadar_ms_RadarTrace[LOWORD(*FrontEndMenuManager_m_nTargetBlipIndex)].m_nBlipDisplay)
    {
        CVector destPosn = CRadar_ms_RadarTrace[LOWORD(*FrontEndMenuManager_m_nTargetBlipIndex)].m_vecPos;
        destPosn.z = CWorld_FindGroundZForCoord(destPosn.x, destPosn.y);
        
        short nodesCount = 0;

        FindPlayerCoors(&player_coords, 0);
        
        CPathFind_DoPathSearch(ThePaths, 0, player_coords, dummy, destPosn, resultNodes, &nodesCount, MAX_NODE_POINTS, &gpsDistance,
            999999.0f, 0, 999999.0f, 0, dummy, 0, *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) == VEHICLE_BOAT);
        
        if (nodesCount > 0) {
            for (i = 0; i < nodesCount; i++) {
                CVector nodePosn = CPathNode_GetCoors(CPathFind_FindNodePointer(ThePaths, resultNodes[i])); // ThePaths.GetPathNode(resultNodes[i])->GetNodeCoors();
                CVector tmpPoint, nodePosn2D;
                nodePosn2D.x = nodePosn.x;
                nodePosn2D.y = nodePosn.y;

                CRadar_TransformRealWorldPointToRadarSpace(&tmpPoint, &nodePosn2D);

                if (!*(unsigned char*)(FrontEndMenuManager + 101))
                    CRadar_TransformRadarPointToScreenSpace(&nodePoints[i], &tmpPoint);
                else {
                    rectLimitRadarPoint(&tmpPoint);
                    CRadar_TransformRadarPointToScreenSpace(&nodePoints[i], &tmpPoint);
                    CRadar_LimitToMap(&nodePoints[i].x, &nodePoints[i].y);
                }
            }

            if (!*(unsigned char*)(FrontEndMenuManager + 101))
            {
                CVector posn1, posn2, start, end;

                start.x = -1.0f;
                start.y = -1.0f;

                end.x = 1.0f;
                end.y = 1.0f;

                CRadar_TransformRadarPointToScreenSpace(&posn1, &start);
                CRadar_TransformRadarPointToScreenSpace(&posn2, &end);
                set_scissor((int)(posn1.x + 2.0f), (int)(posn2.x - posn1.x - 2.0f), (int)(posn2.y + 2.0f), (int)(posn1.y - posn2.y - 2.0f));
            }

            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            unsigned int vertIndex = 0;

            for (i = 0; i < (nodesCount - 1); i++) {
                CVector point[4], shift[2];
                CVector dir;
                dir.x = nodePoints[i + 1].x - nodePoints[i].x;
                dir.y = nodePoints[i + 1].y - nodePoints[i].y;
                float angle = atan2f(dir.y, dir.x);
                if (!*(unsigned char*)(FrontEndMenuManager + 101)) {
                    shift[0].x = cosf(angle - 1.5707963f) * GPS_LINE_WIDTH;
                    shift[0].y = sinf(angle - 1.5707963f) * GPS_LINE_WIDTH;
                    shift[1].x = cosf(angle + 1.5707963f) * GPS_LINE_WIDTH;
                    shift[1].y = sinf(angle + 1.5707963f) * GPS_LINE_WIDTH;
                } else {
                    float mp = *(float*)(FrontEndMenuManager + 112) - 140.0f;
                    if (mp < 140.0f)
                        mp = 140.0f;
                    else if (mp > 960.0f)
                        mp = 960.0f;
                    mp = mp / 960.0f + 0.4f;
                    shift[0].x = cosf(angle - 1.5707963f) * GPS_LINE_WIDTH * mp;
                    shift[0].y = sinf(angle - 1.5707963f) * GPS_LINE_WIDTH * mp;
                    shift[1].x = cosf(angle + 1.5707963f) * GPS_LINE_WIDTH * mp;
                    shift[1].y = sinf(angle + 1.5707963f) * GPS_LINE_WIDTH * mp;
                }
                Setup2dVertex(&lineVerts[vertIndex + 0], nodePoints[i].x + shift[0].x, nodePoints[i].y + shift[0].y);
                Setup2dVertex(&lineVerts[vertIndex + 1], nodePoints[i + 1].x + shift[0].x, nodePoints[i + 1].y + shift[0].y);
                Setup2dVertex(&lineVerts[vertIndex + 2], nodePoints[i].x + shift[1].x, nodePoints[i].y + shift[1].y);
                Setup2dVertex(&lineVerts[vertIndex + 3], nodePoints[i + 1].x + shift[1].x, nodePoints[i + 1].y + shift[1].y);

                vertIndex += 4;
            }
        
            RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, lineVerts, 4 * (nodesCount - 1));

            
            if (!*(unsigned char*)(FrontEndMenuManager + 101))
            {
                set_scissor(0, 639, 0, 447);
            }
            
            CVector tmp = CPathNode_GetCoors(CPathFind_FindNodePointer(ThePaths, resultNodes[0]));
            gpsDistance += DistanceBetweenPoints(&player_coords, &tmp);
            gpsShown = 1;
        }

    }

}

static RwRGBA font_color;
static RwRGBA font_shadow;

void drawGPSDistance() {
    DrawHud();

    if (gpsShown) {
        CFont_SetOrientation(ALIGN_LEFT);
        CFont_SetColor(&font_color);
        CFont_SetBackground(0, 0);
        CFont_SetWrapx(70.0f);
        CFont_SetScale(0.3111112f, 0.8f);
        CFont_SetFontStyle(FONT_SUBTITLES);
        CFont_SetProportional(1);
        CFont_SetDropShadowPosition(0);
        CFont_SetDropColor(&font_shadow);
        CVector radarBottom, tmp;
        tmp.x = -1.0f;
        tmp.y = -1.0f;
        CRadar_TransformRadarPointToScreenSpace(&radarBottom, &tmp);
        char text[16];
        if (gpsDistance > 1000.0f)
            sprintf(text, "%.2fkm", gpsDistance / 1000.0f);
        else
            sprintf(text, "%dm", (int)gpsDistance);
        CFont_PrintString(radarBottom.x + 4.0f, radarBottom.y - 18.0f, text);
    }
}

void setupGPSPatches()
{  
    CRGBA_CRGBA(&font_color, 255, 255, 255, 255);
    CRGBA_CRGBA(&font_shadow, 0, 0, 0, 0);
    RedirectCall(0x26F2F4, processGPS);
    RedirectCall(0x239090, processGPS);
    RedirectCall(0x2437E8, autoDisableTarget);
    
    DrawHud = (void (*)())ReadCall(0x2471E4);
    RedirectCall(0x2471E4, drawGPSDistance);
}
