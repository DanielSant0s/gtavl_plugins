
#include <rwcore.h>
#include <secore.h>

void SetScissorRect(Rect* rect){
    unsigned long int tmp;

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = (rect->left & 0x7ffl) | ((rect->right - 1) << 16) | ((rect->top & 0x7ffl) << 32) | (((unsigned long int)(rect->bottom - 1)) << 48);
    ADDTOPKT(GS_SCISSOR_1, tmp);
}

void SetTests(pixeltests test){
    unsigned long int tmp;

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = test.pack;

    ADDTOPKT(GS_TEST_1, tmp);
}

void SetAlphaMode(uint64_t AlphaMode)
{
    unsigned long int tmp;

	_rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = AlphaMode;

    ADDTOPKT(GS_ALPHA_1, tmp);
}

void SetPABE(uint8_t PerPixel)
{
    unsigned long long int tmp;

	_rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    tmp = GIF_TAG(1,1,0,0,0,1);
    ADDTOPKT(GIF_AD, tmp);

    tmp = (uint64_t)PerPixel;

    ADDTOPKT(GS_PABE, tmp);
}


void GetBackBuffer(RwRaster *raster)
{
    if( _rwDMAOpenVIFPkt(RWDMA_FIXUP, 5) )
    {
        uint64_t tmp, tmp1;
        uint64_t frameAddress;
        uint64_t rasterAddress;
        uint64_t width, height;
        uint64_t pixelFormat;
        RwUInt32 msb, lsb;

        tmp = 4L |       /* NLOOP */
            (1L << 15) | /* EOP */
            (0L << 46) | /* PRE */
            (0L << 58) | /* FLG */
            (1L << 60);  /* NREG */

        tmp1 = (0xEL << (64 - 64)); /* A+D */
        ADDTOPKT(tmp1, tmp);

        if( skyFrameBit & 0x1 )
        {
            tmp = *(uint64_t *)0x829900; // &_rwDMAFlipData.db.draw01.frame1
        }
        else
        {
            tmp = *(uint64_t *)0x829A70; // &_rwDMAFlipData.db.draw11.frame1
        }    
        
        frameAddress = tmp & 0x1FF;

        /*
         * Frame address is word/2k. We need /64...
         */
        frameAddress <<= 5;

        /*
         * Find the base pointer of our raster...
         */
        RpSkyTexGetTex0(raster, &msb, &lsb);
        width = (lsb >> 14) & 0x3F;
		rasterAddress = lsb & 0x3FFF;
		
        pixelFormat = (lsb >> 20) & 0x3F;
        
        /*
         * Now setup the registers. First the BITBLTBUF register...
         */
        tmp = (frameAddress & 0x3FFF) |        /* src buffer base pointer */
            (width << 16) |                    /* src buffer width */
            (pixelFormat << 24) |              /* src pixel storage format */
            ((rasterAddress & 0x3FFF) << 32) | /* dest buffer base pointer */
            (width << 48) |                    /* dest buffer width */
            (pixelFormat << 56);               /* dest pixel storage format */
        
        ADDTOPKT(GS_BITBLTBUF, tmp);
        

        /*
         * ...then set the offset in the buffer, and pixel transmission
         * order...
         */
        ADDTOPKT(GS_TRXPOS, 0L);
        

        /*
         * ...the width and height of the transmission for the dest buffer...
         */
        width = (uint64_t)(raster->width);
        height = (uint64_t)(raster->height);

        tmp = width |       
            (height << 32);

        ADDTOPKT(GS_TRXREG, tmp);
        

        /*
         * ...and finally the direction of transmission, local-to-local...
         */
        ADDTOPKT(GS_TRXDIR, 2L);
        
    }
}


void GetBackBufferOffset(RwRaster *raster, int x, int y)
{
    sceGsFrame* buf_frame;

    if( _rwDMAOpenVIFPkt(RWDMA_FIXUP, 5) )
    {
        uint64_t tmp;
        uint64_t frameAddress;
        uint64_t rasterAddress;
        uint64_t width, height;
        uint64_t pixelFormat;
        RwUInt32 msb, lsb;

        tmp = 4L |       /* NLOOP */
            (1L << 15) | /* EOP */
            (0L << 46) | /* PRE */
            (0L << 58) | /* FLG */
            (1L << 60);  /* NREG */

        ADDTOPKT(GIF_AD, tmp);

        if( skyFrameBit & 0x1 )
        {
            buf_frame = (sceGsFrame *)0x829900; // &_rwDMAFlipData.db.draw01.frame1
        }
        else
        {
            buf_frame = (sceGsFrame *)0x829A70; // &_rwDMAFlipData.db.draw11.frame1
        }    

        frameAddress = buf_frame->FBP;

        /*
         * Frame address is word/2k. We need /64...
         */
        frameAddress <<= 5;

        /*
         * Find the base pointer of our raster...
         */
        RpSkyTexGetTex0(raster, &msb, &lsb);
        width = (lsb >> 14) & 0x3F;
		rasterAddress = lsb & 0x3FFF;
		
        pixelFormat = (lsb >> 20) & 0x3F;
        
        /*
         * Now setup the registers. First the BITBLTBUF register...
         */
        tmp = (frameAddress & 0x3FFF) |        /* src buffer base pointer */
            (buf_frame->FBW << 16) |                    /* src buffer width */
            (pixelFormat << 24) |              /* src pixel storage format */
            ((rasterAddress & 0x3FFF) << 32) | /* dest buffer base pointer */
            (width << 48) |                    /* dest buffer width */
            (pixelFormat << 56);               /* dest pixel storage format */
        
        ADDTOPKT(GS_BITBLTBUF, tmp);
        

        /*
         * ...then set the offset in the buffer, and pixel transmission
         * order...
         */
        ADDTOPKT(GS_TRXPOS, GS_SET_TRXPOS(x, y, 0, 0, 0));
        

        /*
         * ...the width and height of the transmission for the dest buffer...
         */
        width = (uint64_t)(raster->width);
        height = (uint64_t)(raster->height);

        tmp = width |       
            (height << 32);

        ADDTOPKT(GS_TRXREG, tmp);
        

        /*
         * ...and finally the direction of transmission, local-to-local...
         */
        ADDTOPKT(GS_TRXDIR, 2L);
        
    }
}


bool GsRenderStateSet(int state, void* value) {
    pixeltests def_test;
    def_test.pack = *(unsigned long long int*)0x667930;

    switch (state)
    {
    case GS_SCISSORTESTENABLE:
        {
            Rect screen_rect;
            screen_rect.left = 0;
            screen_rect.right = 640; 
            screen_rect.top = 0;
            screen_rect.bottom = 448;

            if (!value) {
                SetScissorRect(&screen_rect);
            } 

        }
        break;
    case GS_ALPHATESTENABLE:
        {
            def_test.data.ate = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_ALPHATESTMETHOD:
        {
            def_test.data.atst = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_ALPHATESTFAIL:
        {
            def_test.data.afail = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_BLENDMETHOD:
        {

        }
        break;
    default:
        break;
    }

    return true;

}
