#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CTexture.h>
#include "renderbuffer.h"
#include <common.h>
#include <math.h>

#include <CPad.h>

#include "bios_alloc.h"

static void (*CGame_ShutdownRenderWare)();
static void (*CMovingThings_Render)();
static void (*CPostEffects_Render)();
static void (*CGame_Init1)(const char*);

static RwRaster *pColorPaletteRaster, *pDOFTable, *pDepthTempBuffer;

static void CSkyEdgePostEffects_Init(const char* a1) {
    int i, j;

    CGame_Init1(a1);

    printf("Generating depth tables\n");

    pDepthTempBuffer = RwRasterCreate(64, 32, 32, rwRASTERTYPETEXTURE);

    pDOFTable = RwRasterCreate(16, 16, 32, rwRASTERTYPETEXTURE);

    RwRGBA *dof_pal = (RwRGBA *)RwRasterLock(pDOFTable, 0, rwRASTERLOCKWRITE);

	j = 0;
    
	for (i = 0; i < 8; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;

		if (j < 128) {
			j += 24;
		} else if (j != 128) {
			j = 128;
		}
	}      

	for (i = 16; i < 24; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;

		if (j < 128) {
			j += 24;
		} else if (j != 128) {
			j = 128;
		}
	}      

	for (i = 8; i < 16; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;

		if (j < 128) {
			j += 24;
		} else if (j != 128) {
			j = 128;
		}
	}      

	for (i = 24; i < 40; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;

		if (j < 128) {
			j += 24;
		} else if (j != 128) {
			j = 128;
		}
	}      

	for (i = 48; i < 56; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;

		if (j < 128) {
			j += 12;
		} else if (j != 128) {
			j = 128;
		}
	}      

	for (i = 40; i < 48; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 56; i < 72; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 80; i < 88; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 72; i < 80; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 88; i < 104; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 112; i < 120; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 104; i < 112; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 120; i < 136; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      
	
	for (i = 144; i < 152; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 136; i < 144; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}      

	for (i = 152; i < 256; i++)
	{
		dof_pal[i].r = j;
		dof_pal[i].g = j;
		dof_pal[i].b = j;
		dof_pal[i].a = j;
	}

    pColorPaletteRaster = RwRasterCreate(16, 16, 32, rwRASTERTYPETEXTURE);
    
    RwRGBA *in_pallete = (RwRGBA *)RwRasterLock(pColorPaletteRaster, 0, rwRASTERLOCKWRITE);

    // 0 means full fog
    // 128 means no fog

    // 1st plane: 0  - 7
    // 2st plane: 16 - 23
    // 3st plane: 8  - 15
    // 4st plane: 24 - 40
    // 5st plane: 48 - 56
    // 6st plane: 40 - 48
    // 7st plane: 56 - 72

    
	for (i = 0, j = 4; i < 8; i++, j += 4)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 16; i < 24; i++, j += 2)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 8; i < 16; i++, j += 6)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 24; i < 40; i++, j += 2)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 48; i < 56; i++, j++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 40; i < 48; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 56; i < 72; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 80; i < 88; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 72; i < 80; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 88; i < 104; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 112; i < 120; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 104; i < 112; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 120; i < 136; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 144; i < 152; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 136; i < 144; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

	for (i = 152; i < 256; i++)
	{
        in_pallete[i].r = j;
        in_pallete[i].g = j;
        in_pallete[i].b = j;
        in_pallete[i].a = j;
	}

    printf("Depth tables generated\n");
}

static void CSkyEdgePostEffects_CopyDepthBuffer(int in_channel, RwRaster* palette) {
    uint32_t msb, lsb, pal_addr, buf_addr;

    uint32_t zbuffer = ((skyFrameBit? _rwDMAFlipData_db->draw01.zbuf1.ZBP : _rwDMAFlipData_db->draw11.zbuf1.ZBP) << 5);
    uint32_t fbuffer = ((skyFrameBit? _rwDMAFlipData_db->draw01.frame1.FBP : _rwDMAFlipData_db->draw11.frame1.FBP) << 5);

    RpSkyTexCacheAccessSpeculate(palette);
    RpSkyTexCacheAccessRaster(palette, false);
    RpSkyTexGetTex0(palette, &msb, &lsb);

    pal_addr = (lsb & 0x3FFF);

    uint32_t page = 0;

    uint32_t x, y;
    for (y = 0; y < 448; y += 32) {
        for (x = 0; x < 640; x += 64) {

            RpSkyTexCacheAccessSpeculate(pDepthTempBuffer);
            RpSkyTexCacheAccessRaster(pDepthTempBuffer, false);
            RpSkyTexGetTex0(pDepthTempBuffer, &msb, &lsb);

            buf_addr = (lsb & 0x3FFF);

            if( _rwDMAOpenVIFPkt(RWDMA_FIXUP, 5) )
            {
                ADDTOPKT(GIF_AD, GIF_SET_TAG(4, 1, 0, 0, GIF_PACKED, 1));

                ADDTOPKT(GS_BITBLTBUF, GS_SET_BITBLTBUF(zbuffer+page, 1, GS_PSMZ_32, buf_addr, 1, GS_PSM_32));

                ADDTOPKT(GS_TRXPOS, GS_SET_TRXPOS(0, 0, 0, 0, 0)); // ...then set the offset in the buffer, and pixel transmission order...

                ADDTOPKT(GS_TRXREG, (uint64_t)(64) | ((uint64_t)(32) << 32)); // ...the width and height of the transmission for the dest buffer...

                ADDTOPKT(GS_TRXDIR, 2L); // ...and finally the direction of transmission, local-to-local...

            }

            RpSkyTexCacheAccessSpeculate(palette);
            RpSkyTexCacheAccessRaster(palette, false);
            RpSkyTexGetTex0(palette, &msb, &lsb);

            pal_addr = (lsb & 0x3FFF);

            performChannelCopy(in_channel, CHANNEL_ALPHA, x, y, buf_addr, 640, 448, pal_addr);

            page += 32;
        }
    }
}

static void CSkyEdgePostEffects_RenderFog(RwRaster* table, uint32_t tl_c, uint32_t tr_c, uint32_t bl_c, uint32_t br_c) {
    RwRGBA tlc = *(RwRGBA*)(&tl_c);
    RwRGBA trc = *(RwRGBA*)(&tr_c);
    RwRGBA blc = *(RwRGBA*)(&bl_c);
    RwRGBA brc = *(RwRGBA*)(&br_c);

    CSkyEdgePostEffects_CopyDepthBuffer(CHANNEL_BLUE, table);

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 15);

    ADDTOPKT(GIF_PACKED_AD, GIF_SET_TAG(3, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_TEST_1, GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, 1));

    // The GS's alpha blending formula is fixed but it contains four variables that can be reconfigured:
    // Output = (((A - B) * C) >> 7) + D
    // A, B, and D are colors and C is an alpha value. Their specific values come from the ALPHA register:
    //       A                B                C                   D
    //   0   Source RGB       Source RGB       Source alpha        Source RGB
    //   1   Framebuffer RGB  Framebuffer RGB  Framebuffer alpha   Framebuffer RGB
    //   2   0                0                FIX                 0
    //   3   Reserved         Reserved         Reserved            Reserved


    ADDTOPKT(GS_ALPHA_1, GS_SET_ALPHA_1(1, 0, 1, 0, 0));

    ADDTOPKT(GS_XYOFFSET_1, GS_SET_XYOFFSET_1(0, 0));

    ADDTOPKT(GS_RGBAQ | (GS_XYZ2 << 4), 
             GIF_SET_TAG(4, 1, 1, GS_SET_PRIM(GS_PRIM_TRISTRIP, 1, 0, 0, 1, 0, 0, 0, 0), GIF_PACKED, 2)
    );

	{
        //                     BARG

		// RGBAQ
        ADDTOPKT((uint64_t)((tlc.b) | ((uint64_t)tlc.a << 32)), (uint64_t)((tlc.r) | ((uint64_t)tlc.g << 32)));
		// XYZ2
        ADDTOPKT((uint64_t)(1), (uint64_t)((((0 << 4)) | (((uint64_t)(0 << 4)) << 32))));
		// RGBAQ
		ADDTOPKT((uint64_t)((trc.b) | ((uint64_t)trc.a << 32)), (uint64_t)((trc.r) | ((uint64_t)trc.g << 32)));
		// XYZ2
        ADDTOPKT((uint64_t)(1), (uint64_t)((((640 << 4)) | (((uint64_t)(0 << 4)) << 32))));	

		// RGBAQ
        ADDTOPKT((uint64_t)((blc.b) | ((uint64_t)blc.a << 32)), (uint64_t)((blc.r) | ((uint64_t)blc.g << 32)));
		// XYZ2
        ADDTOPKT((uint64_t)(1), (uint64_t)((((0 << 4)) | (((uint64_t)(448 << 4)) << 32))));
		// RGBAQ
        ADDTOPKT((uint64_t)((brc.b) | ((uint64_t)brc.a << 32)), (uint64_t)((brc.r) | ((uint64_t)brc.g << 32)));
		// XYZ2
        ADDTOPKT((uint64_t)(1), (uint64_t)((((640 << 4)) | (((uint64_t)(448 << 4)) << 32))));
	}

    ADDTOPKT(GIF_AD, GIF_SET_TAG(1, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_XYOFFSET_1, skyXyoffset_1);
}

static void CSkyEdgePostEffects_RenderDepthOfField(RwRaster* table, uint32_t sprite_col, int pass_count) {
    RwRGBA sprite_color = *(RwRGBA*)(&sprite_col);

    uint32_t zbuffer = ((skyFrameBit? _rwDMAFlipData_db->draw01.zbuf1.ZBP : _rwDMAFlipData_db->draw11.zbuf1.ZBP) << 5);
    uint32_t fbuffer = ((skyFrameBit? _rwDMAFlipData_db->draw01.frame1.FBP : _rwDMAFlipData_db->draw11.frame1.FBP) << 5);
    
    CSkyEdgePostEffects_CopyDepthBuffer(CHANNEL_BLUE, table);
    
    if (!_rwDMAOpenVIFPkt(RWDMA_FIXUP, 35)) return;

    // Set tests and states

    ADDTOPKT(GIF_PACKED_AD, GIF_SET_TAG(6, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_TEST_1, GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, 1));

    ADDTOPKT(GS_ALPHA_1, GS_SET_ALPHA_1(DST_RGB, SRC_RGB, DST_ALPHA, SRC_RGB, 0));

    ADDTOPKT(GS_XYOFFSET_1, GS_SET_XYOFFSET_1(0, 0));

    ADDTOPKT(GS_TEX0_1, GS_SET_TEX0(fbuffer, 10, GS_PSM_32, 10, 9, 0, 1, 0, 0, 0, 0, 0));

    ADDTOPKT(GS_TEXFLUSH, GS_SET_TEXFLUSH(1));

    ADDTOPKT(GS_FRAME_1, GS_SET_FRAME((skyFrameBit? _rwDMAFlipData_db->draw01.zbuf1.ZBP : _rwDMAFlipData_db->draw11.zbuf1.ZBP), 10, GS_PSM_32, 0x00));

    // Draw first downscaled frame buffer sprite into temp buffer

    ADDTOPKT((GS_RGBAQ) | (GS_UV << 4) | (GS_XYZ2 << 8) | (GS_UV << 12) | (GS_XYZ2 << 16), 
             GIF_SET_TAG(1, 1, 1, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0), GIF_PACKED, 5)
    );

    {
        // RGBAQ
        ADDTOPKT((uint64_t)((sprite_color.b) | ((uint64_t)sprite_color.a << 32)), (uint64_t)((sprite_color.r) | ((uint64_t)sprite_color.g << 32)));
        //// UV
        ADDTOPKT(0, GS_SET_ST(((0) << 4), ((0) << 4)));
	    // XYZ2
        ADDTOPKT(1, (uint64_t)((0) << 4) | ((uint64_t)((0) << 4) << 32));
	    // UV
        ADDTOPKT(0, GS_SET_ST(((640) << 4), ((448) << 4)));
	    // XYZ2
        ADDTOPKT(1, (uint64_t)((320) << 4) | ((uint64_t)((224) << 4) << 32));
    }

    // Set states for the second downscale

    ADDTOPKT(GIF_PACKED_AD, GIF_SET_TAG(1, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_TEX0_1, GS_SET_TEX0(zbuffer, 10, GS_PSM_32, 10, 9, 0, 1, 0, 0, 0, 0, 0));

    // Draw second downscaled frame buffer sprite into temp buffer

    ADDTOPKT((GS_RGBAQ) | (GS_UV << 4) | (GS_XYZ2 << 8) | (GS_UV << 12) | (GS_XYZ2 << 16), 
             GIF_SET_TAG(1, 1, 1, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0), GIF_PACKED, 5)
    );

    {
        // RGBAQ
        ADDTOPKT((uint64_t)((sprite_color.b) | ((uint64_t)sprite_color.a << 32)), (uint64_t)((sprite_color.r) | ((uint64_t)sprite_color.g << 32)));
        //// UV
        ADDTOPKT(0, GS_SET_ST(((0) << 4), ((0) << 4)));
	    // XYZ2
        ADDTOPKT(1, (uint64_t)((0) << 4) | ((uint64_t)((0) << 4) << 32));
	    // UV
        ADDTOPKT(0, GS_SET_ST(((320) << 4), ((224) << 4)));
	    // XYZ2
        ADDTOPKT(1, (uint64_t)((320) << 4) | ((uint64_t)((224) << 4) << 32));
    }

    // Restore states

    ADDTOPKT(GIF_AD, GIF_SET_TAG(2, 1, 0, 0, GIF_PACKED, 1));
	// FRAME
    ADDTOPKT(GS_FRAME_1, GS_SET_FRAME((skyFrameBit? _rwDMAFlipData_db->draw01.frame1.FBP : _rwDMAFlipData_db->draw11.frame1.FBP), 10, GS_PSM_32, 0x00));

    ADDTOPKT(GS_XYOFFSET_1, skyXyoffset_1);

    ADDTOPKT(GIF_AD, GIF_SET_TAG(3, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_XYOFFSET_1, GS_SET_XYOFFSET_1(0, 0));

    ADDTOPKT(GS_TEX0_1, GS_SET_TEX0(zbuffer, 10, GS_PSM_32, 10, 9, 0, 0, 0, 0, 0, 0, 0));

    ADDTOPKT(GS_TEXFLUSH, GS_SET_TEXFLUSH(1));

    ADDTOPKT((GS_UV) | (GS_XYZ2 << 4) | (GS_UV << 8) | (GS_XYZ2 << 12), 
             GIF_SET_TAG(1, 1, 1, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0), GIF_PACKED, 4)
    );

    {
        // UV
        ADDTOPKT(0, GS_SET_ST(((0) << 4), ((0) << 4)));
	    // XYZ2
        ADDTOPKT(1, (uint64_t)((0) << 4) | ((uint64_t)((0) << 4) << 32));
	    // UV
        ADDTOPKT(0, GS_SET_ST(((320) << 4), ((224) << 4)));
	    // XYZ2
        ADDTOPKT(1, (uint64_t)((640) << 4) | ((uint64_t)((448) << 4) << 32));
    }

    ADDTOPKT(GIF_AD, GIF_SET_TAG(1, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_XYOFFSET_1, skyXyoffset_1);
}


static void CSkyEdgePostEffects_Update() {
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)false);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)false);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)false);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);    
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)true);

    //CSkyEdgePostEffects_RenderFog(pColorPaletteRaster, 
    //                              GS_RGBA(0x40, 0x40, 0x40, 0x00), 
    //                              GS_RGBA(0x40, 0x40, 0x40, 0x00), 
    //                              GS_RGBA(0x40, 0x40, 0x40, 0x00), 
    //                              GS_RGBA(0x40, 0x40, 0x40, 0x00));

    CSkyEdgePostEffects_RenderDepthOfField(pDOFTable, 
                                           GS_RGBA(0xC0, 0xC0, 0xC0, 0x00),
                                           2);

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)false);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)true);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)true);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);    
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)false);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
    //CPostEffects_Render();
}

static void CSkyEdgePostEffects_Render() {


    CMovingThings_Render();
}

static void CSkyEdgePostEffects_Shutdown() {
    CGame_ShutdownRenderWare();
}

void InjectSkyEdgePostEffects() {
    //MakeNop(0x246F40); // Disable PostFX

    CGame_Init1 =              (void (*)(const char*))ReadCall(0x242B9C);
    CPostEffects_Render =      (void (*)())ReadCall(0x246F40);
    CMovingThings_Render =     (void (*)())ReadCall(0x246EA8);
    CGame_ShutdownRenderWare = (void (*)())ReadCall(0x247388);

    RedirectCall(0x242B9C, &CSkyEdgePostEffects_Init);
    RedirectCall(0x246990, &CSkyEdgePostEffects_Update);
    RedirectCall(0x246EA8, &CSkyEdgePostEffects_Render);
    RedirectCall(0x247388, &CSkyEdgePostEffects_Shutdown);
}
