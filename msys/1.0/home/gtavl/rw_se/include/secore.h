#ifndef RWSE_CORE_H
#define RWSE_CORE_H

typedef struct {
    unsigned long long int ate    : 1;
    unsigned long long int atst   : 3;
    unsigned long long int aref   : 8;
    unsigned long long int afail  : 2;
    unsigned long long int date   : 1;
    unsigned long long int datm   : 1;
    unsigned long long int zte    : 1;
    unsigned long long int ztst   : 2;
} rwTest;

typedef union {
    unsigned long long int pack;
    rwTest data;
} pixeltests;

typedef struct { 
    int left;    // This is the upper-left corner x-coordinate.
    int top;     // The upper-left corner y-coordinate.
    int right;   // The lower-right corner x-coordinate.
    int bottom;  // The lower-right corner y-coordinate.
} Rect;

#define ALPHA_SRC 0
#define ALPHA_DST 1
#define ALPHA_ZERO 2
#define ALPHA_FIX 2
#define ALPHA(A,B,C,D,FIX) ( (((uint64_t)(A))&3) | ((((uint64_t)(B))&3)<<2) | ((((uint64_t)(C))&3)<<4) | ((((uint64_t)(D))&3)<<6) | ((((uint64_t)(FIX)))<<32UL) )//(A - B)*C >> 7 + D

#define SRC_RGB 0
#define DST_RGB 1
#define ZERO_RGB 2

#define SRC_ALPHA 0
#define DST_ALPHA 1
#define ALPHA_FIX 2

#define GS_ALPHA_BLEND_NORMAL (ALPHA(ALPHA_SRC,ALPHA_DST,ALPHA_SRC,ALPHA_DST,0x00))
#define GS_ALPHA_BLEND_ADD_NOALPHA    (ALPHA(ALPHA_SRC,ALPHA_ZERO,ALPHA_FIX,ALPHA_DST,0x80))
#define GS_ALPHA_BLEND_ADD    (ALPHA(ALPHA_SRC,ALPHA_ZERO,ALPHA_SRC,ALPHA_DST,0x00))

#define GS_RGBA(r, g, b, a) (r | (g << 8) | (b << 16) | (a << 24))

enum GsRenderState {
    GS_SCISSORTESTENABLE,
    GS_ALPHATESTENABLE,
    GS_ALPHATESTMETHOD,
    GS_ALPHATESTFAIL,
    GS_DESTALPHATESTENABLE,
    GS_DESTALPHATESTMETHOD,
    GS_DEPTHTESTENABLE,
    GS_DEPTHTESTMETHOD,
    GS_BLENDMETHOD,
};

enum GsAlphaTestFail {
    GS_ALPHATESTFAILKEEP,
    GS_ALPHATESTFAILFBONLY,
    GS_ALPHATESTFAILZBONLY,
    GS_ALPHATESTFAILRGBONLY
};

enum GsAlphaTestMethod
{
    GS_ALPHATESTMETHODNEVER,         
        /**<Always fail the test */
    GS_ALPHATESTMETHODALWAYS,   
        /**<Always pass the test */
    GS_ALPHATESTMETHODLESS,          
        /**<Accept the new pixel if its alpha value is less than the value of
         *  the reference value */
    GS_ALPHATESTMETHODLESSEQUAL,     
        /**<Accept the new pixel if its alpha value is less than or equal to 
         *  the value of the reference value */
    GS_ALPHATESTMETHODEQUAL,         
        /**<Accept the new pixel if its alpha value equals the value of the
         *  reference value */
    GS_ALPHATESTMETHODGREATEREQUAL,  
        /**<Accept the new pixel if its alpha value is greater than or equal
         *  to the value of the reference value */
    GS_ALPHATESTMETHODGREATER,       
        /**<Accept the new pixel if its alpha value is greater than the value 
         *  of the reference value */
    GS_ALPHATESTMETHODNOTEQUAL      
        /**<Accept the new pixel if its alpha value does not equal the value of 
         *  the reference value */
};

enum GsDepthTestMethod
{
    GS_DEPTHTESTMETHODNEVER,         
        /**<Always fail the test */
    GS_DEPTHTESTMETHODALWAYS,   
        /**<Always pass the test */
    GS_DEPTHTESTMETHODGREATEREQUAL,  
        /**<Accept the new pixel if its alpha value is greater than or equal
         *  to the value of the reference value */
    GS_DEPTHTESTMETHODGREATER,       
        /**<Accept the new pixel if its alpha value is greater than the value 
         *  of the reference value */
};

void SetScissorRect(Rect* rect);

bool GsRenderStateSet(int state, void* value);

void SetPABE(uint8_t PerPixel);

void seGetBackBufferOffset(RwRaster *raster, int x, int y);

void SetAlphaMode(uint64_t AlphaMode);

extern sceGsDBuffDc *_rwDMAFlipData_db;

typedef enum
{
	CHANNEL_RED,
	CHANNEL_GREEN,
	CHANNEL_BLUE,
	CHANNEL_ALPHA,
} ColourChannels;

void performChannelCopy(ColourChannels channelIn, ColourChannels channelOut, uint32_t blockX, uint32_t blockY, uint32_t source, uint32_t width, uint32_t height, uint32_t palette);

#endif
