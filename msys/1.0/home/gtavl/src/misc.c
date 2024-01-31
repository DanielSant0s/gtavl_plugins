#include <injector.h>
#include <stdio.h>
#include <common.h>
#include <math.h>
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

static const char* front_death_anims[] = {
    "deathmod_15",
    "deathmod_19",
    "deathmod_22",
    "deathmod_23",
    "KO_shot_stom",
    "deathmod_29",
    "deathmod_35",
};

static const char* strong_front_death_anims[] = {
    "deathmod_6",
    "deathmod_16",
    "deathmod_5",
};

static const char* back_death_anims[] = {
    "deathmod_11",
    "deathmod_12",
    "deathmod_13",
    "deathmod_14",
    "deathmod_24",
    "deathmod_30",
    "deathmod_37",
};

static const char* left_death_anims[] = {
    "deathmod_3",
    "deathmod_21",
    "deathmod_31",
    "deathmod_33",
    "deathmod_38",  
};

static const char* right_death_anims[] = {
    "deathmod_4",
    "deathmod_20",
    "deathmod_32",
    "deathmod_34",
    "deathmod_39",
};

static const char* shot_front_anims[] = {
    "KO_shot_face",
    "deathmod_26",
    "deathmod_17",
    "deathmod_25",
    "deathmod_28",
};

static const char* shot_strong_front_anims[] = {
    "deathmod_18",
    "deathmod_8",
    "deathmod_2",
    "deathmod_3",
    "deathmod_9",
    "deathmod_27",
};

static void* CTaskSimpleDie_CustomConstructor(void* task, int animGroupId, int animId, float blendDelta, float animSpeed) {
    if (randint(0, 5) == 5) 
        return CTaskSimpleDie_Constructor(task, animGroupId, animId, blendDelta, animSpeed);

    if (animId == ANIM_ID_KO_SKID_FRONT) {
        if (LastCharDamageDamage > 200.0f) {
            int animIdx = randint(0, 3);
            return CTaskSimpleDie_Constructor0(task, strong_front_death_anims[animIdx], "PED", 208, blendDelta, animSpeed);
        }
        int animIdx = randint(0, 7);
        return CTaskSimpleDie_Constructor0(task, front_death_anims[animIdx], "PED", 208, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SKID_BACK) {
        int animIdx = randint(0, 7);
        return CTaskSimpleDie_Constructor0(task, back_death_anims[animIdx], "PED", 208, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SPIN_L) {
        int animIdx = randint(0, 5);
        return CTaskSimpleDie_Constructor0(task, left_death_anims[animIdx], "PED", 208, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SPIN_R) {
        int animIdx = randint(0, 5);
        return CTaskSimpleDie_Constructor0(task, right_death_anims[animIdx], "PED", 208, blendDelta, animSpeed);
    } else if (animId == ANIM_ID_KO_SHOT_FRONT_0) {
        if (LastCharDamageDamage < 30.0f && LastCharDamageBodyPart == 9) {
            int animIdx = randint(0, 5);
            return CTaskSimpleDie_Constructor0(task, shot_front_anims[animIdx], "PED", 208, blendDelta, animSpeed);
        }
        int animIdx = randint(0, 6);
        return CTaskSimpleDie_Constructor0(task, shot_strong_front_anims[animIdx], "PED", 208, blendDelta, animSpeed);
    }

    return CTaskSimpleDie_Constructor(task, animGroupId, animId, blendDelta, animSpeed);
    
}

void injectMiscPatches() {
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
    WriteByte(0x64B6A9, 255); //marker_g
    WriteByte(0x64B6AA, 0); //marker_b

    WriteWord(0x242DFC, 2000);  // Make a day pass in 48 minutes
    WriteDword(0x66B79C, 2000); // Make a day pass in 48 minutes

    MakeRetn(0x516D80); // Disable heat haze

    MakeBranch(0x4BB188, 0x15B); // Disable enex

    MakeBranch(0x4BC998, 0x3F); // Disable Door Camera

}
