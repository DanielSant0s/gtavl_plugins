#pragma pack (1)

typedef struct{
	
	// overall texture size is in pixels, since there is no reason
	// why overall texture size has to be in GS_TEX units (ie: a power of 2)
	char ID[4];	//"BFNT"

	unsigned int TexWidth;	// overall width of texture - in pixels
	unsigned int TexHeight;	// overall width of texture - in pixels

	unsigned int PSM;	// Colour Depth of font Texture (in GS_PSM)

	unsigned int	NumXChars;	// number of characters horizontally in grid
	unsigned int NumYChars;	// number of characters vertically in grid

	unsigned int CharGridWidth;	// width of one char grid - in pixels 
	unsigned int CharGridHeight; // height of one char grid - in pixels
} FNT_HEADER;
