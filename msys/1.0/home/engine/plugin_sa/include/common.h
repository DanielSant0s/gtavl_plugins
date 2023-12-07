#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <rwcore.h>
#include <CVector.h>

// Available rates
#define kBUSCLK         (147456000)
#define kBUSCLKBY16     (kBUSCLK / 16)
#define kBUSCLKBY256    (kBUSCLK / 256)
#define kHBLNK_NTSC     (15734)
#define kHBLNK_PAL      (15625)
#define kHBLNK_DTV480p  (31469)
#define kHBLNK_DTV1080i (33750)

#define PS2_CLOCKS_PER_SEC kBUSCLKBY256 // 576.000
#define PS2_CLOCKS_PER_MSEC (PS2_CLOCKS_PER_SEC / 1000) // 576

#define LOWORD(a) ((unsigned short)(a))
#define HIWORD(a) ((unsigned short)(((unsigned int)(a) >> 16) & 0xFFFF))

typedef struct {
    // RwV3d-like:
    CVector      right;
    unsigned int flags;
    CVector      up;
    unsigned int pad1;
    CVector      at;
    unsigned int pad2;
    CVector      pos;
    unsigned int pad3;
    
	RwMatrix *m_pAttachMatrix;
	int m_bOwnsAttachedMatrix; // do we need to delete attaching matrix at detaching
} CMatrix;

typedef struct
{
	CVector m_vPosn;
	float m_fHeading;
} CSimpleTransform;

typedef struct
{
    uint32_t padding;
    CSimpleTransform placement;
    CMatrix *m_matrix;

} CPlaceable;

typedef struct CEntity CEntity;

struct CEntity {
    CPlaceable placeable;
    union {
        void *m_pRwObject;
        void *m_pRwClump;
        void *m_pRwAtomic;
    };
    /* https://code.google.com/p/mtasa-blue/source/browse/trunk/MTA10/game_sa/CEntitySA.h */
    unsigned int m_bUsesCollision : 1;           // does entity use collision
    unsigned int m_bCollisionProcessed : 1;  // has object been processed by a ProcessEntityCollision function
    unsigned int m_bIsStatic : 1;                // is entity static
    unsigned int m_bHasContacted : 1;            // has entity processed some contact forces
    unsigned int m_bIsStuck : 1;             // is entity stuck
    unsigned int m_bIsInSafePosition : 1;        // is entity in a collision free safe position
    unsigned int m_bWasPostponed : 1;            // was entity control processing postponed
    unsigned int m_bIsVisible : 1;               //is the entity visible
 
    unsigned int m_bIsBIGBuilding : 1;           // Set if this entity is a big building
    unsigned int m_bRenderDamaged : 1;           // use damaged LOD models for objects with applicable damage
    unsigned int m_bStreamingDontDelete : 1; // Dont let the streaming remove this 
    unsigned int m_bRemoveFromWorld : 1;     // remove this entity next time it should be processed
    unsigned int m_bHasHitWall : 1;              // has collided with a building (changes subsequent collisions)
    unsigned int m_bImBeingRendered : 1;     // don't delete me because I'm being rendered
    unsigned int m_bDrawLast :1;             // draw object last
    unsigned int m_bDistanceFade :1;         // Fade entity because it is far away
 
    unsigned int m_bDontCastShadowsOn : 1;       // Dont cast shadows on this object
    unsigned int m_bOffscreen : 1;               // offscreen flag. This can only be trusted when it is set to true
    unsigned int m_bIsStaticWaitingForCollision : 1; // this is used by script created entities - they are static until the collision is loaded below them
    unsigned int m_bDontStream : 1;              // tell the streaming not to stream me
    unsigned int m_bUnderwater : 1;              // this object is underwater change drawing order
    unsigned int m_bHasPreRenderEffects : 1; // Object has a prerender effects attached to it
    unsigned int m_bIsTempBuilding : 1;          // whether or not the building is temporary (i.e. can be created and deleted more than once)
    unsigned int m_bDontUpdateHierarchy : 1; // Don't update the aniamtion hierarchy this frame
 
    unsigned int m_bHasRoadsignText : 1;     // entity is roadsign and has some 2deffect text stuff to be rendered
    unsigned int m_bDisplayedSuperLowLOD : 1;
    unsigned int m_bIsProcObject : 1;            // set object has been generate by procedural object generator
    unsigned int m_bBackfaceCulled : 1;          // has backface culling on
    unsigned int m_bLightObject : 1;         // light object with directional lights
    unsigned int m_bUnimportantStream : 1;       // set that this object is unimportant, if streaming is having problems
    unsigned int m_bTunnel : 1;          // Is this model part of a tunnel
    unsigned int m_bTunnelTransition : 1;        // This model should be rendered from within and outside of the tunnel
    /* */
    unsigned short m_nRandomSeed;
    unsigned short m_nModelIndex;
    void *m_pReferences;
    void *m_pStreamingLink;
    short m_nScanCode;
    char m_nIplIndex;
    unsigned char m_nAreaCode;
    union {
        int m_nLodIndex; // -1 - without LOD model
        CEntity *m_pLod;
    };
    unsigned char m_nNumLodChildren;
    unsigned char m_nNumLodChildrenRendered;
    unsigned char m_nType : 3; // see eEntityType
    unsigned char m_nStatus : 5; // see eEntityStatus
    
};

typedef struct CQuaternion CQuaternion;

struct CQuaternion {
  CVector imag;
  float real;
};

typedef struct CPhysical CPhysical;

struct CPhysical {
    CEntity entity;
    int field_38;
    unsigned int m_nLastCollisionTime;
    struct {
        unsigned int b01 : 1;
        unsigned int bApplyGravity : 1;
        unsigned int bDisableCollisionForce : 1;
        unsigned int bCollidable : 1;
        unsigned int bDisableTurnForce : 1;
        unsigned int bDisableMoveForce : 1;
        unsigned int bInfiniteMass : 1;
        unsigned int bDisableZ : 1;

        unsigned int bSubmergedInWater : 1;
        unsigned int bOnSolidSurface : 1;
        unsigned int bBroken : 1;
        unsigned int b12 : 1; // ref @ 0x6F5CF0
        unsigned int b13 : 1;
        unsigned int bDontApplySpeed : 1;
        unsigned int b15 : 1;
        unsigned int b16 : 1;

        unsigned int b17 : 1;
        unsigned int b18 : 1; // ref @ CPhysical::ProcessCollision
        unsigned int bBulletProof : 1;
        unsigned int bFireProof : 1;
        unsigned int bCollisionProof : 1;
        unsigned int bMeleeProof : 1;
        unsigned int bInvulnerable : 1;
        unsigned int bExplosionProof : 1;

        unsigned int b25 : 1;
        unsigned int bAttachedToEntity : 1;
        unsigned int b27 : 1;
        unsigned int bTouchingWater : 1;
        unsigned int bCanBeCollidedWith : 1;
        unsigned int bDestroyed : 1;
        unsigned int b31 : 1;
        unsigned int b32 : 1;
    } m_nPhysicalFlags;
    CVector          m_vecMoveSpeed;
    CVector          m_vecTurnSpeed;
    CVector          m_vecFrictionMoveSpeed;
    CVector          m_vecFrictionTurnSpeed;
    CVector          m_vecForce;
    CVector          m_vecTorque;
    float            m_fMass;
    float            m_fTurnMass;
    float            m_fVelocityFrequency;
    float            m_fAirResistance;
    float            m_fElasticity;
    float            m_fBuoyancyConstant;
    CVector          m_vecCentreOfMass;
    void            *m_pCollisionList;
    void            *m_pMovingList;
    char field_B8;
    unsigned char    m_nNumEntitiesCollided;
    unsigned char    m_nContactSurface;
    char field_BB;
    CEntity         *m_apCollidedEntities[6];
    float            m_fMovingSpeed; // ref @ CTheScripts::IsVehicleStopped
    float            m_fDamageIntensity;
    CEntity         *m_pDamageEntity;
    CVector          m_vecLastCollisionImpactVelocity;
    CVector          m_vecLastCollisionPosn;
    unsigned short   m_nPieceType;
    short field_FA;
    CPhysical       *m_pAttachedTo;
    CVector          m_vecAttachOffset;
    CVector          m_vecAttachedEntityPosn;
    CQuaternion      m_qAttachedEntityRotation;
    CEntity         *m_pEntityIgnoredCollision;
    float            m_fContactSurfaceBrightness;
    float            m_fDynamicLighting;
    void *m_pShadowData;
};

typedef struct 
{
  CVector min;
  CVector max;
} CBoundingBox;

typedef struct
{
  CVector center;
  float radius;
} CBoundingSphere;

typedef struct
{
  short x;
  short y;
  short z;
} CCompressedVector;

typedef struct
{
  short a;
  short b;
  short c;
  char material;
  char light;
} CColTriangle;

typedef struct 
{
  short numSpheres;
  short numBoxes;
  short numTriangles;
  char numLines;
  char flags;
  int spheres;
  int boxes;
  int lines;
  CCompressedVector *vertices;
  CColTriangle* triangles;
  int trianglePlanes;
  int numShadowTriangles;
  int numShadowVertices;
  int shadowVertices;
  int shadowTriangles;
} CCollisionData;

typedef struct 
{
  CBoundingBox box;
  CBoundingSphere sphere;
  char flags;
  char allocationFlag;
  CCollisionData *colData;
} CColModel;

typedef struct {
    CVector       m_vecStart;
    CVector       m_vecEnd;
    bool          m_bExists;
    char _pad19[3];
    int           m_nCreationTime;
    unsigned int  m_nLifeTime;
    float         m_fRadius;
    unsigned char m_nTransparency;
    char _pad29[3];
} CBulletTrace;

extern CBulletTrace* CBulletTrace_aTraces;

typedef struct 
{
  uint32_t CrimeType;
  int CrimeID;
  uint32_t TimeOfQing;
  CVector Coors;
  char bAlreadyReported;
  char bPoliceDontReallyCare;
  char _padding[2];
} CCrimeBeingQd;

typedef struct {
	unsigned int    m_nChaosLevel;
	unsigned int    m_nChaosLevelBeforeParole;
	unsigned int    m_nLastTimeWantedDecreased;
	unsigned int    m_nLastTimeWantedLevelChanged;
	unsigned int    m_nTimeOfParole;
	float           m_fMultiplier; // New crimes have their wanted level contribution multiplied by this
	unsigned char   m_nCopsInPursuit;
	unsigned char   m_nMaxCopsInPursuit;
	unsigned char   m_nMaxCopCarsInPursuit;
	unsigned char   m_nCopsBeatingSuspect;
	unsigned short  m_nChanceOnRoadBlock;
	unsigned char   m_bPoliceBackOff : 1;       // If this is set the police will leave player alone (for cut-scenes)
    unsigned char   m_bPoliceBackOffGarage : 1; // If this is set the police will leave player alone (for garages)
    unsigned char   m_bEverybodyBackOff : 1;    // If this is set then everybody (including police) will leave the player alone (for cut-scenes)
    unsigned char   m_bSwatRequired : 1;        // These three booleans are needed so that the
    unsigned char   m_bFbiRequired : 1;         // streaming required vehicle stuff can be overrided
    unsigned char   m_bArmyRequired : 1;
	char _pad1F;
	unsigned int    m_nCurrentChaseTime;
	unsigned int    m_nCurrentChaseTimeCounter;
	unsigned int    m_nTimeCounting;
	unsigned int    m_nWantedLevel;
	unsigned int    m_nWantedLevelBeforeParole;
	CCrimeBeingQd   m_CrimesBeingQd[16];
	void        *m_pCopsInPursuit[10];
  /*CAEPoliceScannerAudioEntity m_PoliceScannerAudio;
    bool            m_bLeavePlayerAlone;
    char _pad299[3];*/

} CWanted;

enum PLAYER_FLAG
{
  StoppedMoving = 0x1,
  Adrenaline = 0x2,
  HaveTargetSelected = 0x4,
  FreeAiming = 0x8,
  CanBeDamaged = 0x10,
  AllMeleeAttackPtsBlocked = 0x20,
  JustBeenSnacking = 0x40,
  RequireHandleBreath = 0x80,
  GroupStuffDisabled = 0x100,
  GroupAlwaysFollow = 0x200,
  GroupNeverFollow = 0x400,
  InVehicleDontAllowWeaponChange = 0x800,
  RenderWeapon = 0x1000,
};

typedef struct 
{
  CWanted *pWanted;
  void *pClothesInfo;
  void *m_ArrestingOfficer;
  uint8_t gapC[8];
  float m_fMoveBlendRatio;
  float m_fSprintEnergy;
  float m_fSprintControlCounter;
  unsigned __int8 m_nChosenWeapon;
  char m_nCarDangerCounter;
  short __pad1;
  int m_nStandStillTimer;
  int m_nHitAnimDelayTimer;
  float m_fAttackButtonCounter;
  CPhysical *m_pDangerCar;
  uint16_t Flags;
  short __pad2;
  int m_PlayerGroup;
  int m_AdrenalineEndTime;
  char m_nDrunkenness;
  char m_bFadeDrunkenness;
  char m_nDrugLevel;
  char m_nScriptLimitToGangSize;
  float m_fBreath;
  int m_MeleeWeaponAnimReferenced;
  int m_MeleeWeaponAnimReferencedExtra;
  float m_fFPSMoveHeading;
  int m_fLookPitch;
  int m_fSkateBoardSpeed;
  int m_fSkateBoardLean;
  int m_pSpecialAtomic;
  float m_fGunSpinSpeed;
  int m_fGunSpinAngle;
  int m_LastTimeFiring;
  int m_nTargetBone;
  CVector m_vecTargetBoneOffset;
  int m_busFaresCollected;
  char m_bPlayerSprintDisabled;
  char m_bDontAllowWeaponChange;
  char m_bForceInteriorLighting;
  char __pad3;
  short m_DPadDownPressedInMilliseconds;
  short m_DPadUpPressedInMilliseconds;
  char m_Wetness;
  char m_PlayersGangActive;
  char m_WaterCoverPerc;
  char __pad4;
  float m_waterHeight;
  int m_FireHSMissilePressedTime;
  CEntity *m_LastHSMissileTarget;
  short m_nModelIndexOfLastBuildingShot;
  int m_LastHSMissileLOSTime;
  void *m_pCurrentProstitutePed;
  void *m_pLastProstituteShagged;
} CPlayerData;


typedef struct
{
    float left;          // x1
	float bottom;        // y1
	float right;         // x2
	float top;           // y2
} CRect;

typedef struct
{
    char           m_anUsedCarColors[64];
    unsigned char  m_nType; // see eMenuType
    char           m_aaacRowTitles[4][12][10];
    char _pad1[3];
    int            m_aadwNumberInRowTitle[4][12];
    int            m_aadw2ndNumberInRowTitle[4][12];
    char           m_aacColumnHeaders[4][10];
    char           m_acTitle[10];
    bool           m_abRowSelectable[12];
    bool           m_abRowAlreadyBought[12];
    unsigned char  m_anColumnAlignment[4]; // see eFontAlignment
    unsigned char  m_anColumnHeaderAlignment[4]; // see eFontAlignment
    char           m_nNumRows;
    char           m_nNumColumns;
    bool           m_abColumnInteractive[4];
    float          m_afColumnWidth[4];
    CVector2D      m_vPosn;
    bool           m_bColumnBackground;
    char           m_nSelectedRow;
    char           m_nAcceptedRow;
    char           _pad2;
} tMenuPanel;



#define MAX_NUM_BULLETTRACES 16

extern void (*CMatrix_Attach)(CVector *, uint32_t, CVector*);
extern void (*sub_25F980)(CVector *, uint32_t, CVector*);

CVector TransformFromObjectSpace(uint32_t entity, CVector offset);

extern CColModel *(*CEntity_GetColModel)(uint32_t);

extern unsigned int FrontEndMenuManager;

extern uint32_t* CTimer_m_snTimeInMilliseconds;

extern uint32_t* CTimer_m_FrameCounter;

extern uint32_t* CTimer_ms_fTimeStep;

extern float* CTimer_ms_fTimeScale;

extern uint32_t* CGame_currArea;

extern float* CWeather_Wind;

extern float (*CWorld_FindGroundZFor3DCoord)(float x, float y, float z, int *b, uint32_t **ent);

extern CWanted* (*FindPlayerWanted)(int);
extern int (*IsPlayer)(void* ped);
extern uint32_t* (*FindPlayerVehicle)(int, int);

CVector* getVehCoordinates(uint32_t* veh);

CVector* getCharCoordinates(uint32_t* ped);

extern unsigned int (*FindPlayerPed)(int playerIndex);

extern void (*FindPlayerCoors)(CVector *outPoint, int playerIndex);

float DistanceBetweenPoints(CVector* v1, CVector* v2);

#endif
