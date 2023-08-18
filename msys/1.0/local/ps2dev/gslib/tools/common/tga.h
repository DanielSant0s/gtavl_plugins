// tga.h : TGA file format header

#pragma pack (1)

typedef struct
{
    unsigned char  identsize;          // size of ID field that follows 18 byte header (0 usually)
    unsigned char  colourmaptype;      // type of colour map 0=none, 1=has palette
    unsigned char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourmapstart;     // first colour map entry in palette
    short colourmaplength;    // number of colours in palette
    unsigned char  colourmapbits;      // number of bits per palette entry 15,16,24,32

    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels
    unsigned char bits;               // image bits per pixel 8,16,24,32
    unsigned char descriptor;         // image descriptor bits (vh flip bits)
    
    // pixel data follows header
    
} TGA_HEADER;

// TGA PIXEL FORMATS

typedef struct{
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Alpha;
} TGA32_PIXEL;


typedef struct{
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
} TGA24_PIXEL;
