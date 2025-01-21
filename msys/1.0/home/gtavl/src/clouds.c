#include <stdint.h>
#include <injector.h>
#include <CCam.h>
#include <math.h>
#include <common.h>
#include "hooks.h"

#include "bios_alloc.h"

static void* RwIm3DTransformUnsetTexture(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags) {
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	return RwIm3DTransform(pVerts, numVerts, ltm, flags);
}


void CSprite_RenderBufferedOneXLUSprite(CVector *pos, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, int recipNearZ, uint8_t a11);


static void CSprite_RenderBufferedOneXLUSpriteAspect(CVector *pos, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity, int recipNearZ, uint8_t a11) {
    CSprite_RenderBufferedOneXLUSprite(pos, w*0.75f, h, r, g, b, intensity, recipNearZ, a11);
}

void CSprite_RenderOneXLUSprite(float x, float y, float z, float halfWidth, float halfHeight, 
								 unsigned char red, unsigned char green, unsigned char blue, 
								 short alpha, float rhw, unsigned char intensity, unsigned char udir, 
								 unsigned char vdir);

static void CSprite_RenderOneXLUSpriteAspect(float x, float y, float z, float halfWidth, float halfHeight, 
								 unsigned char red, unsigned char green, unsigned char blue, 
								 short alpha, float rhw, unsigned char intensity, unsigned char udir, 
								 unsigned char vdir) 
{
    CSprite_RenderOneXLUSprite(x, y, z, halfWidth*0.65f, halfHeight, red, green, blue, alpha, rhw, intensity, udir, vdir);
}

#define AMOUNT_OF_SIDESTARS 100

enum eStarSides
{
    SSide_Left = 0,
    SSide_Right,
    SSide_Front,
    SSide_Back,
    SSide_Up,

    SSidesCount
};

float *StarCoorsX = NULL; 
float *StarCoorsY = NULL;

float *StarSizes = NULL;

static const float fSmallStars = 0.5f, fMiddleStars = 1.5f;
static uint8_t nStarsHourStart = 22, nStarsHourLast = 5;

static const CVector PositionsTable[SSidesCount] =
{
    { 100.0f,  0.0f,   10.0f}, // Left
    {-100.0f,  0.0f,   10.0f}, // Right
    {   0.0f,  100.0f, 10.0f}, // Front
    {   0.0f, -100.0f, 10.0f}, // Back
    {   0.0f,  0.0f,   95.0f}, // Up
};

extern int CClock_ms_nGameClockHours; // 0x66B7AC
extern int CClock_ms_nGameClockMinutes; // 0x66B7B0

extern float CWeather_CloudCoverage; // 0x66BD40
extern float CWeather_Foggyness; // 0x66BD44

bool CSprite_CalcScreenCoors(CVector* posn, CVector* out, float* w, float* h, bool checkMaxVisible, bool checkMinVisible);

extern float CDraw_ms_fFOV;

static void DrawSkyStars()
{
    float v7 = 640.0f * (float)(70.0f / CDraw_ms_fFOV);
    float v8 = 448.0f * (float)(70.0f / CDraw_ms_fFOV);

    CVector WorldPos, WorldStarPos;

    CVector *CamPos;

    CamPos = &TheCamera.placeable.m_matrix->pos;
    if ( !TheCamera.placeable.m_matrix ) {
        CamPos = &TheCamera.placeable.placement.m_vPosn;
    }

    float intensity = 255.0f - (255.0f * (CWeather_CloudCoverage > CWeather_Foggyness? CWeather_CloudCoverage : CWeather_Foggyness));
    if (intensity == 0.0f) return;

    if (CClock_ms_nGameClockHours == nStarsHourStart) intensity *= 0.01666f * CClock_ms_nGameClockMinutes;
    else if(CClock_ms_nGameClockHours == nStarsHourLast) intensity *= 0.01666f * (60 - CClock_ms_nGameClockMinutes);

    int side;
    for (side = 0; side < SSidesCount; ++side)
    {
        WorldPos.x = PositionsTable[side].x + CamPos->x;
        WorldPos.y = PositionsTable[side].y + CamPos->y;
        WorldPos.z = PositionsTable[side].z + CamPos->z;

        int i;
        for (i = 0; i < AMOUNT_OF_SIDESTARS; ++i)
        {
            WorldStarPos.x = WorldPos.x;
            WorldStarPos.y = WorldPos.y;
            WorldStarPos.z = WorldPos.z;

            switch (side)
            {
            case SSide_Left:
            case SSide_Right:
                WorldStarPos.y -= StarCoorsX[(AMOUNT_OF_SIDESTARS*side)+i];
                WorldStarPos.z += StarCoorsY[(AMOUNT_OF_SIDESTARS*side)+i];
                break;

            case SSide_Front:
            case SSide_Back:
                WorldStarPos.x -= StarCoorsX[(AMOUNT_OF_SIDESTARS*side)+i];
                WorldStarPos.z += StarCoorsY[(AMOUNT_OF_SIDESTARS*side)+i];
                break;

            default:
                WorldStarPos.x += StarCoorsX[(AMOUNT_OF_SIDESTARS*side)+i];
                WorldStarPos.y += StarCoorsY[(AMOUNT_OF_SIDESTARS*side)+i];
                break;
            }

            float size = StarSizes[(AMOUNT_OF_SIDESTARS*side)+i];
            uint8_t brightness = (uint8_t)((1.0f - 0.015f * (rand() % 32)) * intensity);
            
            CSprite_RenderBufferedOneXLUSpriteAspect(&WorldStarPos, size * v7, size * v8, brightness, brightness, brightness, 255, 255, 0);
        }
    }
}

void setupCloudsPatches() {
    StarCoorsX = br_alloc_array(float, SSidesCount*AMOUNT_OF_SIDESTARS);
    StarCoorsY = br_alloc_array(float, SSidesCount*AMOUNT_OF_SIDESTARS);
    StarSizes =  br_alloc_array(float, SSidesCount*AMOUNT_OF_SIDESTARS);

    RedirectCall(0x2ED840, &RwIm3DTransformUnsetTexture);

    RedirectCall(0x2ECDA0, &CSprite_RenderBufferedOneXLUSpriteAspect);
    RedirectCall(0x2ECEA8, &CSprite_RenderBufferedOneXLUSpriteAspect);
    RedirectCall(0x2ED49C, &CSprite_RenderBufferedOneXLUSpriteAspect);

    RedirectCall(0x2EC6F4, &CSprite_RenderOneXLUSpriteAspect);
    RedirectCall(0x2EC920, &CSprite_RenderOneXLUSpriteAspect);
    RedirectCall(0x2ECAA4, &CSprite_RenderOneXLUSpriteAspect);

    int seed = 0xDEADBEEF;

    unsigned int resetSeed = rand() / 2;

    // Keeps stars always the same
    srand(++seed);

    int side;
    for (side = 0; side < SSidesCount; ++side)
    {
        int i;
        for (i = 0; i < AMOUNT_OF_SIDESTARS; ++i)
        {
            StarCoorsX[(AMOUNT_OF_SIDESTARS*side)+i] = 90.0f * random_float(-1.0f, 1.0f);

            // Side = SSide_Up is when rendering stars directly ABOVE us
            if (side == SSide_Up) {
                StarCoorsY[(AMOUNT_OF_SIDESTARS*side)+i] = 80.0f * random_float(-1.0f, 1.0f);
            } else {
                StarCoorsY[(AMOUNT_OF_SIDESTARS*side)+i] = 80.0f * random_float(-0.35f, 1.0f);
            }

            StarSizes[(AMOUNT_OF_SIDESTARS*side)+i] = 0.8f * random_float(fSmallStars, fMiddleStars);
        }
    }

    srand((RsTimer(0) / 2) + resetSeed); // reset

    MakeBranch(0x2ECC04, 0xAA);
    RedirectCall(0x2ECBFC, &DrawSkyStars);
    
}
