#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveiif.h"
#include "loadimage.h"
#include "exoquant.h"
#include "swizzle.h"

extern char* uppercase(char* str);

// IIF v1 - Ito Image Format
typedef struct{
	uint32 identifier; // 'IIF1' - IIFv (v = version of iif)
	uint32 width;
	uint32 height;
	uint32 psm;
} IIFHeader;

// IIF v2 - Ito Image Format
typedef struct{
	uint32 identifier; // 'IIF2'
	uint32 headerSize;
	uint16 width;
	uint16 height;
	uint16 psm;
	uint16 options;
	char   name[16];
} IIF2Header;

static bool ConvertRGBA32(Image *image, uint32& outBuffer, uint32& outSize)
{
	outSize = image->width*image->height*4;
	outBuffer = (uint32)malloc(outSize);

	memcpy((void*)outBuffer, image->data, outSize);
	
	return true;
}

static bool ConvertRGB24(Image *image, uint32& outBuffer, uint32& outSize)
{
	outSize = image->width*image->height*3;
	outBuffer = (uint32)malloc(outSize);

	uint8* out = (uint8*)outBuffer;
	
	for(uint32 i=0; i < (image->width*image->height); i++)
	{
		out[(i*3)] = image->data[(i*4)];
		out[(i*3)+1] = image->data[(i*4)+1];
		out[(i*3)+2] = image->data[(i*4)+2];
	}
	
	return true;
}

static bool ConvertRGBA16(Image* image, uint32& outBuffer, uint32& outSize, bool swizzle)
{
	outSize = image->width*image->height*2;
	outBuffer = (uint32)malloc(outSize);
	
	uint16* out = (uint16*)outBuffer;

	for(uint32 i=0; i < (outSize/2); i++)
	{	
		uint32 r = image->data[(i*4)+0] >> 3;
		uint32 g = image->data[(i*4)+1] >> 3;
		uint32 b = image->data[(i*4)+2] >> 3;
		uint32 a = (image->data[(i*4)+3] > 0);
		
		out[i] = (a << 15) | ( (b & 0x1F) << 10) | ( (g & 0x1F) << 5) | (r & 0x1F);
	}

	if(swizzle)
	{
		uint8* temp = (uint8*)malloc(outSize);

		Swizzle16to32(temp, (uint8*)outBuffer, image->width, image->height);

		memcpy((void*)outBuffer, temp, outSize);
		free(temp);
	}

	return true;
}

static bool ConvertCLUT8RGBA32(Image *image, uint32& outBuffer, uint32& outSize, bool swizzle)
{
	outSize = (image->width*image->height)+(256*4);
	outBuffer = (uint32)malloc(outSize);
	
	if(image->psm > RGBA16)
	{
		uint32 *p = (uint32*)image->data;
		uint8* pixels = (uint8*)outBuffer+(256*4);
		uint32* pal = (uint32*)outBuffer;
		uint32 index = 0;
		
		for(uint32 i=0; i < (image->width*image->height); i++)
		{
			uint32 j = 0;

			while((p[i] != pal[j]) && (j < 256)) j++;

			if(j == 256) j = 255;

			if(p[i] != pal[j])
			{
				pal[index] = p[i];
				pixels[i] = index;
				index++;
				//printf("!");
			}
			else
			{
				//printf("%i\n", j);
				pixels[i] = j;
			}
		}
	
	}
	else
	{
		exq_data *pExq = exq_init();
		exq_feed(pExq, image->data, image->width*image->height);
		exq_quantize(pExq, 256);	
		exq_get_palette(pExq, (uint8*)outBuffer, 256);
		exq_map_image(pExq, image->width*image->height, image->data, (uint8*)(outBuffer+(256*4)));
		exq_free(pExq);
	}
	
	uint32* pal = (uint32*)outBuffer;

	for(uint32 i=0; i < 8; i++)
	{
		uint32 temp[8];
	
		memcpy(temp, pal+8, (8*4));
		memcpy(pal+8, pal+16, (8*4));
		memcpy(pal+16, temp, (8*4));
		
		pal += 32;
	}


	if(swizzle)
	{
		uint8* temp = (uint8*)malloc(outSize-(256*4));

		Swizzle8to32(temp, (uint8*)outBuffer+(256*4), image->width, image->height);
		memcpy((uint8*)outBuffer+(256*4), temp, outSize-(256*4));
		free(temp);
	}

	return true;
}


static bool ConvertCLUT4RGBA32(Image *image, uint32& outBuffer, uint32& outSize, bool swizzle)
{
	outSize = ((image->width*image->height)/2)+(16*4);
	outBuffer = (uint32)malloc(outSize);

	uint8* buffer = (uint8*)malloc(image->width*image->height);
	
	if(image->psm > CLUT8_RGBA16)
	{
		uint32 *p = (uint32*)image->data;
		uint8* pixels = buffer;
		uint32* pal = (uint32*)outBuffer;
		uint32 index = 0;
		
		for(uint32 i=0; i < (image->width*image->height); i++)
		{
			uint32 j = 0;

			while((p[i] != pal[j]) && (j < 16)) j++;

			if(j == 16) j = 15;;

			if(p[i] != pal[j])
			{
				pal[index] = p[i];
				pixels[i] = index;
				index++;
			}
			else
			{
				pixels[i] = j;
			}
		}
	
	}
	else
	{
		exq_data *pExq = exq_init();
		exq_feed(pExq, image->data, image->width*image->height);
		exq_quantize(pExq, 16);
		exq_get_palette(pExq, (uint8*)outBuffer, 16);
		exq_map_image(pExq, image->width*image->height, image->data, buffer);
		exq_free(pExq);
	}

	uint8* out = ((uint8*)outBuffer)+(16*4);

	for(uint32 i=0; i < ((image->width*image->height)/2); i++)
	{
		//out[i] = (i % 16) << 4 | (i % 16);
			
		out[i] = buffer[(i*2)+1] << 4 | buffer[(i*2)];
	}


	free(buffer);

	if(swizzle)
	{
		uint8* temp = (uint8*)malloc(outSize-(16*4));

		Swizzle4to32(temp, (uint8*)outBuffer+(16*4), image->width, image->height);
		memcpy((uint8*)outBuffer+(16*4), temp, outSize-(16*4));
		free(temp);
	}

	return true;
}

static bool ConvertCLUT8RGBA16(Image *image, uint32& outBuffer, uint32& outSize, bool swizzle)
{
	uint8 palette[256*4];

	outSize = (image->width*image->height)+(256*2);
	outBuffer = (uint32)malloc(outSize);

	if(image->psm > RGBA16)
	{
		uint32 *p = (uint32*)image->data;
		uint8* pixels = (uint8*)outBuffer+(256*2);
		uint32* pal = (uint32*)palette;
		uint32 index = 0;
		
		for(uint32 i=0; i < (image->width*image->height); i++)
		{
			uint32 j = 0;

			while((p[i] != pal[j]) && (j < 256)) j++;
			
			if(j == 256) j = 255;

			if(p[i] != pal[j])
			{
				pal[index] = p[i];
				pixels[i] = index;
				index++;
				//printf("!");
			}
			else
			{
				//printf("%i\n", j);
				pixels[i] = j;
			}
		}
	
	}
	else
	{

		exq_data *pExq = exq_init();
		exq_feed(pExq, image->data, image->width*image->height);
		exq_quantize(pExq, 256);
		exq_get_palette(pExq, palette, 256);
		exq_map_image(pExq, image->width*image->height, image->data, (uint8*)(outBuffer+(256*2)));
		exq_free(pExq);
	}

	uint32* pal = (uint32*)palette;

	for(uint32 i=0; i < 8; i++)
	{
		uint32 temp[8];
	
		memcpy(temp, pal+8, (8*4));
		memcpy(pal+8, pal+16, (8*4));
		memcpy(pal+16, temp, (8*4));
		
		pal += 32;
	}


	uint16* out = (uint16*)outBuffer;

	for(uint32 i=0; i < 256; i++)
	{	
		uint32 r = palette[(i*4)+0] >> 3;
		uint32 g = palette[(i*4)+1] >> 3;
		uint32 b = palette[(i*4)+2] >> 3;
		uint32 a = (palette[(i*4)+3] > 0);
		
		out[i] = (a << 15) | ( (b & 0x1F) << 10) | ( (g & 0x1F) << 5) | (r & 0x1F);
	}
	
	if(swizzle)
	{
		uint8* temp = (uint8*)malloc(outSize-(256*2));

		Swizzle8to32(temp, (uint8*)outBuffer+(256*2), image->width, image->height);
		memcpy((uint8*)outBuffer+(256*2), temp, outSize-(256*2));
		free(temp);
	}


	return true;
}

static bool ConvertCLUT4RGBA16(Image *image, uint32& outBuffer, uint32& outSize, bool swizzle)
{
	uint8 palette[16*4];

	outSize = ((image->width*image->height)/2)+(16*2);
	outBuffer = (uint32)malloc(outSize);

	uint8* buffer = (uint8*)malloc(image->width*image->height);

	if(image->psm > CLUT8_RGBA16)
	{
		uint32 *p = (uint32*)image->data;
		uint8* pixels = buffer;
		uint32* pal = (uint32*)palette;
		uint32 index = 0;
		
		for(uint32 i=0; i < (image->width*image->height); i++)
		{
			uint32 j = 0;

			while((p[i] != pal[j]) && (j < 16)) j++;

			if(j == 16) j = 15;

			if(p[i] != pal[j])
			{
				pal[index] = p[i];
				pixels[i] = index;
				index++;
			}
			else
			{
				pixels[i] = j;
			}
		}
	
	}
	else
	{
		exq_data *pExq = exq_init();
		exq_feed(pExq, image->data, image->width*image->height);
		exq_quantize(pExq, 16);
		exq_get_palette(pExq, palette, 16);
		exq_map_image(pExq, image->width*image->height, image->data, buffer);
		exq_free(pExq);
	}
	
	uint16* out = (uint16*)outBuffer;

	for(uint32 i=0; i < 16; i++)
	{	
		uint32 r = palette[(i*4)+0] >> 3;
		uint32 g = palette[(i*4)+1] >> 3;
		uint32 b = palette[(i*4)+2] >> 3;
		uint32 a = (palette[(i*4)+3] > 0);
		
		out[i] = (a << 15) | ( (b & 0x1F) << 10) | ( (g & 0x1F) << 5) | (r & 0x1F);
	}

	uint8* out2 = (uint8*)(outBuffer+(16*2));

	for(uint32 i=0; i < ((image->width*image->height)/2); i++)
	{
		out2[i] = ((buffer[(i*2)+1] & 0xF) << 4) | (buffer[(i*2)] & 0xF);
	}

	free(buffer);
	
	if(swizzle)
	{
		uint8* temp = (uint8*)malloc(outSize-(16*2));

		Swizzle4to32(temp, (uint8*)outBuffer+(16*2), image->width, image->height);
		memcpy((uint8*)outBuffer+(16*2), temp, outSize-(16*2));
		free(temp);
	}

	return true;
}






bool SaveIIF(char* filename, Image *image, uint32 psm, uint32 options)
{
		uint32 error = 0;
	
		dprintf("SaveIIF(..)\n");
		dprintf("image->data 0x%08X\n", (uint32)image->data);
		dprintf("image->width: %i\n", image->width);
		dprintf("image->height: %i\n", image->height);


		if(image->width > 4096)
		{	
			printf("Error: Width must be less than or equal 4096\n");
			return false;
		}

		if(image->height > 4096)
		{	
			printf("Error: Height must be less than or equal 4096\n");
			return false;
		}

	
		if((options & IIF_OPTION_SWIZZLE))
		{
			switch(psm)
			{
				case RGBA32:
				case RGB24:
				case AUTO:
					printf("Error: RGBA32 & RGB24 images can not be swizzled\n");
					return false;
				break;

				case RGBA16:
					if(image->width <= 64)
					{
						if(image->width % 16) error = 1;
					}
					else
					{
						if(image->width % 64) error = 1;
					}

					if(image->height <= 64)
					{
						if(image->height % 8) error = 1;
					}
					else
					{
						if(image->height % 64) error = 1;
					}
					
					if(error)
					{
						printf("Error: Could not swizzle image PSM: RGBA16 Width: %i Height: %i\n", image->width, image->height);
						printf("Width must be 16, 32, 48, 64, any multiple of 64 less than or equal to 4096\n");
						printf("Height must be 8, 16, 24, 32, 40, 48, 56, 64, any multiple of 64 less than or equal to 4096\n");
						return false;
					}		
				break;

				case CLUT8_RGBA32:
				case CLUT8_RGBA16:

					if(image->width % 16) error = 1;
					if(image->height % 4) error = 1;

					if(error)
					{
						printf("Error: Could not swizzle image PSM: CLUT8-RGBA16 / CLUT8-RGBA32 Width: %i Height: %i\n", image->width, image->height);
						printf("Width must be any multiple of 16 less than or equal to 4096\n");
						printf("Height must be any multiple of 4 less than or equal to 4096\n");
						return false;
					}
				break;

				case CLUT4_RGBA32:
				case CLUT4_RGBA16:
					
					if(image->width <= 128)
					{
						if(image->width % 32) error = 1;
					}
					else
					{
						if(image->width % 128) error = 1;
					}

					if(image->height <= 128)
					{
						if(image->height % 16) error = 1;
					}
					else
					{
						if(image->height % 128) error = 1;
					}
					
					if(error)
					{
						printf("Error: Could not swizzle image PSM: CLUT4-RGBA16 / CLUT4-RGBA32 Width %i Height %i\n", image->width, image->height);
						printf("Width must be 32, 64, 96, 128, any multiple of 128 less than or equal to 4096\n");
						printf("Height must be 16, 32, 48, 64, 80, 96, 112, 128, any multiple of 128 less than or equal to 4096\n");
						return false;
					}
				break;

				default:
					printf("Internal Error 2\n");
					return false;
				break;
			}
		}
		
		uint32 buffer;
		uint32 bufferSize;
		bool res = false;
	
		switch(psm)
		{
			case RGBA32: res = ConvertRGBA32(image, buffer, bufferSize); break;
			case RGB24: res = ConvertRGB24(image, buffer, bufferSize); break;
			case RGBA16: res = ConvertRGBA16(image, buffer, bufferSize, options & IIF_OPTION_SWIZZLE); break;
			case CLUT8_RGBA32: res = ConvertCLUT8RGBA32(image, buffer, bufferSize, options & IIF_OPTION_SWIZZLE); break;
			case CLUT4_RGBA32: res = ConvertCLUT4RGBA32(image, buffer, bufferSize, options & IIF_OPTION_SWIZZLE); break;
			case CLUT8_RGBA16: res = ConvertCLUT8RGBA16(image, buffer, bufferSize, options & IIF_OPTION_SWIZZLE); break;
			case CLUT4_RGBA16: res = ConvertCLUT4RGBA16(image, buffer, bufferSize, options & IIF_OPTION_SWIZZLE); break;
			
			default:
			{
				printf("Internal Error 3\n");
				return false;
			}
		}
		
		if(res == false)
		{
			printf("Error: Could not encode IIF\n");
			return false;
		}


		FILE* fpIIF;

		if(!(fpIIF = fopen( filename, "wb")))
		{	
			printf("Error: Could not open IIF file \"%s\" for writing.\n", filename);
			return false;
		}

		dprintf("bufferSize ~%i KB\n", bufferSize/1024);	
		dprintf("buffer %08X\n", buffer);


		if(options & IIF_OPTION_FORCE)
		{
			IIFHeader header;

			header.identifier = 0x31464949; // 'IIF1'text string
			header.width = image->width;
			header.height = image->height;
			header.psm = psm;

			fseek(fpIIF, 0, SEEK_SET);
			fwrite(&header, sizeof(uint8), sizeof(header), fpIIF);
			fseek(fpIIF, sizeof(header), SEEK_SET);
			fwrite((void*)buffer, sizeof(uint8), bufferSize, fpIIF);
		}
		else
		{
			IIF2Header header;

			header.identifier = 0x32464949; // 'IIF2'text string
			header.headerSize = 24;
			header.width = image->width;
			header.height = image->height;
			header.psm = psm;
			header.options = options & ~IIF_OPTION_FORCE;
			
			memset(header.name, 0, sizeof(header.name));
			
			if(strlen(filename) > sizeof(header.name))
				strncpy(header.name, filename, sizeof(header.name));
			else
				strcpy(header.name, filename);
			
			bool clear = false;
	
			for(uint32 i=0; i < sizeof(header.name); i++)
			{
				if(header.name[i] == '.') clear = true;
				if(clear) header.name[i] = 0;
			}
			
			uppercase(header.name);
			header.name[sizeof(header.name)-1] = 0;

			fseek(fpIIF, 0, SEEK_SET);
			fwrite(&header, sizeof(uint8), sizeof(header), fpIIF);
			fseek(fpIIF, sizeof(header), SEEK_SET);
			fwrite((void*)buffer, sizeof(uint8), bufferSize, fpIIF);
		}

		free((void*)buffer);

		fclose(fpIIF);

		return true;
}

