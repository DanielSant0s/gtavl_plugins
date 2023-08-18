// iif.h : IIF file format header

#pragma pack (1)



typedef struct{
	char ID[4]; // 'IIF1' - IIFv (v = version of iif)
	unsigned int width;
	unsigned int height;
	unsigned int psm;
} IIF_HEADER;



// PS2 PIXEL FORMATS

typedef struct{
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
	unsigned char Alpha;
} RGBA32_PIXEL;

typedef struct{
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
} RGB24_PIXEL;

typedef struct{
	unsigned short Red:5;
	unsigned short Green:5;
	unsigned short Blue:5;
	unsigned short Alpha:1;

} RGB16_PIXEL;
