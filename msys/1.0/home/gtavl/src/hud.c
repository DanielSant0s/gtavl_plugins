
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

uint16_t* CHud_m_ItemToFlash = (uint16_t*)0x66BBFC;

uint8_t* MaxHealth = (uint8_t*)0x70974B;
uint32_t* CWorld_PlayerInFocus = (uint32_t*)0x7095D0;

float (*CStats_GetFatAndMuscleModifier)(unsigned int) = (float (*)(unsigned int))0x2D8C20;
void (*CSprite2d_DrawBarChart)(float, float, unsigned short, unsigned char, float, signed char, unsigned char, unsigned char, RwRGBA*, RwRGBA*) = (void (*)(float, float, unsigned short, unsigned char, float, signed char, unsigned char, unsigned char, RwRGBA*, RwRGBA*))0x2B0B00;

void CHud_PrintHealthForPlayer(int playerID, float posX, float posY)
{
    float width;
    float fPercentage;
    float fPercentagea;
    RwRGBA v10;
    RwRGBA color;

    if (*CHud_m_ItemToFlash != 4)
    {
        fPercentage = 100.0f * 109.0f;
        width = fPercentage / CStats_GetFatAndMuscleModifier(10);
        fPercentagea = (*(float*)(CWorld_PlayerInFocus[111 * playerID] + 1408));
        CRGBA_CRGBA(&v10, 0, 0, 0, 255);

        if (*(float*)(CWorld_PlayerInFocus[111 * playerID] + 1408) >= 30.0f) {
            CRGBA_CRGBA(&color, 0x3e, 0x9b, 0x01, 255);
        } else {
            if ((*CTimer_m_FrameCounter & 8) != 0) {
                CRGBA_CRGBA(&color, 255, 0, 0, 255);
            } else {
                CRGBA_CRGBA(&color, 128, 0, 0, 255);
            }
            
        }

        CSprite2d_DrawBarChart(40.0f, 420.0f, width, 9.0f, fPercentagea, 0, 0, 1, &color, &v10);
    }
}

float* CRadar_m_radarRange = (float*)0x66B958;

static uint32_t show_msec = 0;
static uint32_t add_msec = 0;

static bool money_visible = false;
static bool score_set_visible = false;
static bool shop_active = false;

#define PlayerInfoSize 111

uint32_t* DisplayedScore = (uint32_t*)0x7096B8;
uint32_t* Score = (uint32_t*)0x7096B4;
uint8_t* PlayerInFocus = (uint8_t*)0x66BA74;

static char money_diff_str[32];
static char money_str[32];

static uint32_t curr_money = 0;
static uint32_t last_money = 0;

static RwRGBA add_color, sub_color, cur_color, shadow_color, star_slot, star_slot_shadow, gray_color;

#define ABS(a)  (((a) < 0) ? (-(a)) : (a))
#define isNearlyEqualF(a, b, t) (fabsf(a - b) <= t)
#define interpF(a, b, f) a = a + (f) * (b - a)
#define norm(value, min, max) (((value) < (min)) ? 0 : (((value) > (max)) ? 1 : (((value) - (min)) / ((max) - (min)))))
#define lerp(no, mi, ma) ( (no) * ((ma) - (mi)) + (mi) )

#define clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))


enum eWeaponState
{
    WEAPONSTATE_READY,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT
};

enum eWeaponType
{
	WEAPON_UNARMED = 0x0,
	WEAPON_BRASSKNUCKLE = 0x1,
	WEAPON_GOLFCLUB = 0x2,
	WEAPON_NIGHTSTICK = 0x3,
	WEAPON_KNIFE = 0x4,
	WEAPON_BASEBALLBAT = 0x5,
	WEAPON_SHOVEL = 0x6,
	WEAPON_POOLCUE = 0x7,
	WEAPON_KATANA = 0x8,
	WEAPON_CHAINSAW = 0x9,
	WEAPON_DILDO1 = 0xA,
	WEAPON_DILDO2 = 0xB,
	WEAPON_VIBE1 = 0xC,
	WEAPON_VIBE2 = 0xD,
	WEAPON_FLOWERS = 0xE,
	WEAPON_CANE = 0xF,
	WEAPON_GRENADE = 0x10,
	WEAPON_TEARGAS = 0x11,
	WEAPON_MOLOTOV = 0x12,
	WEAPON_ROCKET = 0x13,
	WEAPON_ROCKET_HS = 0x14,
	WEAPON_FREEFALL_BOMB = 0x15,
	WEAPON_PISTOL = 0x16,
	WEAPON_PISTOL_SILENCED = 0x17,
	WEAPON_DESERT_EAGLE = 0x18,
	WEAPON_SHOTGUN = 0x19,
	WEAPON_SAWNOFF = 0x1A,
	WEAPON_SPAS12 = 0x1B,
	WEAPON_MICRO_UZI = 0x1C,
	WEAPON_MP5 = 0x1D,
	WEAPON_AK47 = 0x1E,
	WEAPON_M4 = 0x1F,
	WEAPON_TEC9 = 0x20,
	WEAPON_COUNTRYRIFLE = 0x21,
	WEAPON_SNIPERRIFLE = 0x22,
	WEAPON_RLAUNCHER = 0x23,
	WEAPON_RLAUNCHER_HS = 0x24,
	WEAPON_FTHROWER = 0x25,
	WEAPON_MINIGUN = 0x26,
	WEAPON_SATCHEL_CHARGE = 0x27,
	WEAPON_DETONATOR = 0x28,
	WEAPON_SPRAYCAN = 0x29,
	WEAPON_EXTINGUISHER = 0x2A,
	WEAPON_CAMERA = 0x2B,
	WEAPON_NIGHTVISION = 0x2C,
	WEAPON_INFRARED = 0x2D,
	WEAPON_PARACHUTE = 0x2E,
	WEAPON_UNUSED = 0x2F,
	WEAPON_ARMOUR = 0x30,
	WEAPON_FLARE = 0x3A
};

typedef struct {
    uint32_t m_nType;
    uint32_t m_nState;
	unsigned int m_nAmmoInClip;
	unsigned int m_nTotalAmmo;
	unsigned int m_nTimeForNextShot;
	char field_14;
	char field_15;
	char field_16;
	char field_17;
    void *m_pFxSystem; // flamethrower, spraycan, extinguisher particle
} CWeapon;

#define getPedActiveWeaponSlot(ped) *(uint8_t*)((uint32_t)ped + 0x758)
#define getPedWeapons(ped) (CWeapon*)((uint32_t)ped + 0x5E0)

char (*CPed_GetWeaponSkill)(void* ped) = (char (*)(void* ped))0x19A1A0;


#define MAX_WEAPON_INFOS 80
#define MAX_WEAPON_NAMES 50

#define WEAPONINFO_NUM_WEAPONS_WITH_SKILLS 11
#define WEAPONINFO_NUM_WEAPONS 46

enum eWeaponSkill
{
	WEAPSKILL_POOR,
	WEAPSKILL_STD,
	WEAPSKILL_PRO,
	WEAPSKILL_COP
};

typedef struct {
    /* some info here https://code.google.com/p/mtasa-blue/source/browse/tags/1.3.4/MTA10/game_sa/CWeaponInfoSA.h */
    unsigned int   m_nWeaponFire; // see eWeaponFire
    float          m_fTargetRange; // max targeting range
    float          m_fWeaponRange; // absolute gun range / default melee attack range
    int            m_nModelId1; // modelinfo id
    int            m_nModelId2; // second modelinfo id
    unsigned int   m_nSlot;
    struct {
        unsigned int bCanAim : 1;
        unsigned int bAimWithArm : 1;
        unsigned int b1stPerson : 1;
        unsigned int bOnlyFreeAim : 1;
        unsigned int bMoveAim : 1; // can move when aiming
        unsigned int bMoveFire : 1; // can move when firing
        unsigned int b06 : 1; // this bitfield is not used
        unsigned int b07 : 1; // this bitfield is not used
        unsigned int bThrow : 1;
        unsigned int bHeavy : 1; // can't run fast with this weapon
        unsigned int bContinuosFire : 1;
        unsigned int bTwinPistol : 1;
        unsigned int bReload : 1; // this weapon can be reloaded
        unsigned int bCrouchFire : 1; // can reload when crouching
        unsigned int bReload2Start : 1; // reload directly after firing
        unsigned int bLongReload : 1;
        unsigned int bSlowdown : 1;
        unsigned int bRandSpeed : 1;
        unsigned int bExpands : 1;
    }              m_nFlags;
	unsigned int   m_nAnimToPlay; // instead of storing pointers directly to anims, use anim association groups
	unsigned short m_nAmmoClip; // ammo in one clip
	unsigned short m_nDamage; // damage inflicted per hit
	CVector        m_vecFireOffset; // offset from weapon origin to projectile starting point
	unsigned int   m_nSkillLevel; // what's the skill level of this weapontype
	unsigned int   m_nReqStatLevel; // what stat level is required for this skill level
	float          m_fAccuracy; // modify accuracy of weapon
	float          m_fMoveSpeed; // how fast can move with weapon
	float          m_fAnimLoopStart; // start of animation loop
	float          m_fAnimLoopEnd; // end of animation loop
	unsigned int   m_nAnimLoopFire; // time in animation when weapon should be fired
	unsigned int   m_nAnimLoop2Start; // start of animation2 loop
	unsigned int   m_nAnimLoop2End; // end of animation2 loop
	unsigned int   m_nAnimLoop2Fire; // time in animation2 when weapon should be fired
	float          m_fBreakoutTime; // time after which player can break out of attack and run off
	float          m_fSpeed; // speed of projectile
	float          m_fRadius; // radius affected
	float          m_fLifespan; // time taken for shot to dissipate
	float          m_fSpread; // angle inside which shots are created
	unsigned short m_nAimOffsetIndex; // index into array of aiming offsets
	unsigned char  m_nBaseCombo; // base combo for this melee weapon
	unsigned char  m_nNumCombos; // how many further combos are available
} CWeaponInfo;

CWeaponInfo *(*CWeaponInfo_GetWeaponInfo)(uint32_t weaponType, unsigned char skill) = (CWeaponInfo *(*)(uint32_t weaponType, unsigned char skill))0x1410B0;

bool bShowAmmo;
int nAmmoFadeAlpha;
int nTimeToShowAmmoDifference;

bool (*CDarkel_FrenzyOnGoing)() = (bool (*)())0x2D1770;

RwRGBA dropfade_color;

void CHudNew_DrawAmmo() {
    void* playa = FindPlayerPed(0);
    int slot = getPedActiveWeaponSlot(playa);
    CWeapon* weapons = getPedWeapons(playa);
    int weaponType = weapons[slot].m_nType;
    int totalAmmo = weapons[slot].m_nTotalAmmo;
    int ammoInClip = weapons[slot].m_nAmmoInClip;
    int maxAmmoInClip = CWeaponInfo_GetWeaponInfo(weapons[slot].m_nType, CPed_GetWeaponSkill(playa))->m_nAmmoClip;
    char str_ammo[16], str_clip[16];

    if (maxAmmoInClip <= 1 || maxAmmoInClip >= 1000) {
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
        nAmmoFadeAlpha = (int)interpF(nAmmoFadeAlpha, 255, 0.2f * *CTimer_ms_fTimeStep);
    else
        nAmmoFadeAlpha = (int)interpF(nAmmoFadeAlpha, 0, 0.2f * *CTimer_ms_fTimeStep);

    static int previousTotalAmmo;
    static int previousAmmoInClip;
    if (previousTotalAmmo != (totalAmmo - ammoInClip) || previousAmmoInClip != ammoInClip) {
        bShowAmmo = true;
        nTimeToShowAmmoDifference = *CTimer_m_snTimeInMilliseconds + 8000;
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

    if (CDarkel_FrenzyOnGoing()
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

    if (nTimeToShowAmmoDifference < *CTimer_m_snTimeInMilliseconds)
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

void (*CShopping_LoadShop)(const char* a1) = (void (*)(const char* a1))0x4F1EB0;
void (*CShopping_RemoveLoadedShop)() = (void (*)())0x4F2410;

void loadShopShowMoney(const char* a1) {
    shop_active = true;
    CShopping_LoadShop(a1);
}

void unloadShopHideMoney() {
    shop_active = false;
    CShopping_RemoveLoadedShop();
}


void drawPlayerMoney() {
    CPad* pad = CPad_GetPad(0);

    float money_y = 30.0f;
    float money_set_y = 50.0f;

    if (FindPlayerWanted(-1)->m_nWantedLevel > 0) {
        money_y += 20.0f;
        money_set_y += 20.0f;
    }

    last_money = curr_money;
    curr_money = Score[(*PlayerInFocus) * PlayerInfoSize];

    if (last_money != curr_money) {
        add_msec = *CTimer_m_snTimeInMilliseconds;
        show_msec = *CTimer_m_snTimeInMilliseconds;
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
        show_msec = *CTimer_m_snTimeInMilliseconds;
        money_visible = true;
        updateMoney();
        
    }

    if ( money_visible || shop_active ) {
        setupCurrentMoneyText();
        CFont_PrintString(612.0f, money_y, money_str);
        if (((*CTimer_m_snTimeInMilliseconds - show_msec) > 5000) && !shop_active) {
            money_visible = false;
        }
    }

    if (score_set_visible) {
        setupSetMoneyText(money_diff_str[0] == '-');
        CFont_PrintString(612.0f, money_set_y, money_diff_str);
        if ((*CTimer_m_snTimeInMilliseconds - add_msec) > 5000) {
            score_set_visible = false;
        }
    }

}

void DrawWantedStars() {
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

    if (FindPlayerWanted(-1)->m_nLastTimeWantedLevelChanged + 2000 > *CTimer_m_snTimeInMilliseconds)
        c = (( (*CTimer_m_snTimeInMilliseconds % 600) < 300 ) ? &cur_color : &gray_color);

    CFont_SetDropColor(&shadow_color);
    CFont_SetColor(c);
    CFont_PrintString(x, y, star_buf);
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

}
