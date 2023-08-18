#include "types.h"
#include "loadimage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Based on : http://netghost.narod.ru/gff/graphics/summary/micbmp.htm

typedef struct{
	
	//uint16 type; 
	uint32 size; // 2
	uint32 reserved;  // 6
	uint32 offset; // 10

	uint32 header_size; // 14, 40 for Windows 3.x BMP
	uint32 width ;  // 18
	uint32 height; // 22
	uint16 planes; // always 1  
	uint16 bpp; // must be 24  
	uint32 compression; // must be 0, which is none
	uint32 image_size; // in bytes
	uint32 ppm[2]; // pixels pr. meter, not used and unreliable.
	uint32 color_info[2]; // don't use it

} bmp_header;


static bool Mirror(Image *image)
{
	uint8 tmp[image->width*4];

	for(uint32 y=0; y < (image->height/2); y++)
	{
		memcpy(tmp, image->data+((y*image->width)*4), image->width*4);
		memcpy(image->data+((y*image->width)*4), image->data+(((image->height-(y+1))*image->width)*4), image->width*4);
		memcpy(image->data+(((image->height-(y+1))*image->width)*4), tmp, image->width*4);
	}

	return true;
}

static bool Read4(Image *image, uint8* data, uint32 size, uint32 palsize)
{
	uint8* palette = data;
	uint8* pixels = palette+(16*palsize);
	
	for(uint32 y=0; y < image->height; y++)
		for(uint32 x=0; x < image->width; x++)
		{
			uint32 index = 0;
			uint32 i = (((image->height-(y+1))*image->width)+x);

			if(i % 2)
				index = (pixels[(i/2)]) & 0xF;
			else
				index  = (pixels[(i/2)] >> 4) & 0xF;

			image->data[(((y*image->width)+x)*4)+0] =  palette[(index*palsize)+2];
			image->data[(((y*image->width)+x)*4)+1] =  palette[(index*palsize)+1];
			image->data[(((y*image->width)+x)*4)+2] =  palette[(index*palsize)+0];
			
			if(palsize == 4)
				image->data[(((y*image->width)+x)*4)+3] =  palette[(index*palsize)+3];
			else
				image->data[(((y*image->width)+x)*4)+3] =  0x80;


		}


	image->psm = CLUT4_RGBA32;
	
	return true;
}

static bool Read8(Image *image, uint8* data, uint32 size, uint32 palsize)
{
	uint8* palette = data;
	uint8* pixels = palette+(256*palsize);
	
	size = (size-(256*palsize));

	for(uint32 y=0; y < image->height; y++)
		for(uint32 x=0; x < image->width; x++)
		{
			image->data[(((y*image->width)+x)*4)+0] =  palette[(pixels[(((image->height-(y+1))*image->width)+x)]*palsize)+2];
			image->data[(((y*image->width)+x)*4)+1] =  palette[(pixels[(((image->height-(y+1))*image->width)+x)]*palsize)+1];
			image->data[(((y*image->width)+x)*4)+2] =  palette[(pixels[(((image->height-(y+1))*image->width)+x)]*palsize)+0];
			
			if(palsize == 4)
				image->data[(((y*image->width)+x)*4)+3] =  palette[(pixels[(((image->height-y)*image->width)+x)]*palsize)+3];
			else
				image->data[(((y*image->width)+x)*4)+3] =  0x80;


		}
	
	image->psm = CLUT8_RGBA32;

	return true;
}

static bool Read24(Image *image, uint8* data, uint32 size)
{

	for(uint32 y=0; y < image->height; y++)
		for(uint32 x=0; x < image->width; x++)
		{
			image->data[(((y*image->width)+x)*4)+0] =  data[((((image->height-(y+1))*image->width)+x)*3)+2];
			image->data[(((y*image->width)+x)*4)+1] =  data[((((image->height-(y+1))*image->width)+x)*3)+1];
			image->data[(((y*image->width)+x)*4)+2] =  data[((((image->height-(y+1))*image->width)+x)*3)+0];
			image->data[(((y*image->width)+x)*4)+3] =  0xFF;


		}
	
	image->psm = RGB24;

	return true;
}

static bool ReadRLE4(Image *image, uint8* data, uint32 size, uint32 palsize)
{
	uint32* palette = (uint32*)data;
	uint8* pixels = (uint8*)palette+(16*palsize);

	uint32 i=0;
	uint32* out = (uint32*)image->data;

	// Flip byte in palette
	for(uint32 p=0; p < 16; p++)
	{
		uint8 rgba[4];
		uint8 t;

		memcpy(rgba, &palette[p], 4);
		
		t = rgba[0];
		rgba[0] = rgba[2];
		rgba[2] = t;

		if(palsize == 4)
			rgba[3] = (0xFF-rgba[3])/2;
		else
			rgba[3] = 0x80;

		memcpy(&palette[p], rgba, 4);
	}

	while(i < (size-(16*palsize)))
	{
		if(pixels[i] == 0)
		{
			i++;
			switch(pixels[i])
			{
				case 0: // end of scanline
				{
				  i++;
				} break;

				case 1: // end of data
				{		
					i++;
				} break;

				case 2: // move to x = pixels[i+1], y = pixels[i+2]
				{
					out += (pixels[i+2]*image->width)+pixels[i+1];
					i+=2;
				} break;
				
				default: // unencoded bytes
				{
					
					for(uint32 j=0; j < pixels[i]; j++)
					{
						if(j % 2)
							out[0] = palette[pixels[i+1+(j/2)]>>4];
						else
							out[0] = palette[pixels[i+1+(j/2)] & 0xF];
			
						out++;
					}
					
					uint32 add = 0;
					
					add = pixels[i];

					if(pixels[i] % 2) add++;
					
					add /= 2;

					if(add % 2) add++;
					
					i += add+1;
				} break;
			}
			
		}
		else
		{
			// RLE encoded bytes
			for(uint32 j=0; j < pixels[i]; j++)
			{
				if(j % 2)
					out[0] = palette[pixels[i+1] >> 4];
				else
					out[0] = palette[pixels[i+1] & 0xF];

				out++;
			}

			i+=2;
		}
	}

	Mirror(image);

	image->psm = CLUT4_RGBA32;

	return true;
}

static bool ReadRLE8(Image *image, uint8* data, uint32 size, uint32 palsize)
{
	uint32* palette = (uint32*)data;
	uint8* pixels = (uint8*)palette+(256*palsize);
	
	uint32 i=0;
	uint32* out = (uint32*)image->data;
	uint32 y=0;

	// Flip byte in palette
	for(uint32 p=0; p < 256; p++)
	{
		uint8 rgba[4];
		uint8 t;

		memcpy(rgba, &palette[p], 4);
		
		t = rgba[0];
		rgba[0] = rgba[2];
		rgba[2] = t;

		if(palsize == 4)
			rgba[3] = (0xFF-rgba[3])/2;
		else
			rgba[3] = 0x80;

		memcpy(&palette[p], rgba, 4);
	}

	while(i < (size-(256*palsize)))
	{
		if(pixels[i] == 0)
		{
			i++;
			switch(pixels[i])
			{
				case 0: // end of scanline
				{
					i++;
				} break;

				case 1: // end of data
				{		
					i++;
				} break;

				case 2: // move to x = pixels[i+1], y = pixels[i+2]
				{
					out += (pixels[i+2]*image->width)+pixels[i+1];

					i+=2;
				} break;
				
				default: // unencoded bytes
				{
					for(uint32 j=0; j < pixels[i]; j++)
					{
						out[0] = palette[pixels[i+1+j]];
						out++;
					}
					
					uint32 add = 0;

					if(pixels[i] % 2) add = 1;

					i += pixels[i]+1+add;
				} break;
			}
			
		}
		else
		{
			for(uint32 j=0; j < pixels[i]; j++)
			{
				out[0] = palette[pixels[i+1]];
				out++;
			}

			i+=2;
		}
	}

	Mirror(image);
	
	image->psm = CLUT8_RGBA32;

	printf("pixels %i,%i\n", ((uint8*)out-(uint8*)image->data)/4, 256*128);

	return true;
}

bool LoadBMP(Image* image, char* filename)
{
	FILE*	fpBMP;
	bmp_header hBMP;
	uint8* pBMP;
	uint32 palsize;
	uint16 type;

	if(!(fpBMP = fopen( filename, "rb" )))
	{	
		printf("Error: Could not open BMP file \"%s\" for reading.\n", filename);
		return false;
	}

	fseek(fpBMP, 0, SEEK_SET);
	fread(&type, sizeof(type), sizeof(uint8), fpBMP);

	fseek(fpBMP, 2, SEEK_SET);
	fread( &hBMP, sizeof(bmp_header), sizeof(uint8), fpBMP);

	/*
	dprintf("BMP Header\n");
	dprintf("Type : 0x%04X\n", type);
	dprintf("Size : 0x%08X\n", hBMP.size);
	dprintf("Offset : 0x%08X\n", hBMP.offset);
	dprintf("HeaderSize : %i\n", hBMP.header_size);
	dprintf("Width : %i\n", hBMP.width);
	dprintf("Height : %i\n", hBMP.height);
	dprintf("Bpp : %i\n", hBMP.bpp);
	dprintf("Compression: %i\n", hBMP.compression);
	*/

	if(type != 0x4D42) // "BM"
	{
		printf("Error: \"%s\" is not a valid BMP file.\n", filename);
		return false;
	}

	if( (hBMP.bpp != 24 ) && (hBMP.bpp != 8 ) && (hBMP.bpp != 4 ) )
	{
		printf("Error: %i-bit BMP's are not supported.\n", hBMP.bpp);
		return false;
	}

	if((hBMP.bpp) == 4 || (hBMP.bpp == 8))
	{
		switch(hBMP.header_size)
		{
			case 12: // MS BMP 2.x
				palsize = 3;
			break;

			case 40: // MS BMP 3.x
				palsize = 4;
			break;

			default:
				printf("Error: BMP header not supported.\n");
				return false;
			break;
		}
	}
	
	// dprintf("Palette Size : %i\n", palsize); 

	// Setup Image struct
	image->width = hBMP.width;
	image->height = hBMP.height;
	image->data = (uint8*)malloc(image->width*image->height*4);

	fseek(fpBMP, 0, SEEK_END);

	uint32 len = ftell(fpBMP)-2-12-hBMP.header_size;

	if(!(pBMP = (uint8*)malloc( len )))
	{
		printf("Error: Could not allocate \"%s\"\n", filename);
		return -1;
	}
	
	// Read entire BMP file
	fseek(fpBMP, 2+12+hBMP.header_size, SEEK_SET);
	fread(pBMP, len, sizeof(uint8), fpBMP);
	fclose( fpBMP );

	bool res;

	switch(hBMP.bpp)
	{
		case 24: 
			res = Read24(image, pBMP, len); 
		break;
		
		case 8: 
			if(hBMP.compression == 0)
				res = Read8(image, pBMP, len, palsize); 
			else
				res = ReadRLE8(image, pBMP, len, palsize);
		break;
		
		case 4: 
			if(hBMP.compression == 0)	
				res = Read4(image, pBMP, len, palsize);
			else
				res = ReadRLE4(image, pBMP, len, palsize);
		break;

		default: res = false; break;
	}
	
	free(pBMP);

	if(res == false)
	{
		printf("Error: Could not decode BMP \"%s\"\n", filename);
		return false;
	}


	return true;
}

