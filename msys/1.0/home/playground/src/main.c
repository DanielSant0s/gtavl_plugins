/*
    Plugin-SDK (Grand Theft Auto) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include <stdio.h>
#include <math.h>
#include <injector.h>
#include <CVehicle.h>
#include <CRGBA.h>
#include <rwcore.h>
#include <secore.h>
#include <CRadar.h>
#include <CCam.h>
#include <CFont.h>
#include <CTxdStore.h>
#include <CPad.h>
#include <common.h>

void* Camera = (void*)0x66C2FC;
void (*psCameraShowRaster)(void*) = (void (*)(void*))0x33D8D8;
uint16_t* frameBuffer = (uint16_t*)0x6FEEE4;

/* Flag for sceOpen() */
#define SCE_RDONLY      0x0001
#define SCE_WRONLY      0x0002
#define SCE_RDWR        0x0003
#define SCE_NBLOCK      0x0010  /* Non-Blocking I/O */
#define SCE_APPEND      0x0100  /* append (writes guaranteed at the end) */
#define SCE_CREAT       0x0200  /* open with file create */
#define SCE_TRUNC       0x0400  /* open with truncation */
#define SCE_EXCL        0x0800  /* exclusive create */
#define SCE_NOBUF       0x4000  /* no device buffer and console interrupt */
#define SCE_NOWAIT      0x8000  /* asyncronous i/o */

int (*sceSifLoadModule)(char*, int, int) = (int (*)(char*, int, int))0x005449A0;

int (*open)(const char *pathname, int flags) = (int (*)(const char *, int))0x00540B28;
int (*close)(int fd) = (int (*)(int))0x00540DB8;
size_t (*lseek)(int fd, size_t offset, int whence) = (size_t (*)(int, size_t, int))0x00540F30;
size_t (*read)(int fd, void *buf, size_t count) = (size_t (*)(int, void *, size_t))0x00541170;
size_t (*write)(int fd, const void *buf, size_t count) = (size_t (*)(int, const void *, size_t))0x005413E0;
int (*chdir)(const char *name) = (int (*)(const char *))0x00542D20;

static RwRGBA font_color;
static RwTexture* screenTex256;

void (*CLoadingScreen_LoadSplashes)() = (void (*)())0x592B20;

void (*RwIm2DRenderPrimitive)(RwInt32, RwIm2DVertex*, RwInt32) = (void (*)(RwInt32, RwIm2DVertex*, RwInt32))0x3669E8;
int (*RwRenderStateSet)(RwInt32, void*) = (int (*)(RwInt32, void*))0x34FD68;

typedef struct
{
  CVector pos;
  RwRGBA color;
  CVector objNormal;
  float u;
  float v;
} RxObjSpace3dVertex;

typedef RwUInt16        RxVertexIndex;
typedef RxVertexIndex   RwImVertexIndex;

RxObjSpace3dVertex Temp3DVertexBuffer[4];
RwIm2DVertex Temp2DVertexBuffer[4];

int* TypeOfMirror = (int*)0x66C02C;
uint8_t* MirrorFlags = (uint8_t*)0x66C030;
uint8_t* bFudgeNow = (uint8_t*)0x66C038;

RwRaster** CMirrors_pBuffer = (RwRaster**)0x66C020;

void (*RwIm3DRenderIndexedPrimitive)(int, RwImVertexIndex*, RwInt32) = (void (*)(int, RwImVertexIndex*, RwInt32))0x369B98;
void  *(*RwIm3DTransform)(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags) = (void  *(*)(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags))0x36A280;
void (*RwIm3DEnd)() = (void (*)())0x36A328;
void (*DefinedState)() = (void (*)())0x1104B0;

void (*RwIm2DRenderIndexedPrimitive)(int, RwIm2DVertex*, RwInt32, RwImVertexIndex*, RwInt32) = (void (*)(int, RwIm2DVertex*, RwInt32,  RwImVertexIndex*, RwInt32))0x366A58;

static bool enable_mirror = false;

typedef struct RwRect RwRect;

struct RwRect
{
    RwInt32 x;  /**< X value of the top-left corner */
    RwInt32 y;  /**< Y value of the top-left corner */
    RwInt32 w;  /**< Width of the rectangle */
    RwInt32 h;  /**< Height of the rectangle */
};


RwRaster* (*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) = (RwRaster* (*)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags))0x342DA0;
int (*RwRasterDestroy)(RwRaster * raster) = (int (*)(RwRaster * raster))0x342EA0;

RwRaster* (*RwRasterSubRaster)(RwRaster* subRaster, RwRaster* raster, RwRect* rect) = (RwRaster* (*)(RwRaster* subRaster, RwRaster* raster, RwRect* rect))0x342F78;

RwTexture* (*RwTextureCreate)(RwRaster * raster) = (RwTexture* (*)(RwRaster * raster))0x346418;

void (*CSprite2d_DrawSprite)(CTexture *this, float posX, float posY, float width, float height, RwRGBA *colour) = 
(void (*)(CTexture *this, float posX, float posY, float width, float height, RwRGBA *colour))0x2B08D0;

int (*CCustomCarEnvMapPipeline__SetFxEnvTexture)(void *a1, RwTexture *a2) = (int (*)(void *a1, RwTexture *a2))0x507B90;

void create_screen_texture() {
    CLoadingScreen_LoadSplashes();

    RwRect rect;
    RwRaster* raster = *(RwRaster**)(*(uint32_t*)(Camera) + 96);
    rect.x = 0;
    rect.y = 0;
    rect.w = 256;
    rect.h = 256;

    RwRaster* dummy = RwRasterCreate(0, 0, 0, 0x85);
    screenTex256 = RwTextureCreate(RwRasterSubRaster(dummy, raster, &rect));
}

void SetEnvReflections(void *a1, RwTexture *a2) {
    CCustomCarEnvMapPipeline__SetFxEnvTexture(a1, screenTex256);
}

int _start()
{  
    RedirectCall(0x592548, create_screen_texture);
    RedirectFunction(0x3C8F00, SetEnvReflections);
    return 0;
}
