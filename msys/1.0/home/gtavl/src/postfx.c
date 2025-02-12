#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CTexture.h>
#include "renderbuffer.h"
#include <common.h>
#include <math.h>

#include <CPad.h>

#include <CCam.h>

#include "bios_alloc.h"

static void (*CGame_ShutdownRenderWare)();
static void (*CMovingThings_Render)();
static void (*CPostEffects_Render)();
static void (*CGame_Init1)(const char*);

static RwRaster *pColorPaletteRaster, *pDOFTable, *pDepthTempBuffer;

static void CSkyEdgePostEffects_ScaleDepthMask(RwRaster* palette, uint8_t initial_value, uint8_t factors[16]) {
    int i, j, k = initial_value;

    static const uint8_t factor_order[16][2] = {
        { 0,   7 }, 
        { 16,  23 }, 
        { 8,   15 }, 
        { 24,  39 }, 
        { 48,  55 }, 
        { 40,  47 }, 
        { 56,  71 }, 
        { 80,  87 }, 
        { 72,  79 }, 
        { 88,  103 }, 
        { 112, 119 }, 
        { 104, 111 }, 
        { 120, 135 }, 
        { 144, 151 }, 
        { 136, 143 }, 
        { 152, 255 }
    };

    RwRGBA *pal_rgba = (RwRGBA *)RwRasterLock(palette, 0, rwRASTERLOCKWRITE);

    for (j = 0; j < 16; j++) {
	    for (i = factor_order[j][0]; i < factor_order[j][1]+1; i++)
	    {
	    	pal_rgba[i].r = k;
	    	pal_rgba[i].g = k;
	    	pal_rgba[i].b = k;
	    	pal_rgba[i].a = k;

	    	if (k < 128 || factors[j] >= 128) {
	    		k += factors[j];
	    	} else if (k != 128) {
	    		k = 128;
	    	}
	    }  
    }
}

static void CSkyEdgePostEffects_Init(const char* a1) {
    CGame_Init1(a1);

    printf("Generating depth tables\n");

    pDepthTempBuffer = RwRasterCreate(64, 32, 32, rwRASTERTYPETEXTURE);

    pDOFTable = RwRasterCreate(16, 16, 32, rwRASTERTYPETEXTURE);

    uint8_t dof_scale[16] = { 24, 24, 24, 24, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    CSkyEdgePostEffects_ScaleDepthMask(pDOFTable, 0, dof_scale);    

    pColorPaletteRaster = RwRasterCreate(16, 16, 32, rwRASTERTYPETEXTURE);

    uint8_t fog_scale[16] = { 4, 2, 6, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    CSkyEdgePostEffects_ScaleDepthMask(pColorPaletteRaster, 0, fog_scale);    

    // 0 means full opaque
    // 128 means full transparent

    // 1st plane: 0  - 7
    // 2st plane: 16 - 23
    // 3st plane: 8  - 15
    // 4st plane: 24 - 40
    // 5st plane: 48 - 56
    // 6st plane: 40 - 48
    // 7st plane: 56 - 72

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

static void CSkyEdgePostEffects_RenderDepthOfField(RwRaster* table, uint32_t sprite_col) {
    RwRGBA sprite_color = *(RwRGBA*)(&sprite_col);

    uint32_t zbuffer = ((skyFrameBit? _rwDMAFlipData_db->draw01.zbuf1.ZBP : _rwDMAFlipData_db->draw11.zbuf1.ZBP) << 5);
    uint32_t fbuffer = ((skyFrameBit? _rwDMAFlipData_db->draw01.frame1.FBP : _rwDMAFlipData_db->draw11.frame1.FBP) << 5);
    
    CSkyEdgePostEffects_CopyDepthBuffer(CHANNEL_BLUE, table);
    
    if (!_rwDMAOpenVIFPkt(RWDMA_FIXUP, 35)) return;

    // Set tests and states

    ADDTOPKT(GIF_PACKED_AD, GIF_SET_TAG(6, 1, 0, 0, GIF_PACKED, 1));

    ADDTOPKT(GS_TEST_1, GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, 1));

    // Output = (((DST_RGB - SRC_RGB) * DST_ALPHA) >> 7) + SRC_RGB
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

float old_up_z = 0.0f, new_up_z = 0.0f;

static uint8_t dof_scale[16] = { 24, 24, 24, 24, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

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

    if (TheCamera.placeable.m_matrix->up.z < -0.1f) {
        new_up_z = TheCamera.placeable.m_matrix->up.z;
        
        if (old_up_z != new_up_z) {
            dof_scale[0] = (uint8_t)(24-(-1*((new_up_z+0.1f)*24)));
            dof_scale[1] = (uint8_t)(24-(-1*((new_up_z+0.1f)*24)));
            dof_scale[2] = (uint8_t)(12-(-1*((new_up_z+0.1f)*12)));
            dof_scale[3] = (uint8_t)(12-(-1*((new_up_z+0.1f)*12)));
            dof_scale[4] = (uint8_t)(12-(-1*((new_up_z+0.1f)*12)));

            CSkyEdgePostEffects_ScaleDepthMask(pDOFTable, (uint8_t)(128-(-1*(((new_up_z+0.1f)*4)*128)) < 0? 0 : 128-(-1*(((new_up_z+0.1f)*4)*128))), dof_scale);  
        }

        CSkyEdgePostEffects_RenderDepthOfField(pDOFTable, GS_RGBA(0xA0, 0xA0, 0xA0, 0xA0));
        old_up_z = new_up_z;
    }

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
