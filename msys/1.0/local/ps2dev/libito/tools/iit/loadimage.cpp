#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "loadimage.h"
#include "types.h"

extern char* uppercase(char* str);

// --------------------------------------------------------
// PNG
// --------------------------------------------------------

bool LoadPNG(Image* image, char* filename)
{
	return false;
}


// --------------------------------------------------------
// JPG
// --------------------------------------------------------
bool LoadJPG(Image* image, char* filename)
{

	return false;
}

// --------------------------------------------------------
// TIFF
// --------------------------------------------------------
bool LoadTIFF(Image* image, char* filename)
{

	return false;
}

// --------------------------------------------------------
// Generic
// --------------------------------------------------------

char* SupportedFormats[] = {"BMP"}; /* ,"JPEG", "PNG", "TIFF"}; */
uint32 NumSupportedFormats = 1;

char* GetSupportedFormats()
{
	static char str[64];
	
	sprintf(str,"%s", SupportedFormats[0]);

	for(uint32 i=1; i < NumSupportedFormats; i++)
	{
		sprintf(str, "%s, %s", str, SupportedFormats[i]);
	}
	
	return str;
}

bool LoadImage(Image* image, char* filename)
{
	char ext[32];
	
	image->data = 0;
	image->width = 0;
	image->height = 0;

	uint32 i = strlen(filename);

	while(i > 0)
	{
		if(filename[i] == '.')
		{
			strcpy(ext, &filename[i+1]);
			break;
		}
	
		i--;
	}
	
	if(!strcasecmp(ext, "BMP")) return LoadBMP(image, filename);
//	if(!strcasecmp(ext, "JPG") || !strcasecmp(ext, "JPEG")) return LoadJPG(image, filename);
	if(!strcasecmp(ext, "PNG")) return LoadPNG(image, filename);
//	if(!strcasecmp(ext, "TIF") || !strcasecmp(ext, "TIFF") ) return LoadTIFF(image, filename);

	if(i)
		printf("Error: Image type %s not supported\n",  uppercase(ext));
	else
		printf("Error: '%s' has no extension\n", filename);

	return false;
}

bool FreeImage(Image* image)
{
	dprintf("freeimage\n");

	if(image->data)
		free(image->data);

	image->width = 0;
	image->height = 0;
	
}
