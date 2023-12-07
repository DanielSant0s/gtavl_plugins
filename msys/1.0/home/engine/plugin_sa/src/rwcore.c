#include <rwcore.h>

float* RecipNearClip = (float*)0x66BCEC;
float* NearScreenZ = (float*)0x66BCF0;


void (*psCameraShowRaster)(void*) = (void (*)(void*))0x33D8D8;
void (*RwIm2DRenderPrimitive)(RwInt32, RwIm2DVertex*, RwInt32) = (void (*)(RwInt32, RwIm2DVertex*, RwInt32))0x3669E8;
int (*RwRenderStateSet)(RwInt32, void*) = (int (*)(RwInt32, void*))0x34FD68;
void (*RwIm3DRenderIndexedPrimitive)(int, RwImVertexIndex*, RwInt32) = (void (*)(int, RwImVertexIndex*, RwInt32))0x369B98;
void  *(*RwIm3DTransform)(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags) = (void  *(*)(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags))0x36A280;
void (*RwIm3DEnd)() = (void (*)())0x36A328;
void (*RwIm2DRenderIndexedPrimitive)(int, RwIm2DVertex*, RwInt32, RwImVertexIndex*, RwInt32) = (void (*)(int, RwIm2DVertex*, RwInt32,  RwImVertexIndex*, RwInt32))0x366A58;
RwRaster* (*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) = (RwRaster* (*)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags))0x342DA0;
int (*RwRasterDestroy)(RwRaster * raster) = (int (*)(RwRaster * raster))0x342EA0;
RwRaster* (*RwRasterSubRaster)(RwRaster* subRaster, RwRaster* raster, RwRect* rect) = (RwRaster* (*)(RwRaster* subRaster, RwRaster* raster, RwRect* rect))0x342F78;
RwUInt8 *(*RwRasterLock)(RwRaster * raster, RwUInt8 level, RwInt32 lockMode) = (RwUInt8 *(*)(RwRaster * raster, RwUInt8 level, RwInt32 lockMode))0x3432F0;
RwRaster *(*RwRasterUnlock)(RwRaster * raster) = (RwRaster *(*)(RwRaster * raster))0x343330;
RwTexture* (*RwTextureCreate)(RwRaster * raster) = (RwTexture* (*)(RwRaster * raster))0x346418;
int (*RwTextureDestroy)(RwTexture * texture) = (int (*)(RwTexture * texture))0x3464A0;

void (*RpSkyTexCacheRasterLock)(RwRaster *raster, int bLocked) = (void (*)(RwRaster *raster, int bLocked))0x368EE8;

RwFileFunction* (*RwOsGetFileInterface)(void)  = (RwFileFunction* (*)(void))0x3332B8;

void (*RwRasterPushContext)(RwRaster* raster) = (void (*)(RwRaster* raster))0x3430D0;
void (*RwRasterPopContext)() = (void (*)())0x343148;

RwRaster *(*RwRasterRenderFast)(RwRaster * raster, RwInt32 x, RwInt32 y) = (RwRaster *(*)(RwRaster * raster, RwInt32 x, RwInt32 y))0x343018;
RwRaster *(*RwRasterRender)(RwRaster * raster, RwInt32 x, RwInt32 y) = (RwRaster *(*)(RwRaster * raster, RwInt32 x, RwInt32 y))0x343058;
RwRaster *(*RwRasterRenderScaled)(RwRaster * raster, RwRect * rect) = (RwRaster *(*)(RwRaster * raster, RwRect * rect))0x343098;

void *(*RwCameraBeginUpdate)(void * camera) = (void *(*)(void * camera))0x33D860;
void *(*RwCameraEndUpdate)(void * camera) = (void *(*)(void * camera))0x33D880;

RwRaster *(*RpSkyTexSetTex0)(RwRaster *r, RwUInt32 msb, RwUInt32 lsb) = (RwRaster *(*)(RwRaster *r, RwUInt32 msb, RwUInt32 lsb))0x3696D8;

RwRaster *(*RpSkyTexGetTex0)(RwRaster *r, RwUInt32 *msb, RwUInt32 *lsb) = (RwRaster *(*)(RwRaster *r, RwUInt32 *msb, RwUInt32 *lsb))0x3696F0;

int (*_rwDMAOpenGIFPkt)(RwUInt32, RwUInt32) = (int (*)(RwUInt32, RwUInt32))0x34E378;
int (*_rwDMAOpenVIFPkt)(RwUInt32, RwUInt32) = (int (*)(RwUInt32, RwUInt32))0x34E878;

RwRaster *	(*RpSkyGetDisplayBufferRaster)(void) = (RwRaster *	(*)(void))0x35AF28;
RwRaster *	(*RpSkyGetDrawBufferRaster)(void) = (RwRaster *	(*)(void))0x35AF30;

uint32_t* skyFrameBit = (uint32_t*)0x66797C;

int (*sceGsSyncV)(int mode) = (int (*)(int mode))0x548918;
void (*sceGsResetGraph)(short mode, short inter, short omode, short ffmode) = (void (*)(short mode, short inter, short omode, short ffmode))0x547A90;
int (*sceGsSwapDBuffDc)(void *db, int id) = (int (*)(void *db, int id))0x549A68;
int (*sceGsSyncPath)(int mode, uint16_t timeout) = (int (*)(int mode, uint16_t timeout))0x5489B0;
void *(*sceDmaGetChan)(int id) = (void *(*)(int id))0x549C20;
void (*sceDmaSendN)(void *d, void *addr, int size) = (void (*)(void *d, void *addr, int size))0x549FB8;

void (*setInitialRenderState)(void) = (void (*)(void))0x357BA0;

void (*RpSkyTexCacheFlush)(void) = (void (*)(void))0x3691C0;

bool (*_rwDMAHook)() = (bool (*)())0x34F2F0;

bool* skySuspended = (bool*)0x66791C;

void *_rwDMAFlipData = (void*)0x8298A0;

void* skyInitialClear = (void*)0x82A200;

RwInt32 (*RwEngineGetNumVideoModes)(void) = (RwInt32 (*)(void))0x33E268;
RwVideoMode *(*RwEngineGetVideoModeInfo)(RwVideoMode *modeinfo, RwInt32 modeIndex) = (RwVideoMode *(*)(RwVideoMode *modeinfo, RwInt32 modeIndex))0x33EBF8;
RwInt32 (*RwEngineGetCurrentVideoMode)(void) = (RwInt32 (*)(void))0x33E2D8;
bool (*RwEngineSetVideoMode)(RwInt32 modeIndex) = (bool (*)(RwInt32 modeIndex))0x33E348;

void (*RsRwInitialize)(void*) = (void (*)(void*))0x39B510;
void (*RsRwTerminate)(void) = (void (*)(void))0x39B4E0;

void (*SkyStart)() = (void (*)())0x358B70;
uint32_t *currentMode = (uint32_t*)0x66C228;

void (*RpSkySuspend)() = (void (*)())0x35AB38;

void (*sceGsSetDefDBuffDc)(void *db, short psm, short w, short h, short ztest, short zpsm, short clear) = 
(void (*)(void *db, short psm, short w, short h, short ztest, short zpsm, short clear))0x548620;



