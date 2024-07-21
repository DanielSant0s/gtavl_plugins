
#include <injector.h>
#include <CVehicle.h>
#include <CPad.h>
#include <CFont.h>
#include <CRadar.h>
#include <CPool.h>
#include <CRGBA.h>
#include <string.h>
#include <common.h>
#include <stdio.h>
#include <math.h>
#include <bit.h>

#include "hooks.h"

extern uint16_t CHud_m_ItemToFlash;
extern uint8_t MaxHealth;
extern uint32_t CWorld_PlayerInFocus[];

float CStats_GetFatAndMuscleModifier(uint32_t);
void CSprite2d_DrawBarChart(float, float, uint16_t, uint8_t, float, char, uint8_t, uint8_t, RwRGBA*, RwRGBA*);

static void CHud_PrintArmourForPlayer(int playerID, float posX, float posY)
{
    float fPercentage;
    RwRGBA v8; 
    RwRGBA color; 

    if ( CHud_m_ItemToFlash != 3 )
    {
        CRGBA_CRGBA(&v8, 0, 0, 0, 255);
        CRGBA_CRGBA(&color, 22, 76, 90, 255);
        fPercentage = *(float *)(CWorld_PlayerInFocus[111 * playerID] + 1416);

        CSprite2d_DrawBarChart(84.0f, 420.0f, 27.5f, 9.0f, fPercentage, 0, 0, 1, &color, &v8);
    }
}

float CHud_AbilityPercent = 100.0f;

static void CHud_PrintBreathOrAbilityForPlayer(int playerID, float posX, float posY)
{
    float fPercentage;
    RwRGBA v8; 
    RwRGBA color; 
    int breath_activate;
    uint32_t ped_player;

    if ( CHud_m_ItemToFlash != 10 )
    {
        CRGBA_CRGBA(&v8, 0, 0, 0, 255);

        asm volatile("move %0, $t6" : "=r" (breath_activate));

        if (breath_activate) {
            CRGBA_CRGBA(&color, 131, 190, 217, 255);
            ped_player = *(uint32_t*)(CWorld_PlayerInFocus[111 * playerID] + 1208);
            fPercentage = *(float *)(ped_player + 68) / 11.52f;
        } else {
            CRGBA_CRGBA(&color, 244, 203, 111, 255);
            fPercentage = CHud_AbilityPercent;
        }


        CSprite2d_DrawBarChart(108.0f, 420.0f, 27.5f, 9.0f, fPercentage, 0, 0, 1, &color, &v8);
    }
}

static void CHud_PrintHealthForPlayer(int playerID, float posX, float posY)
{
    float fPercentagea;
    RwRGBA v10;
    RwRGBA color;

    if (CHud_m_ItemToFlash != 4)
    {
        fPercentagea = (*(float*)(CWorld_PlayerInFocus[111 * playerID] + 1408));
        CRGBA_CRGBA(&v10, 0, 0, 0, 255);

        if (*(float*)(CWorld_PlayerInFocus[111 * playerID] + 1408) >= 30.0f) {
            CRGBA_CRGBA(&color, 0x3e, 0x9b, 0x01, 255);
        } else {
            if ((CTimer_m_FrameCounter & 8) != 0) {
                CRGBA_CRGBA(&color, 255, 0, 0, 255);
            } else {
                CRGBA_CRGBA(&color, 128, 0, 0, 255);
            }
            
        }

        CSprite2d_DrawBarChart(38.7f, 420.0f, 47.0f, 9.0f, fPercentagea, 0, 0, 1, &color, &v10);
    }
}

extern float CRadar_m_radarRange;

static uint32_t show_msec = 0;
static uint32_t add_msec = 0;

static bool money_visible = false;
static bool score_set_visible = false;
static bool shop_active = false;

#define PlayerInfoSize 111

extern uint32_t DisplayedScore;
extern uint32_t Score[];
extern uint8_t PlayerInFocus;

static char money_diff_str[32];
static char money_str[32];

static uint32_t curr_money = 0;
static uint32_t last_money = 0;

static RwRGBA add_color, sub_color, cur_color, shadow_color, star_slot, star_slot_shadow, gray_color;

#define ABS(a)  (((a) < 0) ? (-(a)) : (a))
#define isNearlyEqualF(a, b, t) (fabsf(a - b) <= t)
#define interpF(a, b, f) a + (f) * (b - a)
#define norm(value, min, max) (((value) < (min)) ? 0 : (((value) > (max)) ? 1 : (((value) - (min)) / ((max) - (min)))))
#define lerp(no, mi, ma) ( (no) * ((ma) - (mi)) + (mi) )

#define clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static bool bShowAmmo;
static int nAmmoFadeAlpha;
static int nTimeToShowAmmoDifference;

bool CDarkel_FrenzyOnGoing();

static RwRGBA dropfade_color;

bool DrawAmmoEnableOverride = false;
int *DrawAmmoOverrideAmmoQuantity = NULL;
int *DrawAmmoOverrideClipQuantity = NULL;

void DrawAmmoSetOverride(int *ammo_qt, int *clip_qt) {
    DrawAmmoEnableOverride = true;
    DrawAmmoOverrideAmmoQuantity = ammo_qt;
    DrawAmmoOverrideClipQuantity = clip_qt;
}

void DrawAmmoClearOverride() {
    DrawAmmoEnableOverride = false;
}

static void CHudNew_DrawAmmo() {
    void* playa = FindPlayerPed(0);
    int slot = getPedActiveWeaponSlot(playa);
    CWeapon* weapons = getPedWeapons(playa);
    int weaponType = weapons[slot].m_nType;
    int totalAmmo = weapons[slot].m_nTotalAmmo;
    int ammoInClip = weapons[slot].m_nAmmoInClip;
    int maxAmmoInClip = CWeaponInfo_GetWeaponInfo(weapons[slot].m_nType, CPed_GetWeaponSkill(playa))->m_nAmmoClip;
    char str_ammo[16], str_clip[16];

    if (DrawAmmoEnableOverride) {
        sprintf(str_ammo, "%d", *DrawAmmoOverrideAmmoQuantity);
        if (DrawAmmoOverrideClipQuantity) {
            sprintf(str_clip, "%d", *DrawAmmoOverrideClipQuantity);
        } else {
            strcpy(str_clip, "");
        }
    } 
    else if (maxAmmoInClip <= 1 || maxAmmoInClip >= 1000) {
        sprintf(str_ammo, "%d", totalAmmo);
        strcpy(str_clip, "");
    }
    else {
        if (weaponType == WEAPON_FTHROWER) {
            unsigned int total = 9999;
            if ((totalAmmo - ammoInClip) / 10 <= 9999)
                total = (totalAmmo - ammoInClip) / 10;
            sprintf(str_ammo, "%d", total);
            sprintf(str_clip, "%d", ammoInClip / 10);
        }
        else {
            unsigned int total = totalAmmo - ammoInClip;
            if (totalAmmo - ammoInClip > 9999) {
                strcpy(str_ammo, "-");
                strcpy(str_clip, "-");
            }
            else {
                sprintf(str_ammo, "%d", total);
                sprintf(str_clip, "%d", ammoInClip);
            }
        }
    }

    if (bShowAmmo)
        nAmmoFadeAlpha = (int)interpF(nAmmoFadeAlpha, 255, 0.2f * CTimer_ms_fTimeStep);
    else
        nAmmoFadeAlpha = (int)interpF(nAmmoFadeAlpha, 0, 0.2f * CTimer_ms_fTimeStep);

    static int previousTotalAmmo;
    static int previousAmmoInClip;
    if (previousTotalAmmo != (totalAmmo - ammoInClip) || previousAmmoInClip != ammoInClip || DrawAmmoEnableOverride) {
        bShowAmmo = true;
        nTimeToShowAmmoDifference = CTimer_m_snTimeInMilliseconds + 8000;
    }

    CFont_SetBackground(0, 0);
    CFont_SetWrapx(640.0f);
    //CFont_SetClipX(SCREEN_WIDTH);
    CFont_SetFontStyle(3);
    CFont_SetOrientation(ALIGN_RIGHT);
    CFont_SetDropShadowPosition(0);
    CFont_SetEdge(1);
    CFont_SetScale(0.4f, 1.1f);

    CRGBA_CRGBA(&dropfade_color, 0, 0, 0, 255);

    CFont_SetDropColor(&dropfade_color);

    float heightLerp = 0.0f;

    if (FindPlayerWanted(-1)->m_nWantedLevel > 0) {
        heightLerp += 20.0f;
    }

    if (money_visible) {
        heightLerp += 20.0f;
    }

    if (score_set_visible) {
        heightLerp += 20.0f;
    }

    // heightLerp = interpF(heightLerp, GetShiftOffsetForAmmo(), 0.4f * CTimer_ms_fTimeStep);
    if (DrawAmmoEnableOverride) {
        CFont_SetColor(&cur_color);
        
        CFont_PrintString(612.0f-(str_clip[0]? (CFont_GetStringWidth(str_clip, 1, 1) + 9.0f) : 0.0f), 30.0f + heightLerp, str_ammo);

        CFont_SetColor(&gray_color);
        CFont_PrintString(612.0f, 30.0f + heightLerp, str_clip);
    }
    else if (CDarkel_FrenzyOnGoing()
        || weaponType == WEAPON_UNARMED
        || weaponType == WEAPON_DETONATOR
        || weaponType >= WEAPON_DILDO1 && weaponType < WEAPON_GRENADE
        || weaponType == WEAPON_PARACHUTE
        || CWeaponInfo_GetWeaponInfo(weaponType, 1)->m_nWeaponFire == 5
        || CWeaponInfo_GetWeaponInfo(weaponType, 1)->m_nSlot <= 1) {
        nTimeToShowAmmoDifference = 0;
        bShowAmmo = false;
    }
    else {
        CFont_SetColor(&cur_color);
        
        CFont_PrintString(612.0f-(str_clip[0]? (CFont_GetStringWidth(str_clip, 1, 1) + 9.0f) : 0.0f), 30.0f + heightLerp, str_ammo);

        CFont_SetColor(&gray_color);
        CFont_PrintString(612.0f, 30.0f + heightLerp, str_clip);
    }

    if (nTimeToShowAmmoDifference < CTimer_m_snTimeInMilliseconds)
        bShowAmmo = false;

    previousTotalAmmo = (totalAmmo - ammoInClip);
    previousAmmoInClip = ammoInClip;
}


inline void setupSetMoneyText(bool negative) {
    CFont_SetProportional(0);
    CFont_SetBackground(0, 0);
    CFont_SetScale(0.4f, 1.1f);
    CFont_SetOrientation(ALIGN_RIGHT);
    CFont_SetWrapx(600.0f);
    //CFont_SetRightJustifyWrap(v53, v54); 
    CFont_SetFontStyle(3);
    CFont_SetDropShadowPosition(0);
    CFont_SetEdge(1);
    CFont_SetDropColor(&shadow_color);
    if (negative) {
        CFont_SetColor(&sub_color);
        return;
    }
    CFont_SetColor(&add_color);
}

inline void setupCurrentMoneyText() {
    CFont_SetProportional(0);
    CFont_SetBackground(0, 0);
    CFont_SetScale(0.4f, 1.1f);
    CFont_SetOrientation(ALIGN_RIGHT);
    CFont_SetWrapx(600.0f);
    //CFont_SetRightJustifyWrap(v53, v54);
    CFont_SetFontStyle(3);
    CFont_SetDropShadowPosition(0);
    CFont_SetEdge(1);
    CFont_SetDropColor(&shadow_color);
    CFont_SetColor(&cur_color);
}

void updateMoney() {
    if (curr_money < 0) {
        sprintf(money_str, "-$%d", curr_money);
    } else {
        sprintf(money_str, "$%d", curr_money);
    }
}

void CShopping_LoadShop(const char* a1);
void CShopping_RemoveLoadedShop();

static void loadShopShowMoney(const char* a1) {
    shop_active = true;
    CShopping_LoadShop(a1);
}

static void unloadShopHideMoney() {
    shop_active = false;
    CShopping_RemoveLoadedShop();
}

static void drawPlayerMoney() {
    CPad* pad = CPad_GetPad(0);

    float money_y = 30.0f;
    float money_set_y = 50.0f;

    if (FindPlayerWanted(-1)->m_nWantedLevel > 0) {
        money_y += 20.0f;
        money_set_y += 20.0f;
    }

    last_money = curr_money;
    curr_money = Score[PlayerInFocus * PlayerInfoSize];

    if (last_money != curr_money) {
        add_msec = CTimer_m_snTimeInMilliseconds;
        show_msec = CTimer_m_snTimeInMilliseconds;
        score_set_visible = true;
        money_visible = true;

        if (last_money > curr_money) {
            sprintf(money_diff_str, "-$%d", last_money-curr_money);
        } else {
            sprintf(money_diff_str, "+$%d", curr_money-last_money);
        }

        updateMoney();
    }

    if ( pad->NewState.DPadDown && !pad->OldState.DPadDown ) {
        show_msec = CTimer_m_snTimeInMilliseconds;
        money_visible = true;
        updateMoney();
        
    }

    if ( money_visible || shop_active ) {
        setupCurrentMoneyText();
        CFont_PrintString(612.0f, money_y, money_str);
        if (((CTimer_m_snTimeInMilliseconds - show_msec) > 5000) && !shop_active) {
            money_visible = false;
        }
    }

    if (score_set_visible) {
        setupSetMoneyText(money_diff_str[0] == '-');
        CFont_PrintString(612.0f, money_set_y, money_diff_str);
        if ((CTimer_m_snTimeInMilliseconds - add_msec) > 5000) {
            score_set_visible = false;
        }
    }

}

static void DrawWantedStars() {
    if (FindPlayerWanted(-1)->m_nWantedLevel < 1)
        return;

    char star_buf[8];
    RwRGBA *c = &cur_color;

    float x = 612.0f;
    float y = 30.0f;

    CFont_SetProportional(0);
    CFont_SetBackground(0, 0);
    CFont_SetScale(0.4f, 1.1f);
    CFont_SetOrientation(ALIGN_RIGHT);
    CFont_SetWrapx(600.0f);
    //CFont_SetRightJustifyWrap(v53, v54);
    CFont_SetFontStyle(2);
    CFont_SetDropShadowPosition(0);
    CFont_SetEdge(1);

    CFont_SetDropColor(&star_slot_shadow);
    CFont_SetColor(&star_slot);
    CFont_PrintString(x, y, "]]]]]");

    memset(star_buf, 0, 8);

    int i;
    for (i = 0; i < FindPlayerWanted(-1)->m_nWantedLevel; i++) {
        star_buf[i] = ']';
    }

    if (FindPlayerWanted(-1)->m_nLastTimeWantedLevelChanged + 2000 > CTimer_m_snTimeInMilliseconds)
        c = (( (CTimer_m_snTimeInMilliseconds % 600) < 300 ) ? &cur_color : &gray_color);

    CFont_SetDropColor(&shadow_color);
    CFont_SetColor(c);
    CFont_PrintString(x, y, star_buf);
}


static void (*DrawHud)();

static float zoom;

uint32_t zoomOutTimer = 0;

static void updateRadarZoom() {
    WriteWord(0x267E04, (uint16_t)zoom);
    CRadar_m_radarRange = zoom;
}

static void showVehicleAreaZOomOutMinimap() {
    CPad* pad = CPad_GetPad(0);

    if(pad->NewState.DPadDown && !pad->OldState.DPadDown) {
        WriteDword(0x66BC14, 1);
        WriteDword(0x66BC0C, 1500);
        zoom = 320.0f;
        zoomOutTimer = CTimer_m_snTimeInMilliseconds;
    }

    if ( zoom > 160.0f ) {
        if ( CTimer_m_snTimeInMilliseconds - zoomOutTimer > 1000 ) {
            zoom -= 10.0f;
            if ( 160.0f > zoom ) {
                zoom = 160.0f;
            }
        }
    }

    DrawHud();
}


void injectHUDPatches() {
    CRGBA_CRGBA(&sub_color, 220, 128, 128, 255);
    CRGBA_CRGBA(&add_color, 128, 220, 128, 255);
    CRGBA_CRGBA(&shadow_color, 0, 0, 0, 255);
    CRGBA_CRGBA(&cur_color, 255, 255, 255, 255);
    CRGBA_CRGBA(&star_slot, 0, 0, 0, 1);
    CRGBA_CRGBA(&star_slot_shadow, 0, 0, 0, 32);
    CRGBA_CRGBA(&gray_color, 128, 128, 128, 255);

    RedirectCall(0x2ABA98, CHudNew_DrawAmmo);
    RedirectCall(0x2ABAD4, CHudNew_DrawAmmo);

    RedirectCall(0x2B00B4, DrawWantedStars);

    RedirectCall(0x2A2154, unloadShopHideMoney);
    RedirectCall(0x4BD31C, unloadShopHideMoney);
    RedirectCall(0x4F1658, unloadShopHideMoney);
    RedirectCall(0x505A78, unloadShopHideMoney);

    RedirectCall(0x4BD33C, loadShopShowMoney);
    RedirectCall(0x4D402C, loadShopShowMoney);

    RedirectCall(0x2AB9C4, drawPlayerMoney);

    RedirectCall(0x2AB4B0, CHud_PrintHealthForPlayer);
    RedirectCall(0x2AB4F8, CHud_PrintHealthForPlayer); 

    RedirectCall(0x2AB518, CHud_PrintArmourForPlayer);
    RedirectCall(0x2AB55C, CHud_PrintArmourForPlayer); 

    WriteDword(0x2AB688, 0x0220702D); // move $t6, $s1
    RedirectCall(0x2AB6AC, CHud_PrintBreathOrAbilityForPlayer);

    WriteDword(0x2AB6B4, 0x0200702D); // move $t6, $s0
    RedirectCall(0x2AB6F4, CHud_PrintBreathOrAbilityForPlayer); 

    zoom = 160.0f;
    
    DrawHud = (void (*)())ReadCall(0x2471E4);
    RedirectCall(0x2471E4, showVehicleAreaZOomOutMinimap);
    MakeBranch(0x267D80, 0x27);
    MakeNop(0x267D84);

    RedirectCall(0x267E20, updateRadarZoom);
    MakeNop(0x267E24);

}
