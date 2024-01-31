#include <stdio.h>
#include <stdint.h>
#include <injector.h>
#include <common.h>
#include <string.h>
#include <CFont.h>
#include <CRGBA.h>
#include <rwcore.h>
#include <CTxdStore.h>
#include <CTexture.h>
#include <CVector.h>
#include <stdlib.h>
#include <CCam.h>

#include "colour_filter.h"
#include "hooks.h"

static void loadMenuTextures() {
    RwTexture **v9;
    char **v10;
    uint32_t _this;

    asm("move %0, $s1" : "=r" (_this)); // get from our parent function

    v10 = (char**)0x5F9D98;
    v9 = (RwTexture **)(_this + 4 * 13 + 276); //&this->textures[13].texture;
    do
    {
        if (!strcmp("map", v10[0])) {
            CSprite2d_SetTexture((CTexture *)v9, v10[0], v10[1]);
            CSprite2d_SetAddressing((CTexture *)v9, 3);
        }

        v10 += 2;
        ++v9;
    }
    while ( (int)v10 < (int)0x5F9DD8 );
    CTxdStore_PopCurrentTxd();
}

static void MakeSpaceForFrontend2() {
    CStreaming_MakeSpaceFor(33256); // Increase menu texture temporary buffer
}

int bShouldRenderTransparentMenu = false;
RwRGBA MenuBGAddColor = {255, 255, 255, 255}; 
RwRGBA MenuBGReplaceColor = {255, 255, 255, 255}; 

static void InitTransparentMenu() {
    uint32_t s0;
    asm("move %0, $s0" : "=r" (s0)); // get from our parent function

    bShouldRenderTransparentMenu = true;

    BlurFilterAddColor = MenuBGAddColor;
    BlurFilterReplaceColor = MenuBGReplaceColor;

    enable_blur = true;
    bCustomBlurColor = true;

    asm volatile ("lb $a0, 0x171(%0)\n"
                  "li $v1, 0x13 \n" 
                  : 
                  : "r"(s0));
}

static bool ShouldRenderTransparentMenu() {
    return 0;
}

static RwRGBA quad_color = { 0, 0, 0, 166 };

static void ProcessTransparentMenu() {
    //CSprite2d_Draw2DPolygon(20.0f, 73.0f, 
    //                        20.0f, 396.0f, 
    //                        621.0f, 73.0f, 
    //                        621.0f, 396.0f, &quad_color);
}

void Render2dStuff();
void Render2dStuffAfterFade();
void CPlantMgr_Render(void);

static void TransparentMenuCPlantMgr_Render() {
    if (!bShouldRenderTransparentMenu) {
        CPlantMgr_Render();
    }
}

static void TransparentMenuRender2dStuff() {
    if (!bShouldRenderTransparentMenu) {
        Render2dStuff();
    }
}

void (*CPed_PreRenderAfterTestTransparent)(uint32_t);

static void TransparentMenuCPed_PreRender(uint32_t this) {
    if (!bShouldRenderTransparentMenu) {
        CPed_PreRenderAfterTestTransparent(this);
    }
}

static void FinishTransparentMenu() {
    bShouldRenderTransparentMenu = false;

    BlurFilterReplaceColor.r = 255;
    BlurFilterReplaceColor.g = 255;
    BlurFilterReplaceColor.b = 255;
    BlurFilterReplaceColor.a = 255;

    BlurFilterAddColor.r = 255;
    BlurFilterAddColor.g = 255;
    BlurFilterAddColor.b = 255;
    BlurFilterAddColor.a = 255;

    bCustomBlurColor = false;
    enable_blur = false;
}

void setupPauseMenuPatches()
{
    RedirectCall(0x23387C, &MakeSpaceForFrontend2);
    RedirectCall(0x233A90, &MakeSpaceForFrontend2);

    RedirectCall(0x23AE54, &InitTransparentMenu);
    MakeNop(0x23AE58);

    RedirectCall(0x234348, &ProcessTransparentMenu);
    MakeNop(0x234380);

    RedirectCall(0x23B04C, &FinishTransparentMenu);

    RedirectCall(0x24675C, &ShouldRenderTransparentMenu);
    MakeNop(0x246760);

    RedirectCall(0x2469E4, &TransparentMenuRender2dStuff);
    RedirectCall(0x246DC8, &TransparentMenuCPlantMgr_Render);
    CPed_PreRenderAfterTestTransparent = (void (*)(uint32_t))ReadCall(0x19A728);
    RedirectCall(0x19A728, &TransparentMenuCPed_PreRender);

    int i;
    for (i = 0; i < 72; i+=4) {
        MakeNop(0x2338E4 + i);
        MakeNop(0x233AF8 + i);
    }

    RedirectCall(0x2338E4, loadMenuTextures);
    RedirectCall(0x233AF8, loadMenuTextures);

    WriteByte(0x23B124, 0);
    WriteByte(0x23B14C, 0);

    MakeNop(0x233DB0);
    MakeNop(0x233DB8);
    MakeNop(0x233DC0);
    MakeNop(0x233DC8);

    
    MakeNop(0x239174); // Remove map non discovered overlay

    MakeNop(0x238E9C); // Remove map gray background
    MakeNop(0x238F3C); // Remove map black background
    MakeNop(0x238FDC); // Remove map blue background

    /*MakeNop(0x239214);
    MakeNop(0x239258);
    MakeNop(0x239298);
    MakeNop(0x2392DC);

    MakeNop(0x239344);
    MakeNop(0x2393AC);
    MakeNop(0x239418);
    MakeNop(0x239480);*/
}
