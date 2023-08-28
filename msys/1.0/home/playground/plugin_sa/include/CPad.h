#ifndef CPAD_H_
#define CPAD_H_

// Set values to 128 unless otherwise specified
typedef struct {
    signed short LeftStickX; // move/steer left (-128?)/right (+128)
    signed short LeftStickY; // move back(+128)/forwards(-128?)
    signed short RightStickX; // numpad 6(+128)/numpad 4(-128?)
    signed short RightStickY;
    
    signed short LeftShoulder1;
    signed short LeftShoulder2;
    signed short RightShoulder1; // target / hand brake
    signed short RightShoulder2; 
    
    signed short DPadUp; // radio change up           Next radio station / Call gang forward/Recruit gang member
    signed short DPadDown; // radio change down       Previous radio station / Gang stay back/Release gang (hold)
    signed short DPadLeft; //                         Skip trip/Action / Negative talk reply
    signed short DPadRight; //                        Next user MP3 track / Positive talk reply
    
    signed short Start;                             //Pause
    signed short Select;                            //Camera modes
    
    signed short ButtonSquare; // jump / reverse      Break/Reverse / Jump/Climb
    signed short ButtonTriangle; // get in/out        Exit vehicle / Enter veihcle
    signed short ButtonCross; // sprint / accelerate  Accelerate / Sprint/Swim
    signed short ButtonCircle; // fire                Fire weapon
    
    signed short ShockButtonL;
    signed short ShockButtonR; // look behind

} CControllerState;

typedef struct {
    CControllerState NewState;
    CControllerState OldState;
    short SteeringLeftRightBuffer[10];
    int DrunkDrivingBufferUsed;
    CControllerState PCTempJoyState;
    char Phase;

    char _pad109;

    short Mode;
    short ShakeDur;

    union
    {
        struct
        {
            unsigned short unk1 : 1; // eCamMode::MODE_1STPERSON leftover?
            unsigned short unk2 : 1; // unused
            unsigned short bPlayerAwaitsInGarage : 1;
            unsigned short bPlayerOnInteriorTransition : 1;
            unsigned short unk3 : 1; // unused
            unsigned short bPlayerSafe : 1;
            unsigned short bPlayerTalksOnPhone : 1; // bPlayerSafeForPhoneCall?
            unsigned short bPlayerSafeForCutscene : 1;
            unsigned short bPlayerSkipsToDestination : 1; // bPlayerSafeForDestination?
        };
        unsigned short DisablePlayerControls; 
    };
    char ShakeFreq;
    char bHornHistory[5];
    char iCurrHornHistory;
    char JustOutOfFrontEnd;
    char bApplyBrakes;
    char bDisablePlayerEnterCar;
    char bDisablePlayerDuck;
    char bDisablePlayerFireWeapon;
    char bDisablePlayerFireWeaponWithL1;
    char bDisablePlayerCycleWeapon;
    char bDisablePlayerJump;
    char bDisablePlayerDisplayVitalStats;
    int LastTimeTouched;
    int AverageWeapon;
    int AverageEntries;
    int NoShakeBeforeThis;
    char NoShakeFreq;

    char _pad131[3];
} CPad;

extern CPad* (*CPad_GetPad)(int);
extern void (*CPad_UpdatePads)(void);

#endif
