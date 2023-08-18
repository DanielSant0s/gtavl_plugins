#include <stdio.h>
#include <string.h>
#include "loadimage.h"
#include "saveiif.h"
#include "types.h"


#define IIT_VERSION	"1.0"

#define NUMBER_OF_FORMATS	7
const char* formats[NUMBER_OF_FORMATS] = {	"RGBA32", "RGB24", "RGBA16","CLUT8-RGBA32", 
										"CLUT8-RGBA16", "CLUT4-RGBA32", "CLUT4-RGBA16" };

#define NUMBER_OF_ARGS		5
char* args[] = {"-p", "-s", "-n", "-c", "-f"};

uint32 flag_psm = AUTO;
uint32 flag_swizzle = 0;
uint32 flag_noclut = 0;
uint32 flag_onlyclut = 0;
uint32 flag_force = 0;

int32 main(int32 argc, char *argv[])
{
	if(argc < 3)
	{
		if((argc == 2) && (!strcasecmp(argv[1], "-h")))
		{
			printf("ito image tool %s by Lukasz Bruun ( www.lukasz.dk )\n", IIT_VERSION);
			printf("Usage: iit [options] [input file] [ouput IIF file]\n");
			printf("Options:\n");
			printf("  -p <PSM>    Set Pixel Storage Mode (Default is input file PSM)\n");
			printf("  -s          Swizzle (IIF v2 only)\n");
			printf("  -n          Do not include CLUT, for CLUT sharing (IFF v2 only)\n");
			printf("  -c          Save CLUT only (IIF v2 only)\n");
			printf("  -f          Force IIF v1 format\n");
			printf("\nSupported PSM : %s %s %s %s\n", formats[0], formats[1], formats[2], formats[3]);
			printf("                %s %s %s\n", formats[4], formats[5], formats[6]);	
			printf("\nSupported input file formats : %s\n", GetSupportedFormats());
			return 0;
		}
		
		if(argc == 1)
		{
			printf("itt -h for help");
			return 0;
		}

		printf("Error: Atleast 2 arguments required\n");
		
		return 1;
	}
	else
	{
		uint32 save_options = 0;

		if(argc > 3)
		{

		for(uint32 i=1; i < argc-2; i++)
		{	
			int32 valid_arg = -1;
			int32 already_set = 0;
			for(uint32 a=0; a < NUMBER_OF_ARGS; a++)
			{
				if(!strcasecmp(argv[i], args[a]))
					valid_arg = a;
			}
			
			if(valid_arg < 0)
			{
				printf("Error: Unrecognized argument '%s'\n", argv[i]);
				return 1;
			}
			else
			{
				switch(valid_arg)
				{
					case 0: // -p
						if(flag_psm < AUTO) 
							already_set = 1;
						else
						{
							int32 valid_format = -1;
							
							i++;
							
							if(i == argc-2)
							{
								printf("Error: Too few arguments");
								return 1;
							}

							for(uint32 p=0; p < NUMBER_OF_FORMATS; p++)
							{
								if(!strcasecmp(argv[i], formats[p]))
									flag_psm = p;
							}
							
							if(flag_psm == AUTO)
							{
								printf("Error: '%s' is not a valid PSM format\n", argv[i]);
								return 1;
							}
						}
					break;

					case 1: // -s
						if(flag_swizzle)
							already_set = 1;
						else
							flag_swizzle = 1;
					break;

					case 2: // -n
						if(flag_noclut)
							already_set = 1;
						else
							flag_noclut = 1;
					break;

					case 3: // -c
						if(flag_onlyclut)
							already_set = 1;
						else
							flag_onlyclut = 1;
					break;

					case 4: // -f
						if(flag_force)
							already_set = 1;
						else
							flag_force = 1;
					break;


					default:
						printf("Internal Error 1\n");
						return 1;
					break;	

				}
				
				if(already_set)
				{
					printf("Error: Argument '%s' already set once\n", args[valid_arg]);
					return 1;
				}

			}
		}
		
		// Check Args

		if(flag_force && (flag_onlyclut || flag_noclut || flag_swizzle))
		{
			printf("Error: Argument '%s' can not be used together with '%s%', '%s', '%s'\n", args[4], args[1], args[2], args[3]);
		}
		
		if(flag_force) save_options |= IIF_OPTION_FORCE;
		if(flag_swizzle) save_options |= IIF_OPTION_SWIZZLE;
		if(flag_noclut) save_options |= IIF_OPTION_NOCLUT;
		if(flag_onlyclut) save_options |= IIF_OPTION_ONLYCLUT;

		} // if(argc > 3)
		
		Image image;

		if(!LoadImage(&image, argv[argc-2]))
		{
			FreeImage(&image);
			return 1;
		}

		if(flag_psm == AUTO) flag_psm = image.psm;

		if(!SaveIIF(argv[argc-1], &image, flag_psm, save_options))
		{
			FreeImage(&image);
			return -1;
		}
		
		FreeImage(&image);
	}

	return 0;
}


char* uppercase(char* str)
{
	char val = 'A'-'a';
	uint32 len = strlen(str);
	uint32 i;

	for(i=0; i < len; i++)
		str[i] += val;

	return str;
}

