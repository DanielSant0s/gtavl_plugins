#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CVector.h>
#include <CFont.h>
#include <CVehicle.h>
#include <CTxdStore.h>
#include <CTexture.h>
#include <string.h>
#include <CPad.h>
#include <CCoronas.h>
#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <CCam.h>
#include <math.h>
#include <CRGBA.h>

RwRGBA drop_color, bg_color, selected_color, unsel_color;

CTexture pause_background, pause_map;

const char* option_panels[] = {
    "Input",
    "Gameplay",
    "Graphics",
    "Audio"
};

const char* menu_labels[] = {
    "Resume",
    "Map",
    "Game",
    "Gallery",
    "Briefs",
    "Stats",
    "Options"
};

#define menu_labels_size 7

static char selected_menu = 0;

static bool map_loaded = false;

void (*CSprite2d_Delete)(CTexture *this) = (void (*)(CTexture *this))0x2B0750;

void CMenuManager_UnloadMapTexture() { 
    CSprite2d_Delete(&pause_map);

    map_loaded = false;
}

void CMenuManager_LoadMapTexture() { 
    CTxdStore_PushCurrentTxd();
    int slot = CTxdStore_FindTxdSlot("fronten2");
        if ( slot == -1 )
            slot = CTxdStore_AddTxdSlot("fronten2");
        CTxdStore_LoadTxd(slot, "MODELS\\FRONTEN2.TXD");
        CTxdStore_AddRef(slot);
    CTxdStore_SetCurrentTxd(slot);

    CSprite2D_SetTexture(&pause_map, "map");

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(slot);
    CTxdStore_RemoveTxdSlot(slot);

    map_loaded = true;
}

void (*CMenuManager_PrintMap)(void *this) = (void (*)(void *this))0x2388A0;

void CMenuManager_DrawBackground(void *this) {
    CFont_SetFontStyle(3);
    CFont_SetDropColor(&drop_color);
    CFont_SetScale(0.7f, 1.3f);
    CFont_SetOrientation(1);

    CSprite2d_DrawSprite(&pause_background, 0.0f, 0.0f, 640.0f, 448.0f, &bg_color);

    //char cur_menu = ((char*)this)[376];

    int i;
    for (i = 0; i < menu_labels_size; i++) {
        if (selected_menu == i) {
            int string_width = (strlen(menu_labels[i])-1)*10.0f;

            CSprite2d_Draw2DPolygon(25.0f, 95.0f+(i*40.0f), 
                                    70.0f+string_width, 90.0f+(i*40.0f), 
                                    30.0f, 95.0f+((i+1)*40.0f), 
                                    65.0f+string_width, 100.0f+((i+1)*40.0f), 
                                    &unsel_color);

            CFont_SetColor(&selected_color);
            CFont_SetEdge(0);
        } else {
            CFont_SetColor(&unsel_color);
            CFont_SetEdge(2);
        }
        CFont_PrintString(40.0f, 100.0f+(i*40.0f), menu_labels[i]);
    }

    if((selected_menu == 0 || selected_menu == 1)) {
        if (!map_loaded) {
            CMenuManager_LoadMapTexture();
        }
    } else if (map_loaded) {
        CMenuManager_UnloadMapTexture();
    }

    switch (selected_menu) {
        case 0:
        case 1:
            CSprite2d_DrawSprite(&pause_map, 300.0f, 224.0f-175.0f, 350.0f*0.8f, 350.0f, &bg_color);
            break;
        default:
            break;
    }
}

void CMenuManager_SwapTexturesRound(void *this, int a2) { 
}

void CMenuManager_UserInput(void* this) {
    CPad* pad = CPad_GetPad(0);

    if (pad->NewState.DPadDown && !pad->OldState.DPadDown) {
        selected_menu++;
        if (selected_menu >= menu_labels_size) {
            selected_menu = 0;
        }
    } else if (pad->NewState.DPadUp && !pad->OldState.DPadUp) {
        selected_menu--;
        if (selected_menu < 0) {
            selected_menu = menu_labels_size-1;
        }
    }

    if (pad->NewState.ButtonCross && !pad->OldState.ButtonCross) {
        switch (selected_menu) {
            case 0:
                ((char*)this)[54] = 0;
                break;
            case 1:
                CMenuManager_PrintMap(this);
                break;
            default:
                break;
        }
    }

}

void CMenuManager_LoadAllTextures(void *this) { 
    CTxdStore_PushCurrentTxd();
    int slot = CTxdStore_FindTxdSlot("fronten3");
        if ( slot == -1 )
            slot = CTxdStore_AddTxdSlot("fronten3");
        CTxdStore_LoadTxd(slot, "MODELS\\FRONTEN3.TXD");
        CTxdStore_AddRef(slot);
    CTxdStore_SetCurrentTxd(slot);

    CSprite2D_SetTexture(&pause_background, "pause_bg");

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(slot);
    CTxdStore_RemoveTxdSlot(slot);

    CMenuManager_LoadMapTexture();
}

void CMenuManager_UnloadAllTextures(void *this) { 
    CSprite2d_Delete(&pause_background);
}

int _start() {  
    CRGBA_CRGBA(&bg_color, 255, 255, 255, 255);
    CRGBA_CRGBA(&unsel_color, 255, 255, 255, 255);
    CRGBA_CRGBA(&selected_color, 0, 0, 0, 255);
    CRGBA_CRGBA(&drop_color, 99, 135, 91, 255);

    RedirectCall(0x233EB0, CMenuManager_DrawBackground);
    RedirectFunction(0x233A20, CMenuManager_SwapTexturesRound);

    RedirectCall(0x23B170, CMenuManager_LoadAllTextures);
    RedirectCall(0x23B23C, CMenuManager_LoadAllTextures);

    RedirectCall(0x23B05C, CMenuManager_UnloadAllTextures);
    RedirectCall(0x24302C, CMenuManager_UnloadAllTextures);

    RedirectCall(0x23A01C, CMenuManager_UserInput);

    return 0;
}
