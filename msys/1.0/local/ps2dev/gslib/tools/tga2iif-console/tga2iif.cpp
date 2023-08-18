// tga2fnt.cpp : implementation file

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "../common/tga.h"
#include "../common/iif.h"


int main(int argc, char* argv[6])
{
	TGA_HEADER	tgaHeader;
	IIF_HEADER	iifHeader;

	FILE*	tgaFile;
	FILE*	iifFile;

	// Parameters to get from argv
	char* tgaFilename; 
	char* iifFilename;
	char* format;


	if(argc < 4)
	{

	if(argc == 2)
	{
		if(!strcmp((_strupr(argv[1])),"-H"))
		{
			printf("tga2iif - by A.Lee (aka Hiryu)\n");
			printf("Usage: bmp2iif <tga file> <output format> <output file>\n"
					"formats: RGBA32 RGB24 RGB16\n");
			return -1;
		}
	}
		
		printf("tga2iif -h for help\n");
		printf("Error: Too few arguments.\n");
		return -1;
	}

	tgaFilename = argv[1];
	format = argv[2];
	iifFilename = argv[3];

	tgaFile = fopen(tgaFilename,"rb"); 

	if (tgaFile == 0)
	{
		printf("Error: TGA file \'%s\' could not be opened !\n",tgaFilename);
		return -1;
	}


	iifFile = fopen(iifFilename, "wb");

	if (iifFile == 0)
	{
		printf("Error: IIF file \'%s\' could not be opened !\n",iifFilename);
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



	// Create the header of the .fnt file

	iifHeader.ID[0] = 'I';
	iifHeader.ID[1] = 'I';
	iifHeader.ID[2] = 'F';
	iifHeader.ID[3] = '1';

	iifHeader.width = tgaHeader.width;
	iifHeader.height = tgaHeader.height;


	if (!strcmp((_strupr(argv[2])),"RGBA32"))
		iifHeader.psm = 0x00;	// 32bit
	else if (!strcmp((_strupr(argv[2])),"RGB24"))
		iifHeader.psm = 0x12;	// 24 bit
	else if (!strcmp((_strupr(argv[2])),"RGB16"))
		iifHeader.psm = 0x02;	// 16 bit
	else
	{
		printf("Error: Invalid format specified!\n");
		return -1;
	}

	fwrite(&iifHeader, 1, sizeof(iifHeader), iifFile);

	int numpixels = tgaHeader.width * tgaHeader.height;

	TGA32_PIXEL* tgaPix32;
	TGA24_PIXEL* tgaPix24;

	RGBA32_PIXEL* iifPix32;
	RGB24_PIXEL* iifPix24;
	RGB16_PIXEL* iifPix16;

	int pixel;

	switch(tgaHeader.bits)
	{
	case 32:
		tgaPix32 = (TGA32_PIXEL*)malloc(numpixels*sizeof(TGA32_PIXEL));
		fread(tgaPix32, sizeof(TGA32_PIXEL), numpixels, tgaFile);

		switch (iifHeader.psm)
		{
		case 0x00:	// Read 32bit, Write 32bit

			iifPix32 = (RGBA32_PIXEL*)malloc(numpixels*sizeof(RGBA32_PIXEL));

			for (pixel=0;pixel<numpixels;pixel++)
			{
				iifPix32[pixel].Red   = tgaPix32[pixel].Red;
				iifPix32[pixel].Green = tgaPix32[pixel].Green;
				iifPix32[pixel].Blue  = tgaPix32[pixel].Blue;
				iifPix32[pixel].Alpha = tgaPix32[pixel].Alpha/2;
			}

			fwrite(iifPix32, sizeof(RGBA32_PIXEL), numpixels, iifFile);

			free(iifPix32);

			break;

		case 0x12:	// Read 32bit, Write 24bit

			iifPix24 = (RGB24_PIXEL*)malloc(numpixels*sizeof(RGB24_PIXEL));

			for (pixel=0;pixel<numpixels;pixel++)
			{
				iifPix24[pixel].Red   = tgaPix32[pixel].Red;
				iifPix24[pixel].Green = tgaPix32[pixel].Green;
				iifPix24[pixel].Blue  = tgaPix32[pixel].Blue;
			}

			fwrite(iifPix24, sizeof(RGB24_PIXEL), numpixels, iifFile);

			free(iifPix24);

			break;



		case 0x02:	// Read 32bit, Write 16bit

			iifPix16 = (RGB16_PIXEL*)malloc(numpixels*sizeof(RGB16_PIXEL));

			for (pixel=0;pixel<numpixels;pixel++)
			{
				iifPix16[pixel].Red   = tgaPix32[pixel].Red >> 3;
				iifPix16[pixel].Green = tgaPix32[pixel].Green >> 3;
				iifPix16[pixel].Blue  = tgaPix32[pixel].Blue >> 3;
				iifPix16[pixel].Alpha = tgaPix32[pixel].Alpha >> 7;
			}

			fwrite(iifPix16, sizeof(RGB16_PIXEL), numpixels, iifFile);

			free(iifPix16);
			break;

		}

		free (tgaPix32);

		break;

	case 24:
		tgaPix24 = (TGA24_PIXEL*)malloc(numpixels*sizeof(TGA24_PIXEL));
		fread(tgaPix24, sizeof(TGA24_PIXEL), numpixels, tgaFile);

		switch (iifHeader.psm)
		{
		case 0x00:	// Read 24bit, Write 32bit - Add solid alpha value

			iifPix32 = (RGBA32_PIXEL*)malloc(numpixels*sizeof(RGBA32_PIXEL));

			for (pixel=0;pixel<numpixels;pixel++)
			{
				iifPix32[pixel].Red   = tgaPix24[pixel].Red;
				iifPix32[pixel].Green = tgaPix24[pixel].Green;
				iifPix32[pixel].Blue  = tgaPix24[pixel].Blue;
				iifPix32[pixel].Alpha = 0x80;
			}

			fwrite(iifPix32, sizeof(RGBA32_PIXEL), numpixels, iifFile);
			free(iifPix32);
			break;


		case 0x12:	// Read 24bit, Write 24bit - nothing to change
			fwrite(tgaPix24, sizeof(RGB24_PIXEL), numpixels, iifFile);
			break;

		case 0x02:	// Read 24bit, Write 16bit

			iifPix16 = (RGB16_PIXEL*)malloc(numpixels*sizeof(RGB16_PIXEL));

			for (pixel=0;pixel<numpixels;pixel++)
			{
				iifPix16[pixel].Red   = tgaPix24[pixel].Red >> 3;
				iifPix16[pixel].Green = tgaPix24[pixel].Green >> 3;
				iifPix16[pixel].Blue  = tgaPix24[pixel].Blue >> 3;
				iifPix16[pixel].Alpha = 0;
			}

			fwrite(iifPix16, sizeof(RGB16_PIXEL), numpixels, iifFile);
			free(iifPix16);

			break;
		}

		free (tgaPix24);

		break;


	case 8:
	case 16:
	default:
		printf("Error: Unsupported format TGA file\n");
		return -1;
		break;

	}


	// Close all the files
	fclose(tgaFile);
	fclose(iifFile);

	return 0;

}
