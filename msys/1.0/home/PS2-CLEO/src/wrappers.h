#ifndef PS2_WRAPPERS_H
#define PS2_WRAPPERS_H

#include <ctype.h>

extern "C" {

  typedef struct {
      float x;
      float y;
      float z;

  } CVector;

  typedef struct {
      unsigned int   m_nColour; // see eBlipColour
      unsigned int   m_nEntityHandle;
      CVector        m_vecPos;
      unsigned short m_nCounter;
      float          m_fSphereRadius;
      unsigned short m_nBlipSize;
      unsigned int  *m_pEntryExit;
      unsigned char  m_nRadarSprite; // see eRadarSprite
      unsigned char  m_bBright : 1; // It makes use of bright colors. Always set.
      unsigned char  m_bInUse : 1; // It is available.
      unsigned char  m_bShortRange : 1; // It doesn't show permanently on the radar.
      unsigned char  m_bFriendly : 1; // It is affected by BLIP_COLOUR_THREAT.   
      unsigned char  m_bBlipRemain : 1; // It has the priority over the entity (it will still appear after the entity's deletion).
      unsigned char  m_bBlipFade : 1; // Possibly a leftover. Always unset (unused).
      unsigned char  m_nCoordBlipAppearance : 2; // see eBlipAppearance
      unsigned char  m_nBlipDisplay : 2; // see eBlipDisplay
      unsigned char  m_nBlipType : 4; // see eBlipType
  } tRadarTrace;

  typedef struct {
      unsigned char bIsLawEnforcer : 1; // Is this guy chasing the player at the moment
      unsigned char bIsAmbulanceOnDuty : 1; // Ambulance trying to get to an accident
      unsigned char bIsFireTruckOnDuty : 1; // Firetruck trying to get to a fire
      unsigned char bIsLocked : 1; // Is this guy locked by the script (cannot be removed)
      unsigned char bEngineOn : 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
      unsigned char bIsHandbrakeOn : 1; // How's the handbrake doing ?
      unsigned char bLightsOn : 1; // Are the lights switched on ?
      unsigned char bFreebies : 1; // Any freebies left in this vehicle ?
      unsigned char bIsVan : 1; // Is this vehicle a van (doors at back of vehicle)
      unsigned char bIsBus : 1; // Is this vehicle a bus
      unsigned char bIsBig : 1; // Is this vehicle a bus
      unsigned char bLowVehicle : 1; // Need this for sporty type cars to use low getting-in/out anims
      unsigned char bComedyControls : 1; // Will make the car hard to control (hopefully in a funny way)
      unsigned char bWarnedPeds : 1; // Has scan and warn peds of danger been processed?
      unsigned char bCraneMessageDone : 1; // A crane message has been printed for this car allready
      unsigned char bTakeLessDamage : 1; // This vehicle is stronger (takes about 1/4 of damage)
      unsigned char bIsDamaged : 1; // This vehicle has been damaged and is displaying all its components
      unsigned char bHasBeenOwnedByPlayer : 1;// To work out whether stealing it is a crime
      unsigned char bFadeOut : 1; // Fade vehicle out
      unsigned char bIsBeingCarJacked : 1; // Fade vehicle out
      unsigned char bCreateRoadBlockPeds : 1;// If this vehicle gets close enough we will create peds (coppers or gang members) round it
      unsigned char bCanBeDamaged : 1; // Set to FALSE during cut scenes to avoid explosions
      unsigned char bOccupantsHaveBeenGenerated : 1; // Is true if the occupants have already been generated. (Shouldn't happen again)
      unsigned char bGunSwitchedOff : 1; // Level designers can use this to switch off guns on boats
      unsigned char bVehicleColProcessed : 1;// Has ProcessEntityCollision been processed for this car?
      unsigned char bIsCarParkVehicle : 1; // Car has been created using the special CAR_PARK script command
      unsigned char bHasAlreadyBeenRecorded : 1; // Used for replays
      unsigned char bPartOfConvoy : 1;
      unsigned char bHeliMinimumTilt : 1; // This heli should have almost no tilt really
      unsigned char bAudioChangingGear : 1; // sounds like vehicle is changing gear
      unsigned char bIsDrowning : 1; // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
      unsigned char bTyresDontBurst : 1; // If this is set the tyres are invincible
      unsigned char bCreatedAsPoliceVehicle : 1;// True if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
      unsigned char bRestingOnPhysical : 1; // Dont go static cause car is sitting on a physical object that might get removed
      unsigned char bParking : 1;
      unsigned char bCanPark : 1;
      unsigned char bFireGun : 1; // Does the ai of this vehicle want to fire it's gun?
      unsigned char bDriverLastFrame : 1; // Was there a driver present last frame ?
      unsigned char bNeverUseSmallerRemovalRange : 1;// Some vehicles (like planes) we don't want to remove just behind the camera.
      unsigned char bIsRCVehicle : 1; // Is this a remote controlled (small) vehicle. True whether the player or AI controls it.
      unsigned char bAlwaysSkidMarks : 1; // This vehicle leaves skidmarks regardless of the wheels' states.
      unsigned char bEngineBroken : 1; // Engine doesn't work. Player can get in but the vehicle won't drive
      unsigned char bVehicleCanBeTargetted : 1;// The ped driving this vehicle can be targetted, (for Torenos plane mission)
      unsigned char bPartOfAttackWave : 1; // This car is used in an attack during a gang war
      unsigned char bWinchCanPickMeUp : 1; // This car cannot be picked up by any ropes.
      unsigned char bImpounded : 1; // Has this vehicle been in a police impounding garage
      unsigned char bVehicleCanBeTargettedByHS : 1;// Heat seeking missiles will not target this vehicle.
      unsigned char bSirenOrAlarm : 1; // Set to TRUE if siren or alarm active, else FALSE

      unsigned char bHasGangLeaningOn : 1;
      unsigned char bGangMembersForRoadBlock : 1;// Will generate gang members if NumPedsForRoadBlock > 0
      unsigned char bDoesProvideCover : 1; // If this is false this particular vehicle can not be used to take cover behind.
      unsigned char bMadDriver : 1; // This vehicle is driving like a lunatic
      unsigned char bUpgradedStereo : 1; // This vehicle has an upgraded stereo
      unsigned char bConsideredByPlayer : 1; // This vehicle is considered by the player to enter
      unsigned char bPetrolTankIsWeakPoint : 1;// If false shootong the petrol tank will NOT Blow up the car
      unsigned char bDisableParticles : 1; // Disable particles from this car. Used in garage.

      unsigned char bHasBeenResprayed : 1; // Has been resprayed in a respray garage. Reset after it has been checked.
      unsigned char bUseCarCheats : 1; // If this is true will set the car cheat stuff up in ProcessControl()
      unsigned char bDontSetColourWhenRemapping : 1;// If the texture gets remapped we don't want to change the colour with it.
      unsigned char bUsedForReplay : 1; // This car is controlled by replay and should be removed when replay is done.
  } m_nVehicleFlags;

  #define NULL (void*)0

  #define getCurrentGear(veh)  *(uint8_t*)(veh + 0x4BC)
  #define getVehicleSubClass(veh) *(uint32_t*)(veh + 0x5D0)
  #define getVehicleFlags(veh) ((m_nVehicleFlags*)((uint32_t)veh + 1072))
  #define attachedTo(entity) *(uint32_t*)((uint32_t)entity + 0xFC)
  #define getVehicleBombLightsWinchFlags(veh) *(uint8_t*)((uint32_t)veh + 1200)
  #define setVehicleBombLightsWinchFlags(veh, val) *(uint8_t*)((uint32_t)veh + 1200) = val
  #define getVehicleHealth(veh) *(float*)((uint32_t)veh + 1224)

  #define sizeof_CPed 2064
  #define sizeof_CVehicle 2688
  #define sizeof_CObject 412

  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;
  typedef long int int64_t;
  typedef unsigned long int uint64_t;

  typedef uint8_t BYTE;
  typedef uint16_t WORD;
  typedef uint32_t DWORD;
  typedef uint64_t QWORD;

  typedef DWORD size_t;

  #define SEEK_SET 0
  #define SEEK_CUR 1
  #define SEEK_END 2

  typedef int FILE;

  #define O_RDONLY 0x0001

  typedef struct RwFileFunction{
  	int padding;  
  	FILE* (*rwfopen)(const char*, const char*);
  	int (*rwfclose)(FILE *);
  	size_t (*rwfread)(FILE *, void *, size_t);   
  	size_t (*rwfwrite)(FILE *, const void *, size_t);
  	int (*rwfgets)(int, int, int);
  	int (*rwfputs)(int, int); 
  	int (*rwfeof)(FILE*);   
  	int (*rwfseek)(FILE *, long int, int);  
  	int (*rwfflush)(int*);
  	int (*rwftell)(FILE*);  
  } RwFileFunction;

  RwFileFunction* RwOsGetFileInterface(void);

  void FlushCache(int);

  FILE* rw_open(const char*, const char*);
  size_t rw_read(FILE *, void *, size_t);
  int rw_seek(FILE *, long int, int);
  int rw_close(FILE *);
  char* loadLine(FILE *);
  int setDirectory(const char *);


  struct sce_stat {
          unsigned int    st_mode;       

          unsigned int    st_attr;       
          unsigned int    st_size;       
          unsigned char   st_ctime[8];   
          unsigned char   st_atime[8];   
          unsigned char   st_mtime[8];   
          unsigned int    st_hisize;     
          unsigned int    st_private[6]; 
  };

  struct sce_dirent {
          struct sce_stat d_stat; 
          char d_name[256];       
          void    *d_private;     
  };

  int sceDopen(const char *dirname);
  int sceDclose(int fd);
  int sceDread(int fd, struct sce_dirent *buf);
  int sceRemove(const char *filename);
  int sceMkdir(const char *dirname, int flag);
  int sceRmdir(const char *dirname);

  int scePrintf(const char *, ...);
  int printf(const char *, ...);
  int sscanf( const char * s, const char * format, ...);

  void* malloc(size_t);
  void free(void*);
  void* memset(void *, int, size_t);
  void* memcpy(void *, const void *, size_t);
  int memcmp(const void *, const void *, size_t);

  size_t strlen(const char *);
  int    strcmp(const char *, const char *);
  char*  strcpy(char *, const char *);
  char*  strcat(char *, const char *);
  char*  strncpy(char *, const char *, size_t);

  int sprintf(char *, const char *, ...);
  float floorf(float);
  float powf(float, float);
  int64_t __extendsfdf2(float);

  float logf(float x);

  #include "CTheScripts.h"

  int sceSifLoadModule(char*, int, int);

  extern size_t CTimer_m_snTimeInMilliseconds;
  extern BYTE CTheScripts_ScriptSpace[];
  extern bool CTheScripts_FailCurrentMission;
  extern int _StyledText_2;

  bool CCutsceneMgr_IsCutsceneSkipButtonBeingPressed();
  void CRunningScript_DoDeatharrestCheck(CRunningScript *);
  void CTheScripts_ReinitialiseSwitchStatementData() ;

 extern SCRIPT_VAR missionLocals[];

  extern SCRIPT_VAR opcodeParams[];
  void SetScriptParams(CRunningScript *, int);
  void GetScriptParams(CRunningScript *, int);
  void GetScriptStringParam(CRunningScript *, char *, uint8_t);
  SCRIPT_VAR* GetScriptParamPointer(CRunningScript *, int);

  void SetScriptCondResult(CRunningScript *, bool);

  void PrintHelp(char*, int, int, int);
  void PrintBig(const char *, int, uint16_t);
  void Print(const char *src, int, bool, bool);
  void PrintNow(const char *src, int, bool, bool);

  extern DWORD** pedPool;
  extern DWORD** vehPool;
  extern DWORD** objPool;

  union tPoolObjectFlags {
      struct {
          uint8_t nId : 7;
          uint8_t bEmpty : 1;
      };
      uint8_t nValue;
  };

  struct CPool
  {
    void *objects;
    tPoolObjectFlags *m_byteMap;
    int size;
    int top;
    char initialized;
    char field_11;
    short _pad;
  };


  typedef struct
  {
      /* These are padded to be 16 byte quantities per line */
      CVector             right;
      uint32_t            flags;
      CVector             up;
      uint32_t            pad1;
      CVector             at;
      uint32_t            pad2;
      CVector             pos;
      uint32_t            pad3;
  } RwMatrix;


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
    unsigned char m_nChosenWeapon;
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


  DWORD* GetPedStruct(DWORD **, int);
  DWORD* GetVehStruct(DWORD **, int);
  DWORD* GetObjStruct(DWORD **, int);

  int GetPedHandle(DWORD **, int);
  int GetVehHandle(DWORD **, int);
  int GetObjHandle(DWORD **, int);

  const char* CText_Get(DWORD, const char*);

  int IsPlayer(void* ped);

  extern DWORD gameTexts;

  int CTheScripts_GetActualScriptThingIndex(unsigned int a1, unsigned char a2);

  extern DWORD CModelInfo_ms_modelInfoPtrs[];

  void CMatrix_Attach(CVector *, uint32_t, CVector*);
  void sub_25F980(CVector *, CVector *, CVector*);

  void CVehicle_SetWindowOpenFlag(DWORD* veh, uint8_t door);
  void CVehicle_ClearWindowOpenFlag(DWORD* veh, uint8_t door);
  
  float CWorld_FindGroundZForCoord(float, float);
  
  extern tRadarTrace CRadar_ms_RadarTrace[];
  
  #define LOWORD(a) ((unsigned short)(a))
  
  extern unsigned int FrontEndMenuManager_m_nTargetBlipi;

enum
{
    VEHICLE_TYPE_AUTOMOBILE,
    VEHICLE_TYPE_PLANE,
    VEHICLE_TYPE_BIKE,
    VEHICLE_TYPE_HELI,
    VEHICLE_TYPE_BOAT,
    VEHICLE_TYPE_TRAIN,
    VEHICLE_TYPE_TRAILER,
    VEHICLE_TYPE_BMX,
    VEHICLE_TYPE_MTRUCK,
    VEHICLE_TYPE_QUAD,
    VEHICLE_TYPE_UNKNOWN = -1,
};

}



#endif