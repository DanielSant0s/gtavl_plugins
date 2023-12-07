
#include "renderbuffer.h"

int TempBufferVerticesStored;
int TempBufferIndicesStored;

RxObjSpace3dVertex TempBufferRenderVertices[TEMPBUFFERVERTSIZE];
RwImVertexIndex TempBufferRenderIndexList[TEMPBUFFERINDEXSIZE];

int RenderBuffer_VerticesToBeStored;
int RenderBuffer_IndicesToBeStored;

void RenderBuffer_ClearRenderBuffer() {
	TempBufferVerticesStored = 0;
	TempBufferIndicesStored = 0;
}

void RenderBuffer_StartStoring(int numIndices, int numVertices, RwImVertexIndex** indexStart, RxObjSpace3dVertex** vertexStart) {
	if (TempBufferIndicesStored + numIndices >= TEMPBUFFERINDEXSIZE)
		RenderBuffer_RenderStuffInBuffer();
	if (TempBufferVerticesStored + numVertices >= TEMPBUFFERVERTSIZE)
		RenderBuffer_RenderStuffInBuffer();
	*indexStart = &TempBufferRenderIndexList[TempBufferIndicesStored];
	*vertexStart = &TempBufferRenderVertices[TempBufferVerticesStored];
	RenderBuffer_IndicesToBeStored = numIndices;
	RenderBuffer_VerticesToBeStored = numVertices;
}

void RenderBuffer_StopStoring() {
	int i;
	for (i = TempBufferIndicesStored; i < TempBufferIndicesStored + RenderBuffer_IndicesToBeStored; i++)
		TempBufferRenderIndexList[i] += TempBufferVerticesStored;
	TempBufferIndicesStored += RenderBuffer_IndicesToBeStored;
	TempBufferVerticesStored += RenderBuffer_VerticesToBeStored;
}

void RenderBuffer_RenderStuffInBuffer() {
	if (TempBufferVerticesStored && RwIm3DTransform(TempBufferRenderVertices, TempBufferVerticesStored, NULL, 1)) {
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, TempBufferRenderIndexList, TempBufferIndicesStored);
		RwIm3DEnd();
	}
	RenderBuffer_ClearRenderBuffer();
}
