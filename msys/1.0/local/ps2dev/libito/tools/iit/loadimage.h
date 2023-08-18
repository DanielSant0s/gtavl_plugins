#ifndef _LOADIMAGE_H_
#define _LOADIMAGE_H_

#include "types.h"

typedef struct
{
	uint8* data; // always RGBA32
	uint32  width;
	uint32  height;
	uint32	psm; // original psm
} Image;

bool LoadImage(Image* image, char* filename);
bool FreeImage(Image* image);
char* GetSupportedFormats();


bool LoadBMP(Image* image, char* filename);
bool LoadJPG(Image* image, char* filename);
bool LoadPNG(Image* image, char* filename);
bool LoadTIFF(Image* image, char* filename);

#endif
