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

void setupLoadingText() {
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

void (*CLoadingScreen_LoadSplashes)();

void load_loading_img() {
    CLoadingScreen_LoadSplashes();

    CTxdStore_PushCurrentTxd();
    int v7 = CTxdStore_FindTxdSlot("loadscs");
        if ( v7 == -1 )
            v7 = CTxdStore_AddTxdSlot("loadscs");
        CTxdStore_LoadTxd(v7, "MODELS\\TXD\\LOADSCS.TXD");
        CTxdStore_AddRef(v7);
    CTxdStore_SetCurrentTxd(v7);

    CSprite2D_SetTexture(&loading_icon, "loading");

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(v7);
    CTxdStore_RemoveTxdSlot(v7);
}

float (*RsTimer)(int) = (float (*)(int))0x5931F0;
float* fPercentage = (float*)0x66CACC;
uint8_t* CLoadingScreen_m_bReadyToDelete = (uint8_t*)0x66CADC;
uint8_t* CLoadingScreen_m_bLegalScreen = (uint8_t*)0x66CAB0;
int* CLoadingScreen_m_currDisplayedSplash = (int*)0x668C90;
uint8_t* CLoadingScreen_m_bFadeOutCurrSplashToBlack = (uint8_t*)0x66CAB8;
uint8_t* CLoadingScreen_m_bFading = (uint8_t*)0x66CAAC;
uint8_t* CLoadingScreen_m_FadeAlpha = (uint8_t*)0x66CABC;
uint8_t* CLoadingScreen_m_bFadeInNextSplashFromBlack = (uint8_t*)0x66CAB4;
void (*CLoadingScreen_StartFading)() = (void (*)())0x5931C0;
float* flt_66CAC0 = (float*)0x66CAC0;
float* flt_66CAD0 = (float*)0x66CAD0;
float* flt_66CAD4 = (float*)0x66CAD4;
uint8_t* byte_66CAD8 = (uint8_t*)0x66CAD8;

void (*CLoadingScreen_DisplayNextSplash)() = (void(*)(void))0x592D10;

uint32_t* AudioEngine = (uint32_t*)0x889C80;

void (*CAudioEngine_ServiceLoadingTune)(uint32_t*, float) = (void(*)(uint32_t*, float))0x580920;

void CLoadingScreen_Update()
{
    float v5;
    float v7;
    float v8;

    if ( *flt_66CAD4 > 0.0f )
    {
        *fPercentage = (float)(100.0f * (float)(RsTimer(0) - *flt_66CAD4)) / (float)(21.6f - (float)(*flt_66CAD4 - *flt_66CAD0));
        if ( *fPercentage > 100.0f )
            *fPercentage = 100.0f;
    }

    if(!*CLoadingScreen_m_bReadyToDelete) {
        if ( *fPercentage > (float)(*CLoadingScreen_m_currDisplayedSplash) * 16.666666f ) {
            CLoadingScreen_DisplayNextSplash();
        }

        if ( *byte_66CAD8 ) {
            if ( !*CLoadingScreen_m_bFading && *fPercentage >= 100.0f )
            {
                *CLoadingScreen_m_bReadyToDelete = 1;
                *CLoadingScreen_m_bFadeOutCurrSplashToBlack = 1;
                CLoadingScreen_StartFading();
            }
        }
    }

    if ( *CLoadingScreen_m_bFading )
    {
        v5 = 2.0f;
        v7 = RsTimer(0) - *flt_66CAC0;
        if ( *CLoadingScreen_m_currDisplayedSplash == 0 && *CLoadingScreen_m_bFadeInNextSplashFromBlack )
            v5 = 0.60000002f;
        if ( v7 <= v5 )
        {
            v8 = 255.0f * (float)(v7 / v5);
            *CLoadingScreen_m_FadeAlpha = (uint8_t)v8;
        }
        else
        {
            *CLoadingScreen_m_FadeAlpha = 255;
            *CLoadingScreen_m_bFading = 0;
            
            *CLoadingScreen_m_bFadeInNextSplashFromBlack = 0;
            *CLoadingScreen_m_bFadeOutCurrSplashToBlack = 0;
        }
    }
    else
    {
        *CLoadingScreen_m_FadeAlpha = 255;
    }

    if(*AudioEngine) {
        if ( *CLoadingScreen_m_bFadeInNextSplashFromBlack )
        {
            CAudioEngine_ServiceLoadingTune(&AudioEngine, *CLoadingScreen_m_FadeAlpha * 0.0039215689f);
        }
        else if ( *CLoadingScreen_m_bFadeOutCurrSplashToBlack )
        {
            CAudioEngine_ServiceLoadingTune(&AudioEngine, (255.0f - *CLoadingScreen_m_FadeAlpha) * 0.0039215689f);
        }
        else
        {
            CAudioEngine_ServiceLoadingTune(&AudioEngine, 1.0f);
        }
    }

}

void drawImageRotate(float x, float y, float width, float height, float angle){
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

void (*CLoadingScreen_Shutdown)(int a1) = (void (*)(int a1))0x5925D0;

void deleteLoadingAnim(int a1) {
    CLoadingScreen_Shutdown(a1);
    CSprite2d_Delete(&loading_icon);
}

void renderLoadingAnim() { 
    if (*fPercentage < 100.0f) {
        ((void(*)(void))0x2A58E0)(); // InitPerFrame

        ((void(*)(void))0x2A8620)(); // Render Font Buffer

        setupLoadingText();
        CFont_PrintString(606, 420, "Entering Story Mode");

        ((void(*)(void))0x2A8620)(); // Render Font Buffer

        drawImageRotate(620, 432, 24, 24, icon_angle);
        icon_angle += 0.1f;
    }
}

void CLoadingScreen_RenderLoadingTextAnim()
{
    renderLoadingAnim();
    if ( 0.0f == *flt_66CAD4 )
    {
        *flt_66CAD4 = RsTimer(0);
    }
}

void CLoadingScreen_DebugPrint(const char* str1, const char* str2)
{
    printf("%s: %s\n", str1, str2);
}


void setupLoadingScreenPatches() {
    WriteDword(0x592598, 0); // Skip splash screen mode

    CLoadingScreen_LoadSplashes = (void (*)())ReadCall(0x592548);

    CRGBA_CRGBA(&font_color, 255, 255, 255, 255);

    RedirectCall(0x592548, load_loading_img);
    RedirectCall(0x5932FC, CLoadingScreen_RenderLoadingTextAnim);

    RedirectCall(0x5932EC, CLoadingScreen_Update);

    RedirectCall(0x2465EC, CLoadingScreen_DebugPrint);
    RedirectCall(0x24665C, CLoadingScreen_DebugPrint);

    RedirectCall(0x242AC0, deleteLoadingAnim);
    RedirectCall(0x2466CC, deleteLoadingAnim);
}
