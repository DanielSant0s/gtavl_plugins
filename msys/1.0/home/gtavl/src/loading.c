#include <CPad.h>
#include <stdio.h>
#include <CFont.h>
#include <injector.h>
#include <CRGBA.h>
#include <CVector.h>
#include <common.h>
#include <CTexture.h>
#include <CTxdStore.h>
#include <math.h>
#include <string.h>
#include "hooks.h"

static RwRGBA font_color;
static RwRGBA font_shadow;

static void setupLoadingText() {
    CFont_SetOrientation(ALIGN_RIGHT);
    CFont_SetColor(&font_color);
    CFont_SetBackground(0, 0);
    CFont_SetWrapx(640.0f);
    CFont_SetScale(0.42f, 1.0f);
    CFont_SetFontStyle(FONT_SUBTITLES);
    CFont_SetProportional(1);
    CFont_SetDropShadowPosition(0);
    CFont_SetDropColor(&font_shadow);
    CFont_SetEdge(0);
}

static CTexture loading_icon;

static void (*CLoadingScreen_LoadSplashes)();

extern uint32_t AudioEngine;
void CAudioEngine_ServiceLoadingTune(uint32_t*, float);

void (*CAudioEngine_PreloadBeatTrack)(uint32_t*, short) = (void (*)(uint32_t*, short))0x581310;
void (*CAudioEngine_PlayPreloadedBeatTrack)(uint32_t*, bool) = (void (*)(uint32_t*, bool))0x5814A0;
void (*CAudioEngine_StopBeatTrack)(uint32_t*) = (void (*)(uint32_t*))0x5814E0;
void (*CAudioEngine_Service)(uint32_t*) = (void (*)(uint32_t*))0x580730;

bool started_playing_track = false;

static void load_loading_img() {
    CLoadingScreen_LoadSplashes();

    CTxdStore_PushCurrentTxd();
    int loadscs_slot = CTxdStore_FindTxdSlot("loadscs");
    if ( loadscs_slot == -1 )
        loadscs_slot = CTxdStore_AddTxdSlot("loadscs");
    CTxdStore_LoadTxd(loadscs_slot, "MODELS\\TXD\\LOADSCS.TXD");
    CTxdStore_AddRef(loadscs_slot);
    CTxdStore_SetCurrentTxd(loadscs_slot);

    CSprite2d_SetTexture(&loading_icon, "loading", 0);

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(loadscs_slot);
    CTxdStore_RemoveTxdSlot(loadscs_slot);
}

void CLoadingScreen_Shutdown(int a1);

static void deleteLoadingAnim(int a1) {
    CLoadingScreen_Shutdown(a1);
    CSprite2d_Delete(&loading_icon);
}

extern float fPercentage;
extern uint8_t CLoadingScreen_m_bReadyToDelete;
extern uint8_t CLoadingScreen_m_bLegalScreen;
extern int CLoadingScreen_m_currDisplayedSplash;
extern uint8_t CLoadingScreen_m_bFadeOutCurrSplashToBlack;
extern uint8_t CLoadingScreen_m_bFading;
extern uint8_t CLoadingScreen_m_FadeAlpha;
extern uint8_t CLoadingScreen_m_bFadeInNextSplashFromBlack;

void CLoadingScreen_StartFading();

extern float flt_66CAC0;
extern float flt_66CAD0;
extern float flt_66CAD4;

extern uint8_t byte_66CAD8;

void CLoadingScreen_DisplayNextSplash();

static void CLoadingScreen_Update()
{
    float v5;
    float loadscs_slot;
    float v8;

    if ( flt_66CAD4 > 0.0f )
    {
        fPercentage = (float)(100.0f * (float)(RsTimer(0) - flt_66CAD4)) / (float)(21.6f - (float)(flt_66CAD4 - flt_66CAD0));
        if ( fPercentage > 100.0f )
            fPercentage = 100.0f;
    }

    if(!CLoadingScreen_m_bReadyToDelete) {
        if ( fPercentage > (float)(CLoadingScreen_m_currDisplayedSplash) * 16.666666f ) {
            CLoadingScreen_DisplayNextSplash();
        }

        if ( byte_66CAD8 ) {
            if ( !CLoadingScreen_m_bFading && fPercentage >= 100.0f )
            {
                CLoadingScreen_m_bReadyToDelete = 1;
                CLoadingScreen_m_bFadeOutCurrSplashToBlack = 1;
                CLoadingScreen_StartFading();
            }
        }
    }

    if ( CLoadingScreen_m_bFading )
    {
        v5 = 2.0f;
        loadscs_slot = RsTimer(0) - flt_66CAC0;
        if ( CLoadingScreen_m_currDisplayedSplash == 0 && CLoadingScreen_m_bFadeInNextSplashFromBlack )
            v5 = 0.60000002f;
        if ( loadscs_slot <= v5 )
        {
            v8 = 255.0f * (float)(loadscs_slot / v5);
            CLoadingScreen_m_FadeAlpha = (uint8_t)v8;
        }
        else
        {
            CLoadingScreen_m_FadeAlpha = 255;
            CLoadingScreen_m_bFading = 0;
            
            CLoadingScreen_m_bFadeInNextSplashFromBlack = 0;
            CLoadingScreen_m_bFadeOutCurrSplashToBlack = 0;
        }
    }
    else
    {
        CLoadingScreen_m_FadeAlpha = 255;
    }

    if(AudioEngine) {
        if ( CLoadingScreen_m_bFadeInNextSplashFromBlack )
        {
            CAudioEngine_ServiceLoadingTune(AudioEngine, CLoadingScreen_m_FadeAlpha * 0.0039215689f);
        }
        else if ( CLoadingScreen_m_bFadeOutCurrSplashToBlack )
        {
            CAudioEngine_ServiceLoadingTune(AudioEngine, (255.0f - CLoadingScreen_m_FadeAlpha) * 0.0039215689f);
        }
        else
        {
            CAudioEngine_ServiceLoadingTune(AudioEngine, 1.0f);
        }
    }

}

static void drawImageRotate(float x, float y, float width, float height, float angle){
	float c = cosf(angle);
	float s = sinf(angle);

    CSprite2d_Draw(&loading_icon, 
                   (-width/2)*c - (-height/2)*s+x, (-height/2)*c + (-width/2)*s+y,
                   (-width/2)*c - height/2*s+x, height/2*c + (-width/2)*s+y, 
                   width/2*c - (-height/2)*s+x, (-height/2)*c + width/2*s+y, 
                   width/2*c - height/2*s+x, height/2*c + width/2*s+y,
                   &font_color);

}

static float icon_angle = 0.0f;

static bool debug_loading = false;
char* debugmsg = NULL;

static void renderLoadingAnim() { 
    CPad_UpdatePads();
    CPad* pad = CPad_GetPad(0);

    if(pad->NewState.DPadRight && !pad->OldState.DPadRight) {
        debug_loading ^= 1;
    }

    if (fPercentage < 100.0f) {
        ((void(*)(void))0x2A58E0)(); // InitPerFrame

        ((void(*)(void))0x2A8620)(); // Render Font Buffer

        setupLoadingText();
        CFont_PrintString(606, 420, (debug_loading && debugmsg)? debugmsg : "Entering Story Mode");

        ((void(*)(void))0x2A8620)(); // Render Font Buffer

        drawImageRotate(620, 432, 24, 24, icon_angle);
        icon_angle += 0.1f;
    }
}

static void CLoadingScreen_RenderLoadingTextAnim()
{
    renderLoadingAnim();
    if ( 0.0f == flt_66CAD4 )
    {
        flt_66CAD4 = RsTimer(0);
    }
}

static void CLoadingScreen_DebugPrint(const char* str1, const char* str2)
{
    debugmsg = str2;
    printf("%s: %s\n", str1, str2);
}


void setupLoadingScreenPatches() {
    WriteDword(0x592598, 0); // Skip splash screen mode

    CLoadingScreen_LoadSplashes = (void (*)())ReadCall(0x592548);

    CRGBA_CRGBA(&font_color, 255, 255, 255, 255);

    RedirectCall(0x592548, &load_loading_img);
    RedirectCall(0x5932FC, &CLoadingScreen_RenderLoadingTextAnim);

    RedirectCall(0x5932EC, &CLoadingScreen_Update);

    RedirectCall(0x2465EC, &CLoadingScreen_DebugPrint);
    RedirectCall(0x24665C, &CLoadingScreen_DebugPrint);

    RedirectCall(0x242AC0, &deleteLoadingAnim);
    RedirectCall(0x2466CC, &deleteLoadingAnim);
}
