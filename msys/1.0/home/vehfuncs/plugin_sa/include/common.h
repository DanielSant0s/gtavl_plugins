#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
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

extern void* (*FindPlayerWanted)(int);
extern int (*IsPlayer)(void* ped);
extern uint32_t* (*FindPlayerVehicle)(int, int);

CVector* getVehCoordinates(uint32_t* veh);

CVector* getCharCoordinates(uint32_t* ped);

extern unsigned int (*FindPlayerPed)(int playerIndex);

extern void (*FindPlayerCoors)(CVector *outPoint, int playerIndex);

float DistanceBetweenPoints(CVector* v1, CVector* v2);


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

extern void (*CMatrix_Attach)(CVector *, uint32_t, CVector*);
extern void (*sub_25F980)(CVector *, uint32_t, CVector*);

CVector TransformFromObjectSpace(uint32_t entity, CVector offset);

extern unsigned int FrontEndMenuManager;

extern uint32_t* CTimer_m_snTimeInMilliseconds;

#endif
