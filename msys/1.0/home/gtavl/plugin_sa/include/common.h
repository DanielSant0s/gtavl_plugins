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

enum eEntityType
{
	ENTITY_TYPE_NOTHING,
	ENTITY_TYPE_BUILDING,
	ENTITY_TYPE_VEHICLE,
	ENTITY_TYPE_PED,
	ENTITY_TYPE_OBJECT,
	ENTITY_TYPE_DUMMY,
	ENTITY_TYPE_NOTINPOOLS
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


enum eObjectType {
    OBJECT_MISSION = 2,
    OBJECT_TEMPORARY = 3,
    OBJECT_MISSION2 = 6
};

typedef struct {
    CPhysical parent;
    void           *m_pControlCodeList;
    unsigned char   m_nObjectType; // see enum eObjectType
    unsigned char   m_nBonusValue;
    unsigned short  m_wCostValue;
    struct {
        unsigned int b01 : 1;
        unsigned int b02 : 1;
        unsigned int bPickupPropertyForSale : 1;
        unsigned int bPickupInShopOutOfStock : 1;
        unsigned int bGlassBroken : 1;
        unsigned int b06 : 1;
        unsigned int bIsExploded : 1;
        unsigned int b08 : 1;

        unsigned int bIsLampPost : 1;
        unsigned int bIsTargatable : 1;
        unsigned int bIsBroken : 1;
        unsigned int bTrainCrossEnabled : 1;
        unsigned int bIsPhotographed : 1;
        unsigned int bIsLiftable : 1;
        unsigned int bIsDoorMoving : 1;
        unsigned int bbIsDoorOpen : 1;

        unsigned int bHasNoModel : 1;
        unsigned int bIsScaled : 1;
        unsigned int bCanBeAttachedToMagnet : 1;
        unsigned int b20 : 1;
        unsigned int b21 : 1;
        unsigned int b22 : 1;
        unsigned int bFadingIn : 1; // works only for objects with type 2 (OBJECT_MISSION)
        unsigned int bAffectedByColBrightness : 1;

        unsigned int b25 : 1;
        unsigned int bDoNotRender : 1;
        unsigned int bFadingIn2 : 1;
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int b30 : 1;
        unsigned int b31 : 1;
        unsigned int b32 : 1;
    } m_nObjectFlags;
    unsigned char   m_nColDamageEffect;
    unsigned char   m_nStoredColDamageEffect;
    char field_146;
    char            m_nGarageDoorGarageIndex;
    unsigned char   m_nLastWeaponDamage;
    unsigned char   m_nDayBrightness : 4;
    unsigned char   m_nNightBrightness : 4;
    short           m_nRefModelIndex;
    unsigned char   m_nCarColor[4]; // this is used for detached car parts
    int             m_dwRemovalTime; // time when this object must be deleted
    float           m_fHealth;
    float           m_fDoorStartAngle; // this is used for door objects
    float           m_fScale;
    void           *m_pObjectInfo;
    void           *m_pFire; // CFire *
    short           m_wScriptTriggerIndex;
    short           m_wRemapTxd; // this is used for detached car parts
    RwTexture      *m_pRemapTexture; // this is used for detached car parts
    void           *m_pDummyObject; // used for dynamic objects like garage doors, train crossings etc.
    int             m_dwBurnTime; // time when particles must be stopped
    float           m_fBurnDamage;
} CObject;


typedef struct {
  CObject object;
} CProjectile;

typedef struct {
	unsigned int  m_nWeaponType; // see eWeaponType
	CEntity      *m_pCreator;
	CEntity      *m_pVictim;
	int           m_nDestroyTime;
	bool          m_bActive;
	char _pad11[3];
	CVector       m_vecLastPosn;
	void         *m_pFxSystem;
} CProjectileInfo;

extern CProjectile *ms_apProjectile[32]; // static CProjectile *ms_apProjectile[MAX_PROJECTILES]
extern CProjectileInfo gaProjectileInfo[32];

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
	CCompressedVector normal;
	unsigned short m_nDistance;
	unsigned char m_nOrientation;
} CColTrianglePlane;

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
  CColTrianglePlane *trianglePlanes;
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

extern CBulletTrace CBulletTrace_aTraces[];

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

void CMatrix_Attach(CVector *a1, uint32_t a2, CVector *a3);
void sub_25F980(CVector *a1, uint32_t a2, CVector *a3);

CColModel* CEntity_GetColModel(uint32_t entity);

float CWorld_FindGroundZFor3DCoord(float x, float y, float z, int *b, uint32_t **ent);

CWanted* FindPlayerWanted(int playerIndex);
int IsPlayer(void* ped);
uint32_t* FindPlayerVehicle(int playerIndex, int index);

unsigned int FindPlayerPed(int playerIndex);

void FindPlayerCoors(CVector *outPoint, int playerIndex);

CVector TransformFromObjectSpace(uint32_t entity, CVector offset);

CVector* getVehCoordinates(uint32_t* veh);

CVector* getCharCoordinates(uint32_t* ped);

float DistanceBetweenPoints(CVector* v1, CVector* v2);

void CStreaming_IHaveUsedStreamingMemory(void);
void CStreaming_ImGonnaUseStreamingMemory(void);
void CStreaming_MakeSpaceFor(int);
void CGame_TidyUpMemory(bool, bool);

extern unsigned int FrontEndMenuManager;

extern uint32_t CTimer_m_snTimeInMilliseconds;

extern uint32_t CTimer_m_FrameCounter;

extern float CTimer_ms_fTimeStep;

extern float CTimer_ms_fTimeScale;

extern uint32_t CGame_currArea;

extern float CWeather_Wind;

typedef struct HeapBlockDesc HeapBlockDesc;

struct HeapBlockDesc {
  uint32_t m_size;
	int16_t m_memId;
	int16_t m_ptrListIndex;
	HeapBlockDesc *m_next;
	HeapBlockDesc *m_prev;
};



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

char CPed_GetWeaponSkill(void* ped);

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

CWeaponInfo *CWeaponInfo_GetWeaponInfo(uint32_t weaponType, unsigned char skill);
float RsTimer(int);

CMatrix * CPlaceable_AllocateMatrix(CPlaceable *this);
void CSimpleTransform_UpdateMatrix(CSimpleTransform *this, CMatrix *matrix);

CMatrix * CPlaceable_GetMatrix(CPlaceable *this);


typedef struct
{
    unsigned char day : 4;
    unsigned char night : 4;
} tColLighting;

typedef struct {
	  CVector       m_vecPoint;
	  float field_C;
	  CVector       m_vecNormal;
	  float field_1C;
      // col shape 1 info
	  unsigned char m_nSurfaceTypeA; // see eSurfaceType
	  unsigned char m_nPieceTypeA;
    tColLighting m_nLightingA;

    char _pad;

    // col shape 2 info
	  unsigned char m_nSurfaceTypeB; // see eSurfaceType
	  unsigned char m_nPieceTypeB;
    tColLighting m_nLightingB;

    char _pad2;

    // col depth
	  float         m_fDepth;
} CColPoint;

extern uint32_t gFireManager;

void* StartFireEntity(uint32_t *this, CEntity* target, CEntity* creator, float size, uint8_t arg3, uint32_t time, char numGenerations);
uint32_t GetNumFiresInRange(uint32_t *this, CVector* point, float fRadiusSq);

typedef struct {
    float          m_fAmbientRed;
    float          m_fAmbientGreen;
    float          m_fAmbientBlue;
    float          m_fAmbientRed_Obj;
    float          m_fAmbientGreen_Obj;
    float          m_fAmbientBlue_Obj;
    unsigned short m_nSkyTopRed;
    unsigned short m_nSkyTopGreen;
    unsigned short m_nSkyTopBlue;
    unsigned short m_nSkyBottomRed;
    unsigned short m_nSkyBottomGreen;
    unsigned short m_nSkyBottomBlue;
    unsigned short m_nSunCoreRed;
    unsigned short m_nSunCoreGreen;
    unsigned short m_nSunCoreBlue;
    unsigned short m_nSunCoronaRed;
    unsigned short m_nSunCoronaGreen;
    unsigned short m_nSunCoronaBlue;
    float          m_fSunSize;
    float          m_fSpriteSize;
    float          m_fSpriteBrightness;
    unsigned short m_nShadowStrength;
    unsigned short m_nLightShadowStrength;
    unsigned short m_nPoleShadowStrength;
    float          m_fFarClip;
    float          m_fFogStart;
    float          m_fLightsOnGroundBrightness;
    unsigned short m_nLowCloudsRed;
    unsigned short m_nLowCloudsGreen;
    unsigned short m_nLowCloudsBlue;
    unsigned short m_nFluffyCloudsBottomRed;
    unsigned short m_nFluffyCloudsBottomGreen;
    unsigned short m_nFluffyCloudsBottomBlue;
    float          m_fWaterRed;
    float          m_fWaterGreen;
    float          m_fWaterBlue;
    float          m_fWaterAlpha;
    float          m_fPostFx1Red;
    float          m_fPostFx1Green;
    float          m_fPostFx1Blue;
    float          m_fPostFx1Alpha;
    float          m_fPostFx2Red;
    float          m_fPostFx2Green;
    float          m_fPostFx2Blue;
    float          m_fPostFx2Alpha;
    float          m_fCloudAlpha;
    unsigned int   m_nHighLightMinIntensity;
    unsigned short m_nWaterFogAlpha;
    float          m_fIllumination;
    float          m_fLodDistMult;

} CColourSet;

extern uint32_t CModelInfo_ms_modelInfoPtrs[];

void CShadows_StoreShadowToBeRenderedSingle(uint8_t type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, uint8_t red, uint8_t green, uint8_t blue, float zDistance, bool drawOnWater, float scale, void* realTimeShadow, bool drawOnBuildings);


#endif
