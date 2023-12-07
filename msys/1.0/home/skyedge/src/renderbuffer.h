#include <rwcore.h>
#include <stdint.h>

int  RenderBuffer_VerticesToBeStored;

int  RenderBuffer_IndicesToBeStored;

void RenderBuffer_ClearRenderBuffer(void);

void RenderBuffer_StartStoring(int numIndices, int numVertices, RwImVertexIndex** indexStart, RxObjSpace3dVertex** vertexStart);

void RenderBuffer_StopStoring(void);

void RenderBuffer_RenderStuffInBuffer(void);

#define TEMPBUFFERVERTSIZE 256
#define TEMPBUFFERINDEXSIZE 1024

extern int TempBufferVerticesStored;
extern int TempBufferIndicesStored;
extern RxObjSpace3dVertex TempBufferRenderVertices[TEMPBUFFERVERTSIZE];
extern RwImVertexIndex TempBufferRenderIndexList[TEMPBUFFERINDEXSIZE];
