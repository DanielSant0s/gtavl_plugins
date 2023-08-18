// tga2fnt.cpp : implementation file

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "../common/tga.h"
#include "../common/iif.h"
#include "../common/bfnt.h"


int main(int argc, char* argv[6])
{
	TGA_HEADER	tgaHeader;
	FNT_HEADER	fntHeader;

	FILE*	tgaFile;
	FILE*	datFile;
	FILE*	fntFile;

	// Parameters to get from argv
	char* tgaFilename; 
	char* datFilename;
	char* fntFilename;

	int CharWidth = 16;
	int CharHeight = 16;
	int GridWidth = 16;
	int GridHeight = 16;

	char* end;

	if(argc < 8)
	{

	if(argc == 2)
	{
		if(!strcmp((_strupr(argv[1])),"-H"))
			{
				printf("tga2fnt - by A.Lee (aka Hiryu)\n");
				printf("Usage: bmp2iif <tga file> <metrics file> <output file>\n"
					" <char-width> <char-height> <num H chars> <num V chars>\n");
				return -1;
			}
	}
		
		printf("tga2fnt -h for help\n");
		printf("Error: Too few arguments.\n");
		return -1;
	}

	tgaFilename = argv[1];
	datFilename = argv[2];
	fntFilename = argv[3];

	CharWidth  = strtol(argv[4],&end,10);
	CharHeight = strtol(argv[5],&end,10);
	GridWidth  = strtol(argv[6],&end,10);
	GridHeight = strtol(argv[7],&end,10);



	tgaFile = fopen(tgaFilename,"rb"); 

	if (tgaFile == 0)
	{
		printf("Error: TGA file \'%s\' could not be opened !\n",tgaFilename);
		return -1;
	}

	// Copy the font-metrics data into the .fnt file
	datFile = fopen(datFilename, "rb");

	if (datFile == 0)
	{
		printf("Error: Metrics file \'%s\' could not be opened !\n",datFilename);
		return -1;
	}

	fntFile = fopen(fntFilename, "wb");

	if (datFile == 0)
	{
		printf("Error: Bitmap Font file \'%s\' could not be opened !\n",fntFilename);
		return -1;
	}




	// Read the TGA file header
	fread(&tgaHeader,1,18,tgaFile);

	if ((tgaHeader.imagetype & 0x07) != 2)
	{
		printf("Error: Non-RGB images not yet supported !");
		return -1;
	}

	if (tgaHeader.imagetype >= 8)
	{
		printf("Error: RLE packed images not yet supported !");
		return -1;
	}

	if (tgaHeader.bits != 32)
	{
		printf("Error: Only 32bit images are currently supported !");
		return -1;
	}



	// Create the header of the .fnt file

	fntHeader.ID[0] = 'B';
	fntHeader.ID[1] = 'F';
	fntHeader.ID[2] = 'N';
	fntHeader.ID[3] = 'T';

	fntHeader.TexWidth = tgaHeader.width;
	fntHeader.TexHeight = tgaHeader.height;

	// Size of one char in grid
	fntHeader.CharGridWidth = CharWidth;
	fntHeader.CharGridHeight = CharHeight;

	// number of characters in grid
	fntHeader.NumXChars = GridWidth;
	fntHeader.NumYChars = GridHeight;

	switch(tgaHeader.bits)
	{
	case 8:
		fntHeader.PSM = 0x13;
		break;
	case 16:
		fntHeader.PSM = 0x02;
		break;
	case 24:
		fntHeader.PSM = 0x12;
		break;
	case 32:
	default:
		fntHeader.PSM = 0x00;
		break;
	}

	fwrite(&fntHeader, 1, sizeof(fntHeader), fntFile);


	unsigned char* datBuffer = (unsigned char*)malloc(256);
	fread(datBuffer, 1, 256, datFile);

	fwrite(datBuffer, 1, 256, fntFile);
	free(datBuffer);


	// Copy the bitmap data into the .fnt data
	// (Correcting the alpha value if a 32bit image)
	int numpixels = tgaHeader.width * tgaHeader.height;

	TGA32_PIXEL* tgaBuffer = (TGA32_PIXEL*)malloc(numpixels*sizeof(TGA32_PIXEL));
	RGBA32_PIXEL* fntBuffer = (RGBA32_PIXEL*)malloc(numpixels*sizeof(RGBA32_PIXEL));

	fread(tgaBuffer, sizeof(TGA32_PIXEL), numpixels, tgaFile);
    
	// re-order the colour data, and halve the alpha values of each pixel
	for (int pix=0; pix<numpixels; pix++) // Every 4th byte
	{
		fntBuffer[pix].Red   = tgaBuffer[pix].Red;
		fntBuffer[pix].Green = tgaBuffer[pix].Green;
		fntBuffer[pix].Blue  = tgaBuffer[pix].Blue;
		fntBuffer[pix].Alpha = tgaBuffer[pix].Alpha/2;
	}

	fwrite(fntBuffer, sizeof(RGBA32_PIXEL), numpixels, fntFile);


	free(tgaBuffer);
	free(fntBuffer);

	// Close all the files
	fclose(datFile);
	fclose(tgaFile);
	fclose(fntFile);

	return 0;

}
