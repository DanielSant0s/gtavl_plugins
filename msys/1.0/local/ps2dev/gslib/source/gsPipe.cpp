/*******************************
*                              *
*       GSLIB by Hiryu         *
*                              *
* gsPipe module:               *
* Based on GFX-Pipe by Vzzrzzn *
* (and modifications by Sjeep) *
*                              *
*******************************/

#include <tamtypes.h>
#include <kernel.h>

#include "gsDefs.h"
#include "gsPipe.h"
#include "hw.h"
#include "malloc.h"
#include "string.h"

//added by me
#include "stdio.h"

#define GSPIPE_MINSPACE 18     // minimum space left in pipeline before flush (must be at least 36 dwords)

static int hw_AlphaEnabled;
static int hw_ZBufferEnabled;
static int hw_ZTestEnabled;

static int hw_OriginX;
static int hw_OriginY;

/*
the rules:
    - the list is sendable via gsPipe::Flush after ANY gsPipe:: function call
    - each function must advance the giftag pointer to empty space
    - a function is guaranteed GSPIPE_MINSPACE dwords to work with;
      if it needs more, it must check against the remaining size
      of the pipeline ((m_CurrentDmaAddr + (m_MemSize / 2)) - m_CurrentGifTag)
    - a function must call gsPipe::FlushCheck after it is done adding to the queue
    - a function must update the size field of the dmatag
    - the current dmatag is of the 'count' style after a function returns
*/

// size must be less than 1MB
gsPipe::gsPipe(unsigned int size)
{
	//Set the fail-safe value
	m_MemSize=0;

    if (size < 0x1000)
	{
		printf("Requested size for gsPipe buffer was less than 0x1000 !\n");
        return;
	}

	if (m_Buffer != NULL)
		free(m_Buffer);

	m_Buffer=(unsigned long *)memalign(64,size);

	if (m_Buffer == NULL)
	{
		printf("gsPipe buffer could not be allocated !\n");
		return;
	}


    if ((int)m_Buffer & 0xf)	// this should never happen since we used memalign
	{
		printf("gsPipe buffer is not aligned !\n");
        return;
	}


    m_DmaPipe1 = m_Buffer;
    m_DmaPipe2 = m_DmaPipe1 + (size >> 4);

	m_MemSize = size;

    InitPipe(m_DmaPipe1);

    m_CurrentPipe = m_CurrentDmaAddr = m_DmaPipe1;
    m_CurrentGifTag = m_DmaPipe1 + 2;

	m_OriginX = hw_OriginX;
	m_OriginY = hw_OriginY;

	// setup default states for this pipe
	setAlphaEnable(GS_ENABLE);
	setZTestEnable(GS_DISABLE);
	setFilterMethod(GS_FILTER_NEAREST);

}


gsPipe::~gsPipe()
{
	if (m_Buffer != NULL)
		free(m_Buffer);
}


gsPipe::gsPipe(const gsPipe& copyPipe)
{
	operator= (copyPipe);
}

gsPipe& gsPipe::operator = (const gsPipe& copyPipe)
{
	if (&copyPipe != this)
	{
		// Copy all values, and buffer contents
		m_OriginX = copyPipe.m_OriginX;
		m_OriginY = copyPipe.m_OriginY;

		m_AlphaEnabled = copyPipe.m_AlphaEnabled;
		m_ZBufferEnabled = copyPipe.m_ZBufferEnabled;

		m_MemSize = copyPipe.m_MemSize;

		if (m_Buffer != NULL)
			free (m_Buffer);

		// Create new buffer of same size as gsPipe being copied
		m_Buffer = (unsigned long *)memalign(64, m_MemSize);

		// Copy the actual contents of the pipe buffer
		memcpy(m_Buffer, copyPipe.m_Buffer, m_MemSize);

		// Now setup the Pipe pointers to stick to the same relative positions
		m_DmaPipe1 = m_Buffer + (copyPipe.m_DmaPipe1 - copyPipe.m_Buffer);
		m_DmaPipe2 = m_Buffer + (copyPipe.m_DmaPipe2 - copyPipe.m_Buffer);

		m_CurrentPipe = m_Buffer + (copyPipe.m_CurrentPipe - copyPipe.m_Buffer);

		m_CurrentDmaAddr = m_Buffer + (copyPipe.m_CurrentDmaAddr - copyPipe.m_Buffer);
		m_CurrentGifTag = m_Buffer + (copyPipe.m_CurrentGifTag - copyPipe.m_Buffer);
	}

	return *this;
}

unsigned int gsPipe::getPipeSize(void)
{
	return m_MemSize;
}

void gsPipe::InitPipe(unsigned long *dmatadr)
{
    dmatadr[0] = 0x0000000070000000;
    dmatadr[1] = 0;
    //dmatadr[2] = 0x1000000000008000;
    //dmatadr[3] = 0x000000000000000e;
}


void gsPipe::ReInit(void)
{
	// Should we bother doing this conditionally ?
	// (ie: only if hardware state and gsPipe state differ)
	setZTestEnable(m_ZTestEnabled);
	setAlphaEnable(m_AlphaEnabled);
	setOrigin(m_OriginX, m_OriginY);
	Flush();
}


unsigned int gsPipe::getBytesLeft(void)
{
    unsigned int eob, cgt;

    eob = (unsigned int)m_CurrentPipe;
    eob += (m_MemSize >> 1);
    cgt = (unsigned int)m_CurrentGifTag;

    return eob - cgt;
}


/***********************
* Pipe Flush Functions *
***********************/

void gsPipe::FlushCheck(void)
{
    if ( getBytesLeft() < (GSPIPE_MINSPACE << 3))
    {
        Flush();
    }
}

void gsPipe::Flush(void)
{
    Dma02Wait();
    FlushCache(0);
    SendDma02(m_CurrentPipe);

    if (m_CurrentPipe == m_DmaPipe1)
    {
        InitPipe(m_DmaPipe2);
        m_CurrentPipe = m_CurrentDmaAddr = m_DmaPipe2;
        m_CurrentGifTag = m_CurrentDmaAddr + 2;
    }
    else
    {
        InitPipe(m_DmaPipe1);
        m_CurrentPipe = m_CurrentDmaAddr = m_DmaPipe1;
        m_CurrentGifTag = m_CurrentDmaAddr + 2;
    }
}

void gsPipe::FlushInt(void)
{
    Dma02Wait();
    iFlushCache(0);
    SendDma02(m_CurrentPipe);

    if (m_CurrentPipe == m_DmaPipe1)
    {
        InitPipe(m_DmaPipe2);
        m_CurrentPipe = m_CurrentDmaAddr = m_DmaPipe2;
        m_CurrentGifTag = m_CurrentDmaAddr + 2;
    }
    else
    {
        InitPipe(m_DmaPipe1);
        m_CurrentPipe = m_CurrentDmaAddr = m_DmaPipe1;
        m_CurrentGifTag = m_CurrentDmaAddr + 2;
    }
}


/***********************************
* ZBuffer Enable/Disable Functions *
***********************************/

void gsPipe::setZBuffer(unsigned long base, int psm, unsigned long enable)
{
	enable &= 0x01; // force to single-bit

    unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = GS_SET_ZBUF(base, psm, enable^1);	// notice - enable bit in reg is 0=enable, 1=disable
    m_CurrentGifTag[3] = GS_REG_ZBUF_1;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();

	// update the enabled state for this gsPipe and the hardware state mirror
	m_ZBufferEnabled = hw_ZBufferEnabled = enable;
}

void gsPipe::setZTestEnable(int enable)
{
	enable &= 0x01; // force to single-bit

	// Don't allow ZTest to be enabled, if the ZBuffer is disabled in hardware
	if ((hw_ZBufferEnabled == 0) || (m_ZBufferEnabled==0))
		enable=0;

    unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	if (enable)
		m_CurrentGifTag[2] = GS_SET_TEST(0,0,0,0,0,0,1,3); //0x00070000;
	else
	    m_CurrentGifTag[2] = GS_SET_TEST(0,0,0,0,0,0,1,1); //0x00030000;

    m_CurrentGifTag[3] = GS_REG_TEST_1;
    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();

	m_ZTestEnabled = hw_ZTestEnabled = enable;

}
void gsPipe::setAlphaEnable(int enable)
{
	enable &= 0x01; // force to single-bit

    unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 3);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;


	if (enable)
		m_CurrentGifTag[2] = GS_SET_ALPHA(GS_ALPHA_SOURCE, GS_ALPHA_FRAME, GS_ALPHA_SOURCE, GS_ALPHA_FRAME, 0x7F); //0x0000008000000044;
	else
		m_CurrentGifTag[2] = 0;

	m_CurrentGifTag[3] = GS_REG_ALPHA_1;

// CHECK - Should we write to PABE here ?	
	m_CurrentGifTag[4] = 0;
	m_CurrentGifTag[5] = GS_REG_PABE;

    m_CurrentGifTag = &m_CurrentGifTag[6];        // advance the packet pointer

    FlushCheck();

	m_AlphaEnabled = hw_AlphaEnabled = enable;
}

void gsPipe::setDither(unsigned long enable)
{
	unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = enable;
	m_CurrentGifTag[3] = GS_REG_DTHE;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();
}


void gsPipe::setColClamp(unsigned long enable)
{
	unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = enable;
	m_CurrentGifTag[3] = GS_REG_COLCLAMP;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();
}


void gsPipe::setPrModeCont(unsigned long enable)
{
	unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = enable;
	m_CurrentGifTag[3] = GS_REG_PRMODECONT;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();
}


void gsPipe::setDrawFrame(unsigned long base, unsigned long width , int psm, unsigned long mask)
{
	unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = GS_SET_FRAME(base, width, psm, mask);
	m_CurrentGifTag[3] = GS_REG_FRAME_1;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::setOrigin(int x, int y)
{
	m_OriginX = hw_OriginX = x;
	m_OriginY = hw_OriginY = y;

	unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = GS_SET_XYOFFSET(x, y);
	m_CurrentGifTag[3] = 0x18;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::setScissorRect(long x1, long y1, long x2, long y2)
{
	// should store the scissor area in member variables, so scissor area can be re-init'd when calling ReInit
	// and can also be copied to/from another gsPipe

    unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);

	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = GS_SET_SCISSOR(x1, x2, y1, y2);
	m_CurrentGifTag[3] = GS_REG_SCISSOR_1;
//	m_CurrentGifTag[2] = GS_SET_SCISSOR(x1, x2, y1, y2);
//	m_CurrentGifTag[3] = GS_REG_SCISSOR_2;

    m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer

    FlushCheck();
}


/***********************************
* Texture Upload & Setup Functions *
***********************************/

// send a byte-packed texture from RDRAM to VRAM
// TBP = VRAM_address
// TBW = buffer_width_in_pixels  -- dependent on pxlfmt
// xofs, yofs in units of pixels
// pxlfmt = 0x00 (32-bit), 0x02 (16-bit), 0x13 (8-bit), 0x14 (4-bit)
// wpxls, hpxls = width, height in units of pixels
// tex -- must be qword aligned !!!

#define IMAGE_MAX_QWORD	0x7FF0

void gsPipe::TextureUpload(unsigned int TBP, int TBW, int xofs, int yofs,
						   int pxlfmt, const unsigned char *tex, int wpxls, int hpxls)
{
    int numq;
    unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffff8fff0000) | (1 << 28) | ((dt & 0xffff) + 5); // change tag from refe to cnt

	// first, setup transfer
    m_CurrentGifTag[0] = GIF_SET_TAG(4,1,0,0,0,1); // 0x1000000000008004;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

    m_CurrentGifTag[2] = GS_SET_BITBLTBUF(0, 0, 0, (TBP/256) & 0x3fff, (TBW/64) & 0x3f, pxlfmt & 0x3f);
    m_CurrentGifTag[3] = GS_REG_BITBLTBUF;
    m_CurrentGifTag[4] = GS_SET_TRXPOS(0, 0, xofs, yofs, 0);
    m_CurrentGifTag[5] = GS_REG_TRXPOS;
    m_CurrentGifTag[6] = GS_SET_TRXREG(wpxls, hpxls);
    m_CurrentGifTag[7] = GS_REG_TRXREG;
    m_CurrentGifTag[8] = 0;
    m_CurrentGifTag[9] = GS_REG_TRXDIR;

	m_CurrentDmaAddr = &m_CurrentGifTag[10];
	m_CurrentGifTag = m_CurrentDmaAddr + 2;
	
	FlushCheck();

    numq = wpxls * hpxls;
    switch (pxlfmt)
    {
    case GS_PSMCT32:
		numq = (numq >> 2) + ((numq & 0x03) != 0 ? 1 : 0);
		break;
	case GS_PSMCT24:
		numq = (numq / 3)  + ((numq & 0x02) != 0 ? 1 : 0);
		break;
	case GS_PSMCT16:
	case GS_PSMCT16S:
		numq = (numq >> 3) + ((numq & 0x07) != 0 ? 1 : 0);
		break;

    case GS_PSMT8: 
		numq = (numq >> 4) + ((numq & 0x0f) != 0 ? 1 : 0);
		break;

    case GS_PSMT4:
		numq = (numq >> 5) + ((numq & 0x1f) != 0 ? 1 : 0);
		break;

    default:   numq = 0;
    }
	
	// Send IMAGE mode giftags until all image data is sent
	while(numq)
	{
		int currq;
		
		if(numq > IMAGE_MAX_QWORD) 
			currq = IMAGE_MAX_QWORD;
		else 
			currq = numq;
		
		// dmatag to transfer image mode giftag
		*m_CurrentDmaAddr = (1 << 28) | 1;
		
		// image mode giftag
	    m_CurrentGifTag[0]= 0x0800000000000000 + currq; // IMAGE mode giftag. Flg = 10b, nloop = currq
		m_CurrentGifTag[1]= 0;
		
		// dmatag to transfer image data
	    m_CurrentDmaAddr = &m_CurrentGifTag[2]; // set up dma tag for image transfer. next = tex addr, id = 11b, qwc = numq
		m_CurrentDmaAddr[0] = ((((unsigned long)((int)tex)) & 0x7fffffff) << 32) | (0x0000000030000000 + currq);
	    m_CurrentDmaAddr[1] = 0;
		
	    m_CurrentDmaAddr += 2; // + 16 bytes
		m_CurrentDmaAddr[0] = 0x0000000070000000; // next dma tag, id = end.
		m_CurrentDmaAddr[1] = 0x0000000000000000;
		m_CurrentGifTag = m_CurrentDmaAddr + 2;
		
		FlushCheck();
		
		numq -= currq;
		tex += (currq * 16);
	}
	FlushCheck();
	TextureFlush();
}

// send a byte-packed texture from VRAM to RDRAM
// TBP = VRAM_address
// TBW = buffer_width_in_pixels  -- dependent on pxlfmt
// xofs, yofs in units of pixels
// pxlfmt = 0x00 (32-bit), 0x02 (16-bit), 0x13 (8-bit), 0x14 (4-bit)
// wpxls, hpxls = width, height in units of pixels

void gsPipe::TextureDownload(unsigned int TBP, int TBW, int xofs, int yofs, int pxlfmt, unsigned char* tex, int wpxls, int hpxls)
{
    int numq;
    unsigned long dt = *m_CurrentDmaAddr;
    *m_CurrentDmaAddr = (dt & 0xffffffff8fff0000) | (1 << 28) | ((dt & 0xffff) + 5); // change tag from refe to cnt

	// first, setup transfer
    m_CurrentGifTag[0] = GIF_SET_TAG(4,1,0,0,0,1); // 0x1000000000008004;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

    m_CurrentGifTag[2] = GS_SET_BITBLTBUF(0, 0, 0, (TBP/256) & 0x3fff, (TBW/64) & 0x3f, pxlfmt & 0x3f);
    m_CurrentGifTag[3] = GS_REG_BITBLTBUF;
    m_CurrentGifTag[4] = GS_SET_TRXPOS(0, 0, xofs, yofs, 0);
    m_CurrentGifTag[5] = GS_REG_TRXPOS;
    m_CurrentGifTag[6] = GS_SET_TRXREG(wpxls, hpxls);
    m_CurrentGifTag[7] = GS_REG_TRXREG;
    m_CurrentGifTag[8] = 1;
    m_CurrentGifTag[9] = GS_REG_TRXDIR;

	m_CurrentDmaAddr = &m_CurrentGifTag[10];
	m_CurrentGifTag = m_CurrentDmaAddr + 2;
	
	FlushCheck();

    numq = wpxls * hpxls;
    switch (pxlfmt)
    {
    case GS_PSMCT32:
		numq = (numq >> 2) + ((numq & 0x03) != 0 ? 1 : 0);
		break;
	case GS_PSMCT24:
		numq = (numq / 3)  + ((numq & 0x02) != 0 ? 1 : 0);
		break;
	case GS_PSMCT16:
	case GS_PSMCT16S:
		numq = (numq >> 3) + ((numq & 0x07) != 0 ? 1 : 0);
		break;

    case GS_PSMT8: 
		numq = (numq >> 4) + ((numq & 0x0f) != 0 ? 1 : 0);
		break;

    case GS_PSMT4:
		numq = (numq >> 5) + ((numq & 0x1f) != 0 ? 1 : 0);
		break;

    default:   numq = 0;
    }
	
	// Send IMAGE mode giftags until all image data is received
	while(numq)
	{
		int currq;
		
		if(numq > IMAGE_MAX_QWORD) 
			currq = IMAGE_MAX_QWORD;
		else 
			currq = numq;
		
		// dmatag to transfer image mode giftag
		*m_CurrentDmaAddr = (1 << 28) | 1;
		
		// image mode giftag
	    m_CurrentGifTag[0]= 0x0800000000000000 + currq; // IMAGE mode giftag. Flg = 10b, nloop = currq
		m_CurrentGifTag[1]= 0;
		
		// dmatag to transfer image data
	    m_CurrentDmaAddr = &m_CurrentGifTag[2]; // set up dma tag for image transfer. next = tex addr, id = 11b, qwc = numq
		m_CurrentDmaAddr[0] = ((((unsigned long)((int)tex)) & 0x7fffffff) << 32) | (0x0000000030000000 + currq);
	    m_CurrentDmaAddr[1] = 0;
		
	    m_CurrentDmaAddr += 2; // + 16 bytes
		m_CurrentDmaAddr[0] = 0x0000000070000000; // next dma tag, id = end.
		m_CurrentDmaAddr[1] = 0x0000000000000000;
		m_CurrentGifTag = m_CurrentDmaAddr + 2;
		
		FlushCheck();
		
		numq -= currq;
		tex += (currq * 16);
	}
	FlushCheck();
	TextureFlush();
}

void gsPipe::TextureFlush(void)
{
	// do texture flush
	// Access the TEXFLUSH register with anything to flush the texture
	unsigned long dt = *m_CurrentDmaAddr;
	*m_CurrentDmaAddr = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 2);
	
	m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,0,1); // 0x1000000000008001;
	m_CurrentGifTag[1] = 0xfffffffffffffffe;
	
	m_CurrentGifTag[2] = 0xBAD;
	m_CurrentGifTag[3] = GS_REG_TEXFLUSH;
	
	m_CurrentGifTag = &m_CurrentGifTag[4];        // advance the packet pointer
	
	Flush();
}

void gsPipe::setFilterMethod(int FilterMethod)
{
	m_FilterMethod = FilterMethod;
}

void gsPipe::TextureSet(unsigned int tbp, int tbw, gsTexSize texwidth, gsTexSize texheight, u32 tpsm, u32 cbp, u32 csm, u32 cbw, u32 cpsm)
{

	unsigned long dt = *m_CurrentDmaAddr;
	*(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 9);

	m_CurrentGifTag[0] = GIF_SET_TAG(8,1,0,0,0,1); // 0x1000000000008008;
    m_CurrentGifTag[1] = 0xfffffffffffffffe;

	m_CurrentGifTag[2] = 256/64;
    m_CurrentGifTag[3] = GS_REG_TEXCLUT;			// texclut  <- not really necessary but if i get lazy in future ...

    m_CurrentGifTag[4] = 0;
    m_CurrentGifTag[5] = GS_REG_TEXFLUSH;			// texflush

	m_CurrentGifTag[6] = GS_SET_TEXA(128, 1, 0);
    m_CurrentGifTag[7] = GS_REG_TEXA;			// texa: TA0 = 128, AEM = 1, TA1 = 0

	m_CurrentGifTag[8] = GS_SET_TEX1(0, 0, m_FilterMethod, m_FilterMethod, 0, 0, 0);
    m_CurrentGifTag[9] = GS_REG_TEX1_1;			// tex1_1

																				// Should TCC be 0 or 1 ? (was/is 1)
	m_CurrentGifTag[10]= GS_SET_TEX0(tbp/256, tbw/64, tpsm, texwidth, texheight, 1, 0, cbp/256, cpsm, csm, 0, 1);
	m_CurrentGifTag[11]= GS_REG_TEX0_1;			// tex0_1

	m_CurrentGifTag[12]= GS_SET_CLAMP(1, 1, 0, 2^texwidth, 0, 2^texheight); // CHECK should this be 2^texsize ??
    m_CurrentGifTag[13]= GS_REG_CLAMP_1;			// clamp_1

    m_CurrentGifTag[14]= GS_SET_ALPHA(GS_ALPHA_SOURCE, GS_ALPHA_FRAME, GS_ALPHA_SOURCE, GS_ALPHA_FRAME, 0x7F);
    m_CurrentGifTag[15]= GS_REG_ALPHA_1;			// alpha_1: A = Cs, B = Cd, C = As, D = Cd

    m_CurrentGifTag[16]= 0x0000000000000000;
    m_CurrentGifTag[17]= GS_REG_PABE;			// pabe

	m_CurrentGifTag = &m_CurrentGifTag[18];        // advance the packet pointer

    FlushCheck();
}


/**********************************
* Here are all the line functions *
**********************************/

void gsPipe::Line(int x1, int y1,
				  int x2, int y2,
				  unsigned z, unsigned colour)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 3);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,1,4); // 0x4400000000008001;
    m_CurrentGifTag[1] = 0xffffffffffff5d10;
    m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_LINE, 0, 0, 0, m_AlphaEnabled, 0, 0, 0, 0); //0x0000000000000001;
    m_CurrentGifTag[3] = GS_SET_COLQ(colour);
    m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z);
    m_CurrentGifTag[5] = GS_SET_XYZ(x2,y2,z);
    m_CurrentGifTag = &m_CurrentGifTag[6];        // advance the packet pointer

    FlushCheck();
}

/**************************************
* Here are all the triangle functions *
**************************************/


void gsPipe::TriangleLine(int x1, int y1, unsigned z1, unsigned c1,
						  int x2, int y2, unsigned z2, unsigned c2,
						  int x3, int y3, unsigned z3, unsigned c3)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 6);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);
	x3 = (x3 + m_OriginX);
	y3 = (y3 + m_OriginY);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,1,9); // 0x9400000000008001;
    m_CurrentGifTag[1] = 0xfffffff515151d10;
    m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_LINESTRIP, 1, 0, 0, m_AlphaEnabled, 0, 0, 0, 0); //0x000000000000000a;
    m_CurrentGifTag[3] = GS_SET_COLQ(c1);
    m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z1);
    m_CurrentGifTag[5] = GS_SET_COLQ(c2);
    m_CurrentGifTag[6] = GS_SET_XYZ(x2,y2,z2);
    m_CurrentGifTag[7] = GS_SET_COLQ(c3);
    m_CurrentGifTag[8] = GS_SET_XYZ(x3,y3,z3);
    m_CurrentGifTag[9] = m_CurrentGifTag[3];
    m_CurrentGifTag[10] = m_CurrentGifTag[4];
    m_CurrentGifTag = &m_CurrentGifTag[12];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::TriangleFlat(int x1, int y1, unsigned z1,
						  int x2, int y2, unsigned z2,
						  int x3, int y3, unsigned z3,
						  unsigned colour)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 4);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);
	x3 = (x3 + m_OriginX);
	y3 = (y3 + m_OriginY);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,1,5); // 0x5400000000008001;
    m_CurrentGifTag[1] = 0xfffffffffff5dd10;
    m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 0, 0, m_AlphaEnabled, 0, 0, 0, 0); //0x0000000000000003;
    m_CurrentGifTag[3] = GS_SET_COLQ(colour);
    m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z1);
    m_CurrentGifTag[5] = GS_SET_XYZ(x2,y2,z2);
    m_CurrentGifTag[6] = GS_SET_XYZ(x3,y3,z3);
    m_CurrentGifTag = &m_CurrentGifTag[8];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::TriangleGouraud(int x1, int y1, unsigned z1, unsigned c1,
							 int x2, int y2, unsigned z2, unsigned c2,
							 int x3, int y3, unsigned z3, unsigned c3)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 5);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);
	x3 = (x3 + m_OriginX);
	y3 = (y3 + m_OriginY);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,1,7); // 0x7400000000008001;
    m_CurrentGifTag[1] = 0xfffffffff5151d10;
    m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE, 1, 0, 0, m_AlphaEnabled, 0, 0, 0, 0); //0x000000000000000b;
    m_CurrentGifTag[3] = GS_SET_COLQ(c1);
    m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z1);
    m_CurrentGifTag[5] = GS_SET_COLQ(c2);
    m_CurrentGifTag[6] = GS_SET_XYZ(x2,y2,z2);
    m_CurrentGifTag[7] = GS_SET_COLQ(c3);
    m_CurrentGifTag[8] = GS_SET_XYZ(x3,y3,z3);
    m_CurrentGifTag = &m_CurrentGifTag[10];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::TriangleTexture(int x1, int y1, unsigned z1, unsigned u1, unsigned v1,
							 int x2, int y2, unsigned z2, unsigned u2, unsigned v2,
							 int x3, int y3, unsigned z3, unsigned u3, unsigned v3,
							 unsigned colour)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 5);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);
	x3 = (x3 + m_OriginX);
	y3 = (y3 + m_OriginY);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,1,8); // 0x8400000000008001;
    m_CurrentGifTag[1] = 0xffffffff53535310;
    m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 1, 0, m_AlphaEnabled, 0, 1, 0, 0); //0x0000000000000153;
    m_CurrentGifTag[3] = GS_SET_COLQ(colour);
    m_CurrentGifTag[4] = GS_SET_UV(u1,v1);
    m_CurrentGifTag[5] = GS_SET_XYZ(x1,y1,z1);
    m_CurrentGifTag[6] = GS_SET_UV(u2,v2);
    m_CurrentGifTag[7] = GS_SET_XYZ(x2,y2,z2);
    m_CurrentGifTag[8] = GS_SET_UV(u3,v3);
    m_CurrentGifTag[9] = GS_SET_XYZ(x3,y3,z3);
    m_CurrentGifTag = &m_CurrentGifTag[10];        // advance the packet pointer

    FlushCheck();
}

/***************************************
* Here are all the rectangle functions *
***************************************/

void gsPipe::RectFlat(int x1, int y1,
					  int x2, int y2,
					  unsigned z, unsigned colour)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 4);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);

    m_CurrentGifTag[0] = GIF_SET_TAG(1,1,0,0,1,6); // 0x6400000000008001;
    m_CurrentGifTag[1] = 0xffffffffff55dd10;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE_STRIP, 0, 0, 0, m_AlphaEnabled, 0, 0, 0, 0);
    m_CurrentGifTag[3] = GS_SET_COLQ(colour);
    m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z);
    m_CurrentGifTag[5] = GS_SET_XYZ(x2,y1,z);
    m_CurrentGifTag[6] = GS_SET_XYZ(x1,y2,z);
    m_CurrentGifTag[7] = GS_SET_XYZ(x2,y2,z);
    m_CurrentGifTag = &m_CurrentGifTag[8];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::RectLine(int x1, int y1,
					  int x2, int y2,
					  unsigned z, unsigned colour)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 5);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);

	m_CurrentGifTag[0] = GIF_SET_TAG(1, 1, 0, 0, 1, 8);
    m_CurrentGifTag[1] = 0xfffffffff5555510;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_LINESTRIP, 0, 0, 0, m_AlphaEnabled, 0, 0, 0, 0);
    m_CurrentGifTag[3] = GS_SET_COLQ(colour);
    m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z);
    m_CurrentGifTag[5] = GS_SET_XYZ(x2,y1,z);
    m_CurrentGifTag[6] = GS_SET_XYZ(x2,y2,z);
    m_CurrentGifTag[7] = GS_SET_XYZ(x1,y2,z);
	m_CurrentGifTag[8] = GS_SET_XYZ(x1,y1,z);
	m_CurrentGifTag[9] = 0; // NOP
    m_CurrentGifTag = &m_CurrentGifTag[10];        // advance the packet pointer

    FlushCheck();
}

void gsPipe::RectTexture(int x1, int y1, u32 u1, u32 v1, 
						 int x2, int y2, u32 u2, u32 v2, 
						 u32 z, u32 colour)
{
	unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 4);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);

	m_CurrentGifTag[0] = GIF_SET_TAG(1, 1, 0, 0, 1, 6);
	m_CurrentGifTag[1] = 0xffffffffff535310;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, m_AlphaEnabled, 0, 1, 0, 0);
	m_CurrentGifTag[3] = GS_SET_COLQ(colour);
	m_CurrentGifTag[4] = GS_SET_UV(u1, v1);
	m_CurrentGifTag[5] = GS_SET_XYZ(x1,y1,z);
	m_CurrentGifTag[6] = GS_SET_UV(u2, v2);
	m_CurrentGifTag[7] = GS_SET_XYZ(x2,y2,z);
	m_CurrentGifTag = &m_CurrentGifTag[8];

	FlushCheck();
}

void gsPipe::RectGouraud(int x1, int y1, unsigned c1,
						 int x2, int y2, unsigned c2,
						 unsigned z)
{
	unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 6);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);

	m_CurrentGifTag[0] = GIF_SET_TAG(1, 1, 0, 0, 1, 10);
	m_CurrentGifTag[1] = 0xFFFFFFF5151d1d10;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE_STRIP, 1, 0, 0, m_AlphaEnabled, 0, 0, 0, 0);
	m_CurrentGifTag[3] = GS_SET_COLQ(c1);
	m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z);
	m_CurrentGifTag[5] = GS_SET_COLQ(c2);
	m_CurrentGifTag[6] = GS_SET_XYZ(x2,y1,z);
	m_CurrentGifTag[7] = GS_SET_COLQ(c2);
	m_CurrentGifTag[8] = GS_SET_XYZ(x1,y2,z);
	m_CurrentGifTag[9] = GS_SET_COLQ(c1);
	m_CurrentGifTag[10] = GS_SET_XYZ(x2,y2,z);
	m_CurrentGifTag[11] = 0; // NOP
	m_CurrentGifTag = &m_CurrentGifTag[12];

	FlushCheck();
}

/***********************************
* Here are all the point functions *
***********************************/

void gsPipe::Point(int x, int y, unsigned z, unsigned colour)
{
    unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 3);

	x = (x + m_OriginX);
	y = (y + m_OriginY);

	m_CurrentGifTag[0] = GIF_SET_TAG(1, 1, 0, 0, 1, 4);
	m_CurrentGifTag[1] = 0xFFFFFFFFFFFF510;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_POINT, 0, 0, 0, m_AlphaEnabled, 0, 0, 0, 0);
	m_CurrentGifTag[3] = GS_SET_COLQ(colour);
	m_CurrentGifTag[4] = GS_SET_XYZ(x,y,z);
	m_CurrentGifTag[5] = 0; // NOP
	m_CurrentGifTag = &m_CurrentGifTag[6];

	FlushCheck();
}


/**************************************
* Here are all the TriStrip functions *
**************************************/
void gsPipe::TriStripGouraud(int x1, int y1, unsigned z1, unsigned c1,
							 int x2, int y2, unsigned z2, unsigned c2,
							 int x3, int y3, unsigned z3, unsigned c3,
							 int x4, int y4, unsigned z4, unsigned c4)
{
	unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 6);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);
	x3 = (x3 + m_OriginX);
	y3 = (y3 + m_OriginY);
	x4 = (x4 + m_OriginX);
	y4 = (y4 + m_OriginY);


	m_CurrentGifTag[0] = GIF_SET_TAG(1, 1, 0, 0, 1, 10);
	m_CurrentGifTag[1] = 0xFFFFFFF5151d1d10;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE_STRIP, 1, 0, 0, m_AlphaEnabled, 0, 0, 0, 0);
	m_CurrentGifTag[3] = GS_SET_COLQ(c1);
	m_CurrentGifTag[4] = GS_SET_XYZ(x1,y1,z1);
	m_CurrentGifTag[5] = GS_SET_COLQ(c2);
	m_CurrentGifTag[6] = GS_SET_XYZ(x2,y2,z2);
	m_CurrentGifTag[7] = GS_SET_COLQ(c3);
	m_CurrentGifTag[8] = GS_SET_XYZ(x3,y3,z3);
	m_CurrentGifTag[9] = GS_SET_COLQ(c4);
	m_CurrentGifTag[10] = GS_SET_XYZ(x4,y4,z4);
	m_CurrentGifTag[11] = 0; // NOP
	m_CurrentGifTag = &m_CurrentGifTag[12];

	FlushCheck();
}



void gsPipe::TriStripGouraudTexture(int x1, int y1, unsigned z1, unsigned u1, unsigned v1, unsigned c1,
									int x2, int y2, unsigned z2, unsigned u2, unsigned v2, unsigned c2,
									int x3, int y3, unsigned z3, unsigned u3, unsigned v3, unsigned c3,
									int x4, int y4, unsigned z4, unsigned u4, unsigned v4, unsigned c4)
{

	unsigned long dt = *(m_CurrentDmaAddr);
    *(m_CurrentDmaAddr) = (dt & 0xffffffffffff0000) | ((dt & 0xffff) + 8);

	x1 = (x1 + m_OriginX);
	y1 = (y1 + m_OriginY);
	x2 = (x2 + m_OriginX);
	y2 = (y2 + m_OriginY);
	x3 = (x3 + m_OriginX);
	y3 = (y3 + m_OriginY);
	x4 = (x4 + m_OriginX);
	y4 = (y4 + m_OriginY);

	m_CurrentGifTag[0] = GIF_SET_TAG(1, 1, 0, 0, 1, 14);
	m_CurrentGifTag[1] = 0xfff531531d31d310;
	m_CurrentGifTag[2] = GS_SET_PRIM(GS_PRIM_TRIANGLE_STRIP, 1, 1, 0, m_AlphaEnabled, 0, 1, 0, 0);
	m_CurrentGifTag[3] = GS_SET_COLQ(c1);
	m_CurrentGifTag[4] = GS_SET_UV(u1, v1);
	m_CurrentGifTag[5] = GS_SET_XYZ(x1,y1,z1);
	m_CurrentGifTag[6] = GS_SET_COLQ(c2);
	m_CurrentGifTag[7] = GS_SET_UV(u2, v2);
	m_CurrentGifTag[8] = GS_SET_XYZ(x2,y2,z2);
	m_CurrentGifTag[9] = GS_SET_COLQ(c3);
	m_CurrentGifTag[10] = GS_SET_UV(u3, v3);
	m_CurrentGifTag[11] = GS_SET_XYZ(x3,y3,z3);
	m_CurrentGifTag[12] = GS_SET_COLQ(c4);
	m_CurrentGifTag[13] = GS_SET_UV(u4, v4);
	m_CurrentGifTag[14] = GS_SET_XYZ(x4,y4,z4);
	m_CurrentGifTag[15] = 0; // NOP
	m_CurrentGifTag = &m_CurrentGifTag[16];

	FlushCheck();
}
