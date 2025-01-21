#include <injector.h>
#include <stdio.h>
#include <common.h>
#include <math.h>
#include <string.h>
#include "hooks.h"

#include "anim_association.h"

CEntity* LastCharDamageEntity;
float LastCharDamageDamage;
int LastCharDamageBodyPart;
int LastCharDamageWeaponType;
uint32_t LastCharDamageVictim;

typedef struct
{
  CEntity *pEntity;
  float fDamage;
  int bodyPart;
  int weaponType;
  char bSpeak;
  char __pad[3];
} CPedDamageResponseCalculator;

static void RegisterCharDamage() {
    CPedDamageResponseCalculator* damageCalculator;

    asm(
    "addiu $sp, $sp, -16\n\t"
    "sw $a0, 0($sp)\n\t"
    "sw $a1, 4($sp)\n\t"
    "sw $a2, 8($sp)\n\t"
    "sw $a3, 12($sp)\n\t");
    
     asm volatile("move %0, $s3 \n"
                  "sw      $zero, 4($s1) \n"
                  "sb      $zero, 8($s1) \n"
                  "sb      $zero, 9($s1) \n"
                  "sb      $v0, 0xA($s1) \n"
                 : "=r" (damageCalculator));

    asm volatile("move %0, $a1 \n" : "=r" (LastCharDamageVictim));
    LastCharDamageEntity = damageCalculator->pEntity;
    LastCharDamageDamage = damageCalculator->fDamage;
    LastCharDamageBodyPart = damageCalculator->bodyPart;
    LastCharDamageWeaponType = damageCalculator->weaponType;

    asm(
    "lw $a0, 0($sp)\n\t"
    "lw $a1, 4($sp)\n\t"
    "lw $a2, 8($sp)\n\t"
    "lw $a3, 12($sp)\n\t"
    "addiu $sp, $sp, 16\n\t"
    );
}

void* CTaskSimpleDie_Constructor(void* task, int animGroupId, int animId, float blendDelta, float animSpeed);
void* CTaskSimpleDie_Constructor0(void* task, const char* animName, const char* animBlock, int animFlags, float blendDelta, float animSpeed);

static const char* front_death_anims[] =        { "15", "19", "22", "23", "KO_shot_stom", "29", "35", };
static const char* strong_front_death_anims[] = { "6", "16", "5", };
static const char* back_death_anims[] =         { "11", "12", "13", "14", "24", "30", "37", };
static const char* left_death_anims[] =         { "3", "21", "31", "33", "38", };
static const char* right_death_anims[] =        { "4", "20", "32", "34", "39", };
static const char* shot_front_anims[] =         { "KO_shot_face", "26", "17", "25", "28", };
static const char* shot_strong_front_anims[] =  { "18", "8", "2", "3", "9", "27", };

static void* init_death_anim(const char** anim_group, int num_anims, void* task, float blendDelta, float animSpeed) {
    char anim_name[16] = "deathmod_";

    const char* anim_base = anim_group[randint(0, num_anims)];

    if (((anim_base[0]>='0') && (anim_base[0]<='9'))) {
        strcat(anim_name, anim_base);
        anim_base = anim_name;
    }

    printf("anim %s\n", anim_base);

    return CTaskSimpleDie_Constructor0(task, anim_base, "PED", 208, blendDelta, animSpeed);
}

static void* CTaskSimpleDie_CustomConstructor(void* task, int animGroupId, int animId, float blendDelta, float animSpeed) {
    if (randint(0, 5) == 5) 
        return CTaskSimpleDie_Constructor(task, animGroupId, animId, blendDelta, animSpeed);

    if (animId == ANIM_ID_KO_SKID_FRONT) {
        if (LastCharDamageDamage > 200.0f) {
            return init_death_anim(strong_front_death_anims, 3, task, blendDelta, animSpeed);
        }
        return init_death_anim(front_death_anims, 7, task, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SKID_BACK) {
        return init_death_anim(back_death_anims, 7, task, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SPIN_L) {
        return init_death_anim(left_death_anims, 5, task, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SPIN_R) {
        return init_death_anim(right_death_anims, 5, task, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SHOT_FRONT_0) {
        if (LastCharDamageDamage < 30.0f && LastCharDamageBodyPart == 9) {
            return init_death_anim(shot_front_anims, 5, task, blendDelta, animSpeed);
        }
        return init_death_anim(shot_strong_front_anims, 6, task, blendDelta, animSpeed);
    }

    return CTaskSimpleDie_Constructor(task, animGroupId, animId, blendDelta, animSpeed);
    
}

void CSprite_RenderBufferedOneXLUSprite2D(float x, float y, float w, float h, RwRGBA* color, short intensity, uint8_t alpha);

static void CSprite_RenderBufferedOneXLUSprite2DAspect(float x, float y, float w, float h, RwRGBA* color, short intensity, uint8_t alpha) {
    CSprite_RenderBufferedOneXLUSprite2D(x, y, w*0.75f, h, color, intensity, alpha);
}

void CSprite_RenderBufferedOneXLUSprite_Rotate(CVector* pos, float w, float h, uint8_t r, uint8_t g, uint8_t b, short intensity, float recipNearZ, float angle, uint8_t a12);

static void CSprite_RenderBufferedOneXLUSprite_Rotate_Aspect(CVector* pos, float w, float h, uint8_t r, uint8_t g, uint8_t b, short intensity, float recipNearZ, float angle, uint8_t a12) {
    CSprite_RenderBufferedOneXLUSprite_Rotate(pos, w*0.75f, h, r, g, b, intensity, recipNearZ, angle, a12);
}

extern int AEAudioHardware; // 0x876990
bool CAEScriptAudioEntity_IsMissionAudioSampleFinished(void* this, uint8_t slot); // 0x58ED80
int CAEAudioUtility_GetBankAndSoundFromScriptSlotAudioEvent(int* wavId, int, int, uint8_t); // 0x559510
void CAEAudioHardware_LoadSoundBank(int* ah, uint16_t, uint8_t); // 0x557D40
void CAEAudioHardware_LoadSound(int* ah, uint16_t, uint16_t, uint8_t); // 0x557F10

typedef struct RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RgbColor;

typedef struct HsvColor
{
    uint8_t h;
    uint8_t s;
    uint8_t v;
} HsvColor;

static RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    uint8_t region, p, q, t;
    unsigned int h, s, v, remainder;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }
    
    h = hsv.h;
    s = hsv.s;
    v = hsv.v;

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = v;
            break;
        default:
            rgb.r = v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}

static HsvColor RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    uint8_t rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.s = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.s = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

enum eWeatherType {
    WEATHER_UNDEFINED = -1,

    WEATHER_EXTRASUNNY_LA = 0,
    WEATHER_SUNNY_LA = 1,
    WEATHER_EXTRASUNNY_SMOG_LA = 2,
    WEATHER_SUNNY_SMOG_LA = 3,
    WEATHER_CLOUDY_LA = 4,

    WEATHER_SUNNY_SF = 5,
    WEATHER_EXTRASUNNY_SF = 6,
    WEATHER_CLOUDY_SF = 7,
    WEATHER_RAINY_SF = 8,
    WEATHER_FOGGY_SF = 9,

    WEATHER_SUNNY_VEGAS = 10,
    WEATHER_EXTRASUNNY_VEGAS = 11,
    WEATHER_CLOUDY_VEGAS = 12,

    WEATHER_EXTRASUNNY_COUNTRYSIDE = 13,
    WEATHER_SUNNY_COUNTRYSIDE = 14,
    WEATHER_CLOUDY_COUNTRYSIDE = 15,
    WEATHER_RAINY_COUNTRYSIDE = 16,

    WEATHER_EXTRASUNNY_DESERT = 17,
    WEATHER_SUNNY_DESERT = 18,
    WEATHER_SANDSTORM_DESERT = 19,

    WEATHER_UNDERWATER = 20,
    WEATHER_EXTRACOLOURS_1 = 21,
    WEATHER_EXTRACOLOURS_2 = 22,

    NUM_WEATHERS,
    WEATHER_EXTRA_START = WEATHER_EXTRACOLOURS_1
};

#define NUM_HOURS 8

extern uint8_t CTimeCycle_m_nAmbientRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nAmbientGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nAmbientBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nAmbientRed_Obj[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nAmbientGreen_Obj[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nAmbientBlue_Obj[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nSkyTopRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSkyTopGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSkyTopBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nSkyBottomRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSkyBottomGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSkyBottomBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nSunCoreRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSunCoreGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSunCoreBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nSunCoronaRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSunCoronaGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nSunCoronaBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_fSunSize[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fSpriteSize[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fSpriteBrightness[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nShadowStrength[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nLightShadowStrength[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nPoleShadowStrength[NUM_HOURS][NUM_WEATHERS];
extern uint16_t CTimeCycle_m_fFarClip[NUM_HOURS][NUM_WEATHERS];
extern uint16_t CTimeCycle_m_fFogStart[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fLightsOnGroundBrightness[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nLowCloudsRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nLowCloudsGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nLowCloudsBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_nFluffyCloudsBottomRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nFluffyCloudsBottomGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nFluffyCloudsBottomBlue[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_fWaterRed[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fWaterGreen[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fWaterBlue[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fWaterAlpha[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_fPostFx1Red[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fPostFx1Green[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fPostFx1Blue[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fPostFx1Alpha[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_fPostFx2Red[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fPostFx2Green[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fPostFx2Blue[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_fPostFx2Alpha[NUM_HOURS][NUM_WEATHERS];

extern uint8_t CTimeCycle_m_fCloudAlpha[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nHighLightMinIntensity[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nWaterFogAlpha[NUM_HOURS][NUM_WEATHERS];
extern uint8_t CTimeCycle_m_nDirectionalMult[NUM_HOURS][NUM_WEATHERS];

void CGame_InitialiseCoreDataAfterRW();

static void SetupCustomColorCycleData() {
    CGame_InitialiseCoreDataAfterRW();

    RgbColor rgb;
    HsvColor hsv;

    int hour, weather;
    for (weather = 0; weather < NUM_WEATHERS; weather++) {
        for (hour = 0; hour < NUM_HOURS; hour++) {
            rgb.r = CTimeCycle_m_nAmbientRed[hour][weather];
            rgb.g = CTimeCycle_m_nAmbientGreen[hour][weather];
            rgb.b = CTimeCycle_m_nAmbientBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 0.5f);
            hsv.v = (uint8_t)(hsv.v * 0.5f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nAmbientRed[hour][weather] = rgb.r;
            CTimeCycle_m_nAmbientGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nAmbientBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_nAmbientRed_Obj[hour][weather];
            rgb.g = CTimeCycle_m_nAmbientGreen_Obj[hour][weather];
            rgb.b = CTimeCycle_m_nAmbientBlue_Obj[hour][weather];
        
            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 0.5f);
            hsv.v = (uint8_t)(hsv.v * 0.5f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nAmbientRed_Obj[hour][weather] = rgb.r;
            CTimeCycle_m_nAmbientGreen_Obj[hour][weather] = rgb.g;
            CTimeCycle_m_nAmbientBlue_Obj[hour][weather] = rgb.b;

            //----------------------------------------------------//

            rgb.r = CTimeCycle_m_nSkyTopRed[hour][weather];
            rgb.g = CTimeCycle_m_nSkyTopGreen[hour][weather];
            rgb.b = CTimeCycle_m_nSkyTopBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 0.7f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nSkyTopRed[hour][weather] = rgb.r;
            CTimeCycle_m_nSkyTopGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nSkyTopBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_nSkyBottomRed[hour][weather];
            rgb.g = CTimeCycle_m_nSkyBottomGreen[hour][weather];
            rgb.b = CTimeCycle_m_nSkyBottomBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 0.7f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nSkyBottomRed[hour][weather] = rgb.r;
            CTimeCycle_m_nSkyBottomGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nSkyBottomBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_nSunCoreRed[hour][weather];
            rgb.g = CTimeCycle_m_nSunCoreGreen[hour][weather];
            rgb.b = CTimeCycle_m_nSunCoreBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 1.0f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nSunCoreRed[hour][weather] = rgb.r;
            CTimeCycle_m_nSunCoreGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nSunCoreBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_nSunCoronaRed[hour][weather];
            rgb.g = CTimeCycle_m_nSunCoronaGreen[hour][weather];
            rgb.b = CTimeCycle_m_nSunCoronaBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 1.0f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nSunCoronaRed[hour][weather] = rgb.r;
            CTimeCycle_m_nSunCoronaGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nSunCoronaBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//

            rgb.r = CTimeCycle_m_nLowCloudsRed[hour][weather];
            rgb.g = CTimeCycle_m_nLowCloudsGreen[hour][weather];
            rgb.b = CTimeCycle_m_nLowCloudsBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 1.0f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nLowCloudsRed[hour][weather] = rgb.r;
            CTimeCycle_m_nLowCloudsGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nLowCloudsBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_nFluffyCloudsBottomRed[hour][weather];
            rgb.g = CTimeCycle_m_nFluffyCloudsBottomGreen[hour][weather];
            rgb.b = CTimeCycle_m_nFluffyCloudsBottomBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 1.0f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_nFluffyCloudsBottomRed[hour][weather] = rgb.r;
            CTimeCycle_m_nFluffyCloudsBottomGreen[hour][weather] = rgb.g;
            CTimeCycle_m_nFluffyCloudsBottomBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_fWaterRed[hour][weather];
            rgb.g = CTimeCycle_m_fWaterGreen[hour][weather];
            rgb.b = CTimeCycle_m_fWaterBlue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 1.0f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_fWaterRed[hour][weather] = rgb.r;
            CTimeCycle_m_fWaterGreen[hour][weather] = rgb.g;
            CTimeCycle_m_fWaterBlue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_fPostFx1Red[hour][weather];
            rgb.g = CTimeCycle_m_fPostFx1Green[hour][weather];
            rgb.b = CTimeCycle_m_fPostFx1Blue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 0.5f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_fPostFx1Red[hour][weather] = rgb.r;
            CTimeCycle_m_fPostFx1Green[hour][weather] = rgb.g;
            CTimeCycle_m_fPostFx1Blue[hour][weather] = rgb.b;

            //----------------------------------------------------//
        
            rgb.r = CTimeCycle_m_fPostFx2Red[hour][weather];
            rgb.g = CTimeCycle_m_fPostFx2Green[hour][weather];
            rgb.b = CTimeCycle_m_fPostFx2Blue[hour][weather];

            hsv = RgbToHsv(rgb);
            hsv.s = (uint8_t)(hsv.s * 0.5f);
            rgb = HsvToRgb(hsv);

            CTimeCycle_m_fPostFx2Red[hour][weather] = rgb.r;
            CTimeCycle_m_fPostFx2Green[hour][weather] = rgb.g;
            CTimeCycle_m_fPostFx2Blue[hour][weather] = rgb.b;

            //----------------------------------------------------//

        }
    }
}

void injectMiscPatches() {
    RedirectCall(0x2457F0, &SetupCustomColorCycleData);

    RedirectJump(0x3C44F0, &CSprite_RenderBufferedOneXLUSprite_Rotate_Aspect);

    RedirectCall(0x297AFC, &CSprite_RenderBufferedOneXLUSprite2DAspect);
    RedirectCall(0x297C68, &CSprite_RenderBufferedOneXLUSprite2DAspect);
    RedirectCall(0x297D60, &CSprite_RenderBufferedOneXLUSprite2DAspect);

    RedirectCall(0x419A28, &CTaskSimpleDie_CustomConstructor);
    RedirectCall(0x41A150, &CTaskSimpleDie_CustomConstructor);
    RedirectCall(0x41AA44, &CTaskSimpleDie_CustomConstructor);
    RedirectCall(0x425998, &CTaskSimpleDie_CustomConstructor);
    RedirectCall(0x42D084, &CTaskSimpleDie_CustomConstructor);

    RedirectCall(0x3E19C8, &RegisterCharDamage);
    MakeNop(0x3E19CC);
    MakeNop(0x3E19D0);
    MakeNop(0x3E19D4);

    MakeNop(0x267E24);

    //MakeRetn(0x4FB520); // Fix heap corruption

    MakeNop(0x5157AC); // Disable motion blur
    MakeNop(0x5157C0); // Disable motion blur

    MakeNop(0x14EB50); // No Wheel Turn Back
    MakeNop(0x14ECF0); // No Wheel Turn Back
    MakeNop(0x14ED84); // No Wheel Turn Back

    MakeNop(0x262804); // No flip explosions - player
    MakeNop(0x146790); // No flip explosions - ped

    WriteByte(0x6FF98F, 1); // Widescreen by default
	WriteByte(0x6FF99A, 0); // Enable Radio EQ
	WriteByte(0x66B8B8, 0); // Enable Invert Look

    WriteByte(0x64B6A8, 255); // marker_r
    WriteByte(0x64B6A9, 255); // marker_g
    WriteByte(0x64B6AA, 0);   // marker_b

    WriteWord(0x242DFC, 2000);  // Make a day pass in 48 minutes
    WriteDword(0x66B79C, 2000); // Make a day pass in 48 minutes

    MakeRetn(0x516D80); // Disable heat haze

    MakeBranch(0x4BB188, 0x15B); // Disable enex

    MakeBranch(0x4BC998, 0x3F); // Disable Door Camera

    WriteByte(0x666C4C, 0); // Change moon size

}
