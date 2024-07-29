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
#include <stdlib.h>
#include <rwcore.h>
#include <secore.h>
#include <CRadar.h>
#include <CFont.h>
#include <common.h>

#include "hooks.h"

float CWorld_FindGroundZForCoord(float, float);

typedef struct
{
    short m_nAreaId;
    short m_nNodeId;
} CNodeAddress;

#define MAX_NODE_POINTS 1000

#define gps_line_width 2.5f
#define max_target_distance 20.0f

#define gps_line_color_r 180.0f
#define gps_line_color_g 24.0f
#define gps_line_color_b 255.0f
#define gps_line_color_a 255.0f

#define distance_text_color_r 255
#define distance_text_color_g 255
#define distance_text_color_b 255
#define distance_text_color_a 255

#define distance_text_shadow_r 0
#define distance_text_shadow_g 0
#define distance_text_shadow_b 0
#define distance_text_shadow_a 255

RwRGBAReal line_color;

static float (*limitRadarPoint)(float *a1);
static void (*DrawHud)();

void CHud_DrawGangOverlay(int);

static bool gpsShown;

static float gpsDistance;

//static CNodeAddress resultNodes[MAX_NODE_POINTS];
//static CVector nodePoints[MAX_NODE_POINTS];
//static RwIm2DVertex lineVerts[MAX_NODE_POINTS * 4];



static CNodeAddress *resultNodes = NULL;
static CVector *nodePoints = NULL;
static RwIm2DVertex* lineVerts = NULL;

static bool missionRouteShown = false;

extern unsigned int FrontEndMenuManager_m_nTargetBlipIndex;

void PreRenderWater();

void autoDisableTarget() {
    CVector player_coords;

    PreRenderWater();

    FindPlayerCoors(&player_coords, 0);

    if (FrontEndMenuManager_m_nTargetBlipIndex
        && CRadar_ms_RadarTrace[LOWORD(FrontEndMenuManager_m_nTargetBlipIndex)].m_nCounter == HIWORD(FrontEndMenuManager_m_nTargetBlipIndex)
        && CRadar_ms_RadarTrace[LOWORD(FrontEndMenuManager_m_nTargetBlipIndex)].m_nBlipDisplay
        && FindPlayerPed(-1)
        && DistanceBetweenPoints(&player_coords, &CRadar_ms_RadarTrace[LOWORD(FrontEndMenuManager_m_nTargetBlipIndex)].m_vecPos) < max_target_distance)
    {
        CRadar_ClearBlip(FrontEndMenuManager_m_nTargetBlipIndex);
        FrontEndMenuManager_m_nTargetBlipIndex = 0;
        gpsShown = false;
    }
}

int CPathFind_DoPathSearch(unsigned int*, unsigned char, CVector, CNodeAddress, CVector, CNodeAddress*, short *, int, float *, float, CNodeAddress*, float, int, CNodeAddress, int, int);

extern unsigned int ThePaths;

unsigned int CPathFind_FindNodePointer(void *this, CNodeAddress a2)
{
    return *((unsigned int *)this + a2.m_nAreaId + 513) + 28 * a2.m_nNodeId;
}

void CPathNode_GetCoors(CVector* out, unsigned int this)
{
    out->x = (float)(*(short *)(this + 8) * 0.125f);
    out->y = (float)(*(short *)(this + 10) * 0.125f);
    out->z = (float)(*(short *)(this + 12) * 0.125f);
}

void Setup2dVertex(RwIm2DVertex *vertex, float x, float y) {
    vertex->u.els.scrVertex.x = x;
    vertex->u.els.scrVertex.y = y;
    vertex->u.els.scrVertex.z = NearScreenZ - 0.5f;
    vertex->u.els.u = 0.0f;
    vertex->u.els.v = 0.0f;
    vertex->u.els.camVertex_z = NearScreenZ - 0.5f;
    vertex->u.els.recipZ = RecipNearClip;
    vertex->u.els.color = line_color;
}

void processGPS(CVector* dest_coords) {
    CVector player_coords;
    unsigned int i;

    FindPlayerCoors(&player_coords, 0);

    short nodesCount = 0;

    gpsShown = false;
    unsigned int playa = FindPlayerPed(0);

    CNodeAddress dummy[2];
    dummy[0].m_nAreaId = -1;
    dummy[0].m_nNodeId = -1;
    dummy[1].m_nAreaId = -1;
    dummy[1].m_nNodeId = -1;

    CVector destPosn;
    destPosn.x = dest_coords->x;
    destPosn.y = dest_coords->y;
    destPosn.z = dest_coords->z;

        CPathFind_DoPathSearch(&ThePaths, 0, player_coords, dummy[0], destPosn, resultNodes, &nodesCount, MAX_NODE_POINTS, &gpsDistance,
            999999.0f, NULL, 999999.0f, false, dummy[1], false, *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) == VEHICLE_BOAT);

        if (nodesCount > 0) {

            for (i = 0; i < nodesCount; i++) {
                CVector tmpPoint, nodePosn2D;

                CPathNode_GetCoors(&nodePosn2D, CPathFind_FindNodePointer(&ThePaths, resultNodes[i])); // ThePaths.GetPathNode(resultNodes[i])->GetNodeCoors();

                CRadar_TransformRealWorldPointToRadarSpace(&tmpPoint, &nodePosn2D);

                if (!*(unsigned char*)((uint32_t)(&FrontEndMenuManager) + 101))
                    CRadar_TransformRadarPointToScreenSpace(&nodePoints[i], &tmpPoint);
                else {
                    limitRadarPoint(&tmpPoint);
                    CRadar_TransformRadarPointToScreenSpace(&nodePoints[i], &tmpPoint);
                    CRadar_LimitToMap(&nodePoints[i].x, &nodePoints[i].y);
                }
            }

            if (!*(unsigned char*)((uint32_t)(&FrontEndMenuManager) + 101))
            {
                CVector posn, start, end;
                Rect rect;

                start.x = -1.0f;
                start.y = -1.0f;

                end.x = 1.0f;
                end.y = 1.0f;

                CRadar_TransformRadarPointToScreenSpace(&posn, &start);
                rect.left = posn.x;
                rect.bottom = posn.y;

                CRadar_TransformRadarPointToScreenSpace(&posn, &end);
                rect.right = posn.x;
                rect.top = posn.y;

                GsRenderStateSet(GS_SCISSORTESTENABLE, 1);
                SetScissorRect(&rect);
            }

            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
            
            unsigned int vertIndex = 0;
            short lasti;
            CVector shift[2];
            for (i = 0; i < (nodesCount - 1); i++) {
                CVector dir;

                dir.x = nodePoints[i + 1].x - nodePoints[i].x;
                dir.y = nodePoints[i + 1].y - nodePoints[i].y;

                float angle = atan2f(dir.y, dir.x);

                if (!*(unsigned char*)((uint32_t)(&FrontEndMenuManager) + 101)) {
                    shift[0].x = cosf(angle - 1.5707963f) * gps_line_width;
                    shift[0].y = sinf(angle - 1.5707963f) * gps_line_width;
                    shift[1].x = cosf(angle + 1.5707963f) * gps_line_width;
                    shift[1].y = sinf(angle + 1.5707963f) * gps_line_width;
                } else {
                    float mp = *(float*)((uint32_t)(&FrontEndMenuManager) + 112) - 140.0f;
                    if (mp < 140.0f)
                        mp = 140.0f;
                    else if (mp > 960.0f)
                        mp = 960.0f;
                    mp = mp / 960.0f + 0.4f;
                    shift[0].x = cosf(angle - 1.5707963f) * gps_line_width * mp;
                    shift[0].y = sinf(angle - 1.5707963f) * gps_line_width * mp;
                    shift[1].x = cosf(angle + 1.5707963f) * gps_line_width * mp;
                    shift[1].y = sinf(angle + 1.5707963f) * gps_line_width * mp;
                }

            

                Setup2dVertex(                //
                    &lineVerts[vertIndex + 0],       //
                    nodePoints[i].x + shift[0].x,   // CurrentNode*
                    nodePoints[i].y + shift[0].y   //
                );

                Setup2dVertex(                //
                    &lineVerts[vertIndex + 1],       //
                    nodePoints[i].x + shift[1].x,   // CurrentNode - CurrentNode*
                    nodePoints[i].y + shift[1].y   //
                );

                Setup2dVertex(                    // NextNode*
                    &lineVerts[vertIndex + 2],           //    |
                    nodePoints[i + 1].x + shift[0].x,   // CurrentNode - CurrentNode
                    nodePoints[i + 1].y + shift[0].y   //
                );

                Setup2dVertex(
                    &lineVerts[vertIndex + 3],           // NextNode - NextNode*
                    nodePoints[i + 1].x + shift[1].x,   //    |             |
                    nodePoints[i + 1].y + shift[1].y   // CurrentNode - CurrentNode
                );

                lasti = i+1;
                vertIndex += 4;
            }


            // Create end segment
            CVector2D targetScreen;
            CVector2D tmpPoint;
            CRadar_TransformRealWorldPointToRadarSpace(&tmpPoint, &destPosn);
            if (!*(unsigned char*)((uint32_t)(&FrontEndMenuManager) + 101))
                CRadar_TransformRadarPointToScreenSpace(&targetScreen, &tmpPoint);
            else {
                limitRadarPoint(&tmpPoint);
                CRadar_TransformRadarPointToScreenSpace(&targetScreen, &tmpPoint);
                targetScreen.x *= 1.0f;
                targetScreen.y *= 1.0f;
                CRadar_LimitToMap(&targetScreen.x, &targetScreen.y);
            }

            CVector2D dir;
            dir.x = targetScreen.x - nodePoints[lasti].x; // Direction between last node and the target position
            dir.y = targetScreen.y - nodePoints[lasti].y; // Direction between last node and the target position
            float angle = atan2f(dir.y, dir.x); // Convert direction to angle

            if (!*(unsigned char*)((uint32_t)(&FrontEndMenuManager) + 101)) {
                // 1.5707963 radians = 90 degrees

                shift[0].x = cosf(angle - 1.5707963f) * gps_line_width;
                shift[0].y = sinf(angle - 1.5707963f) * gps_line_width;
                shift[1].x = cosf(angle + 1.5707963f) * gps_line_width;
                shift[1].y = sinf(angle + 1.5707963f) * gps_line_width;
            }
            else {
                float mp = *(float*)((uint32_t)(&FrontEndMenuManager) + 112) - 140.0f;
                if (mp < 140.0f)
                    mp = 140.0f;
                else if (mp > 960.0f)
                    mp = 960.0f;
                mp = mp / 960.0f + 0.4f;
                shift[0].x = cosf(angle - 1.5707963f) * gps_line_width * mp;
                shift[0].y = sinf(angle - 1.5707963f) * gps_line_width * mp;
                shift[1].y = sinf(angle + 1.5707963f) * gps_line_width * mp;
                shift[1].x = cosf(angle + 1.5707963f) * gps_line_width * mp;
            }

            Setup2dVertex(
                &lineVerts[vertIndex+0],
                nodePoints[lasti].x + shift[0].x,
                nodePoints[lasti].y + shift[0].y
            );

            Setup2dVertex(
                &lineVerts[vertIndex+1],
                nodePoints[lasti].x + shift[1].x,
                nodePoints[lasti].y + shift[1].y
            );

            Setup2dVertex(
                &lineVerts[vertIndex + 2],
                (nodePoints[lasti].x + (dir.x / 4.8f)) + (shift[0].x / 2),
                (nodePoints[lasti].y + (dir.y / 4.8f)) + (shift[0].y / 2)
            );

            Setup2dVertex(
                &lineVerts[vertIndex + 3],
                (nodePoints[lasti].x + (dir.x / 4.8f)) + (shift[1].x / 2),
                (nodePoints[lasti].y + (dir.y / 4.8f)) + (shift[1].y / 2)
            );

            Setup2dVertex(
                &lineVerts[vertIndex + 4],
                (nodePoints[lasti].x + (dir.x / 4.5f)),
                (nodePoints[lasti].y + (dir.y / 4.5f))
            );

            RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, lineVerts, (4 * nodesCount)+1);


            if (!*(unsigned char*)((uint32_t)(&FrontEndMenuManager) + 101))
            {
                GsRenderStateSet(GS_SCISSORTESTENABLE, 0);
            }

            CVector tmp;
            CPathNode_GetCoors(&tmp, CPathFind_FindNodePointer(&ThePaths, resultNodes[0]));
            gpsDistance += DistanceBetweenPoints(&player_coords, &tmp);
            gpsShown = true;
        }

}

void renderMissionTrace(tRadarTrace* trace) {
    CVector destVec;
    switch (trace->m_nBlipType) {
    case 1:
        //if (cfg.ENABLE_MOVING)
        //    destVec = CPools_GetVehicle(trace->m_nEntityHandle)->GetPosition();
        //else
            return;
        break;
    case 2:
        //if(cfg.ENABLE_MOVING)
        //    destVec = CPools_GetPed(trace->m_nEntityHandle)->GetPosition();
        //else
            return;
        break;
    case 3:
        //destVec = CPools_GetObject(trace->m_nEntityHandle)->GetPosition();
        return;
        break;
    case 6: // Searchlights
    case 8: // Airstripts
    case 7: // Pickups
    case 0: // NONE???
        return;
    default:
        destVec = trace->m_vecPos;
        break;
    }

    line_color.r = 220.0f;
    line_color.g = 186.0f;
    line_color.b = 96.0f;
    line_color.a = 255.0f;
    processGPS(&destVec);
}

bool CTheScripts_IsPlayerOnAMission();

void GPSEventHandler(bool b) {
    int i;

    CHud_DrawGangOverlay(b);

    unsigned int playa = FindPlayerPed(0);

    if (!(playa
        && *(unsigned int *)(playa + 1484)
        && (*(unsigned char *)(playa + 0x4A5) & 1)
        && *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) != VEHICLE_PLANE
        && *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) != VEHICLE_HELI
        && *(unsigned int *)(*(unsigned int *)(playa + 1484) + 0x5D0) != VEHICLE_BMX)) {
            gpsShown = false;
            return;
    }

    CVector player_coords;
    FindPlayerCoors(&player_coords, 0);

    if ( FrontEndMenuManager_m_nTargetBlipIndex
    && CRadar_ms_RadarTrace[LOWORD(FrontEndMenuManager_m_nTargetBlipIndex)].m_nCounter == HIWORD(FrontEndMenuManager_m_nTargetBlipIndex)
    && CRadar_ms_RadarTrace[LOWORD(FrontEndMenuManager_m_nTargetBlipIndex)].m_nBlipDisplay)
    {
        CVector destPosn = CRadar_ms_RadarTrace[LOWORD(FrontEndMenuManager_m_nTargetBlipIndex)].m_vecPos;
        destPosn.z = CWorld_FindGroundZForCoord(destPosn.x, destPosn.y);

        line_color.r = gps_line_color_r;
        line_color.g = gps_line_color_g;
        line_color.b = gps_line_color_b;
        line_color.a = gps_line_color_a;

        processGPS(&destPosn);
        
    } else if (CTheScripts_IsPlayerOnAMission()) {
        tRadarTrace traces[174];   
        float trace_distances[174];

        for (i = 0; i < 174; i++) {
            tRadarTrace trace = CRadar_ms_RadarTrace[i];

            if (!trace.m_nRadarSprite && trace.m_nBlipDisplay > 1 && DistanceBetweenPoints(&player_coords, &trace.m_vecPos) > max_target_distance) {
                traces[i] = trace;

                if (trace.m_nColour == 8)
                    trace_distances[i] = FLT_MAX; // Prioritize destination markers.
                else
                    trace_distances[i] = DistanceBetweenPoints(&player_coords, &trace.m_vecPos);
            }
        }

        int max_distance_index = 0;
        float max_distance = -FLT_MAX;

        for (i = 0; i < 174; i++) {
            if (trace_distances[i] > max_distance) {
                max_distance = trace_distances[i];
                max_distance_index = i;
            }
        }

        if (max_distance_index >= 0) {
            renderMissionTrace(&traces[max_distance_index]);
        }
    }
}

void CSprite2d_SetMaskVertices(int, float *, float);

extern RwIm2DVertex CSprite2d_maVertices[];

#define FRAC_PI_2 1.57079637f

static RwRGBA font_color;
static RwRGBA font_shadow;

void drawGPSDistance() {
    DrawHud();

    if (gpsShown) {
        CFont_SetOrientation(ALIGN_RIGHT);
        CFont_SetColor(&font_color);
        CFont_SetBackground(0, 0);
        CFont_SetWrapx(70.0f);
        CFont_SetScale(0.3111112f, 0.8f);
        CFont_SetFontStyle(FONT_SUBTITLES);
        CFont_SetProportional(1);
        CFont_SetDropShadowPosition(0);
        CFont_SetDropColor(&font_shadow);
        CFont_SetEdge(0);
        CVector radarBottom, tmp;
        tmp.x = 0.9f;
        tmp.y = -0.45f;
        CRadar_TransformRadarPointToScreenSpace(&radarBottom, &tmp);
        char text[16];
        if (gpsDistance > 1000.0f)
            sprintf(text, "%.2fkm", gpsDistance / 1000.0f);
        else
            sprintf(text, "%dm", (int)gpsDistance);
        CFont_PrintString(radarBottom.x, radarBottom.y + 3.0f, text);
    }
}

void injectGPSPatches()
{  
    resultNodes = br_alloc_array(CNodeAddress, MAX_NODE_POINTS);
    nodePoints = br_alloc_array(CVector, MAX_NODE_POINTS);
    lineVerts = br_alloc_array(RwIm2DVertex, MAX_NODE_POINTS*4);

    limitRadarPoint = (float (*)(float *a1))ReadCall(0x238974);

    CRGBA_CRGBA(&font_color, distance_text_color_r, 
                             distance_text_color_g,
                             distance_text_color_b,
                             distance_text_color_a);

    CRGBA_CRGBA(&font_shadow, distance_text_shadow_r, 
                              distance_text_shadow_g,
                              distance_text_shadow_b,
                              distance_text_shadow_a);

    RedirectCall(0x26F2F4, &GPSEventHandler);
    RedirectCall(0x239090, &GPSEventHandler);

    RedirectCall(0x2437E8, &autoDisableTarget);
    
    DrawHud = (void (*)())ReadCall(0x2471E4);

    RedirectCall(0x2471E4, &drawGPSDistance);

}
