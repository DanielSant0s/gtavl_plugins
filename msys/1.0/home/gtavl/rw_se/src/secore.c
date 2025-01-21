
#include <stdio.h>
#include <rwcore.h>
#include <secore.h>

void SetScissorRect(Rect* rect){
    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    ADDTOPKT(GIF_AD, GIF_SET_TAG(1,1,0,0,0,1));

    ADDTOPKT(GS_SCISSOR_1, GS_SET_SCISSOR((rect->left & 0x7ffl), 
                                          (rect->right - 1), 
                                          (rect->top & 0x7ffl), 
                                          (rect->bottom - 1)));
}



void SetTests(pixeltests test)
{
    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    ADDTOPKT(GIF_AD, GIF_SET_TAG(1,1,0,0,0,1));

    ADDTOPKT(GS_TEST_1, test.pack);
}

void SetAlphaMode(uint64_t AlphaMode)
{
	_rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    ADDTOPKT(GIF_AD, GIF_SET_TAG(1,1,0,0,0,1));

    ADDTOPKT(GS_ALPHA_1, AlphaMode);
}

void SetPABE(uint8_t PerPixel)
{
	_rwDMAOpenVIFPkt(RWDMA_FIXUP, 2);

    ADDTOPKT(GIF_AD, GIF_SET_TAG(1,1,0,0,0,1));

    ADDTOPKT(GS_PABE, (uint64_t)PerPixel);
}

sceGsDBuffDc *_rwDMAFlipData_db = (sceGsDBuffDc *)0x8298A0;

void seGetBackBufferOffset(RwRaster *raster, int x, int y)
{
    sceGsFrame* buf_frame;

    if( _rwDMAOpenVIFPkt(RWDMA_FIXUP, 5) )
    {
        uint64_t frameAddress;
        uint64_t rasterAddress;
        uint64_t width;
        uint64_t pixelFormat;
        uint32_t msb, lsb;

        ADDTOPKT(GIF_AD, GIF_SET_TAG(4, 1, 0, 0, GIF_PACKED, 1));

        if( skyFrameBit & 0x1 ) {
            buf_frame = &_rwDMAFlipData_db->draw01.frame1;
        } else {
            buf_frame = &_rwDMAFlipData_db->draw11.frame1;
        }   

        frameAddress = buf_frame->FBP << 5; // Frame address is word/2k. We need /64...

        RpSkyTexGetTex0(raster, &msb, &lsb); // Find the base pointer of our raster...
        width = (lsb >> 14) & 0x3F;
		rasterAddress = lsb & 0x3FFF;

        pixelFormat = (lsb >> 20) & 0x3F;
        

        // Now setup the registers. First the BITBLTBUF register...
        
        ADDTOPKT(GS_BITBLTBUF, GS_SET_BITBLTBUF(frameAddress & 0x3FFF, 
                                                buf_frame->FBW, 
                                                pixelFormat, 
                                                rasterAddress & 0x3FFF, 
                                                width, 
                                                pixelFormat));

        ADDTOPKT(GS_TRXPOS, GS_SET_TRXPOS(x, y, 0, 0, 0)); // ...then set the offset in the buffer, and pixel transmission order...
        
        ADDTOPKT(GS_TRXREG, GS_SET_TRXREG(raster->width, raster->height)); // ...the width and height of the transmission for the dest buffer...
        
        ADDTOPKT(GS_TRXDIR, 2L); // ...and finally the direction of transmission, local-to-local...
        
    }
}

static inline uint32_t lzw(uint32_t val)
{
	uint32_t res;
	__asm__ __volatile__ ("   plzcw   %0, %1    " : "=r" (res) : "r" (val));
	return(res);
}

void set_tw_th(int w, int h, int *tw, int *th)
{
	*tw = 31 - (lzw(w) + 1);
	if(w > (1<<*tw))
		(*tw)++;

	*th = 31 - (lzw(h) + 1);
	if(h > (1<<*th))
		(*th)++;
}

// Channel copy a block
void performChannelCopy(ColourChannels channelIn, ColourChannels channelOut, uint32_t blockX, uint32_t blockY, uint32_t source, uint32_t width, uint32_t height, uint32_t palette)
{
    sceGsFrame* buf_frame;
    uint64_t frameAddress;
    int tw, th;

    if( skyFrameBit & 0x1 ) {
        buf_frame = &_rwDMAFlipData_db->draw01.frame1;
    } else {
        buf_frame = &_rwDMAFlipData_db->draw11.frame1;
    }    

    frameAddress = buf_frame->FBP;

	// For the BLUE and ALPHA channels, we need to offset our 'U's by 8 texels
	const uint32_t horz_block_offset = (channelIn == CHANNEL_BLUE || channelIn == CHANNEL_ALPHA);
	// For the GREEN and ALPHA channels, we need to offset our 'T's by 2 texels
	const uint32_t vert_block_offset = (channelIn == CHANNEL_GREEN || channelIn == CHANNEL_ALPHA);

	const uint32_t clamp_horz = horz_block_offset ? 8 : 0;
	const uint32_t clamp_vert = vert_block_offset ? 2 : 0;

    if (!_rwDMAOpenVIFPkt(RWDMA_FIXUP, 500)) return;

    ADDTOPKT(GIF_PACKED_AD, GIF_SET_TAG(5, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_XYOFFSET_1, GS_SET_XYOFFSET_1(0, 0));

    set_tw_th(width, height, &tw, &th);

    ADDTOPKT(GS_TEX0_1, GS_SET_TEX0(source, 1, GS_PSM_8, tw, th, 1, 1, palette, GS_PSM_32, 0, 0, 1));

    ADDTOPKT(GS_CLAMP_1, GS_SET_CLAMP(3, 3, 0xF7, clamp_horz, 0xFD, clamp_vert));

    ADDTOPKT(GS_TEXFLUSH, GS_SET_TEXFLUSH(1));

	uint32_t frame_mask;
	switch (channelOut)
	{
		case CHANNEL_RED:
			frame_mask = ~0x000000FF;
			break;
		case CHANNEL_GREEN:
			frame_mask = ~0x0000FF00;
			break;
		case CHANNEL_BLUE:
			frame_mask = ~0x00FF0000;
			break;
		case CHANNEL_ALPHA:
			frame_mask = ~0xFF000000;
			break;
	}

    ADDTOPKT(GS_FRAME_1, GS_SET_FRAME(frameAddress, 10, GS_PSM_32, frame_mask));

    ADDTOPKT((GS_UV) | (GS_XYZ2 << 4) | (GS_UV << 8) | (GS_XYZ2 << 12), 
             GIF_SET_TAG(96, 1, 1, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0), GIF_PACKED, 4)
    );

    int y;
	for (y = 0; y < 32; y += 2)
	{
		if (((y % 4) == 0) ^ (vert_block_offset == 1)) // Even (4 16x2 sprites)
		{
            int x;
			for (x = 0; x < 64; x += 16)
			{
				// UV
                ADDTOPKT(0, GS_SET_ST(8 + ((8 + x * 2) << 4), 8 + ((y * 2) << 4)));
				// XYZ2
                ADDTOPKT(1, (uint64_t)((x + blockX) << 4) | ((uint64_t)((y + blockY) << 4) << 32));
				// UV
                ADDTOPKT(0, GS_SET_ST(8 + ((24 + x * 2) << 4), 8 + ((2 + y * 2) << 4)));
				// XYZ2
                ADDTOPKT(1, (uint64_t)((x + 16 + blockX) << 4) | ((uint64_t)((y + 2 + blockY) << 4) << 32));
			}
		}
		else // Odd (Eight 8x2 sprites)
		{
            int x;
			for (x = 0; x < 64; x += 8)
			{
				// UV
                ADDTOPKT(0, GS_SET_ST(8 + ((4 + x * 2) << 4), 8 + ((y * 2) << 4)));
				// XYZ2
                ADDTOPKT(1, (uint64_t)((x + blockX) << 4) | ((uint64_t)((y + blockY) << 4) << 32));
				// UV
                ADDTOPKT(0, GS_SET_ST(8 + ((12 + x * 2) << 4), 8 + ((2 + y * 2) << 4)));
				// XYZ2
                ADDTOPKT(1, (uint64_t)((x + 8 + blockX) << 4) | ((uint64_t)((y + 2 + blockY) << 4) << 32));
			}
		}
	}

    ADDTOPKT(GIF_AD, GIF_SET_TAG(3, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_CLAMP_1, GS_SET_CLAMP(1, 1, 0x00, 0x0, 0x00, 0x00));
	// FRAME
    ADDTOPKT(GS_FRAME_1, GS_SET_FRAME(frameAddress, 10, GS_PSM_32, 0x00));

    ADDTOPKT(GS_XYOFFSET_1, skyXyoffset_1);
    
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
    case GS_DESTALPHATESTENABLE:
        {
            def_test.data.date = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_DESTALPHATESTMETHOD:
        {
            def_test.data.datm = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_DEPTHTESTENABLE:
        {
            def_test.data.zte = (int)value;
            SetTests(def_test);
        }
        break;
    case GS_DEPTHTESTMETHOD:
        {
            def_test.data.ztst = (int)value;
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
