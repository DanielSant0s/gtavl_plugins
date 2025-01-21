/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 3.0
 */
/*
 *                      Emotion Engine Library
 *                          Version 0.2.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         eestruct.h
 *                         develop library
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.1.0
 *       0.2.0          2003/9/25       hana        change comment
 */

#ifndef __eestruct__
#define __eestruct__

#ifdef __cplusplus
extern "C" {
#endif



/*-dma--------------------------------------------------*/

#define DMA_VIF0        0
#define DMA_VIF1        1
#define DMA_GIF         2
#define DMA_fromIPU     3
#define DMA_toIPU       4
#define DMA_SIF0        5
#define DMA_SIF1        6
#define DMA_SIF2        7
#define DMA_fromSPR     8
#define DMA_toSPR       9



/*-vif0--------------------------------------------------*/

#define VIF0_SET_CODE(immediate, num, cmd, irq) ((uint32_t)(immediate) | ((uint32_t)(num) << 16) | ((uint32_t)(cmd) << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_NOP(irq) ((uint32_t)(irq) << 31)

#define VIF0_SET_STCYCL(wl, cl, irq) ((uint32_t)(cl) | ((uint32_t)(wl) << 8)  | ((uint32_t)0x01 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_ITOP(itop, irq) ((uint32_t)(itop) | ((uint32_t)0x04 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_STMOD(stmod, irq) ((uint32_t)(stmod) | ((uint32_t)0x05 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_MARK(mark, irq) ((uint32_t)(mark) | ((uint32_t)0x07 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_FLUSHE(irq) (((uint32_t)0x10 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_MSCAL(vuaddr, irq) ((uint32_t)(vuaddr) | ((uint32_t)0x14 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_MSCNT(irq) (((uint32_t)0x17 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_STMASK(irq) (((uint32_t)0x20 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_STROW(irq) (((uint32_t)0x30 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_STCOL(irq) (((uint32_t)0x31 << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_MPG(vuaddr, num, irq) ((uint32_t)(vuaddr) | ((uint32_t)(num) << 16) | ((uint32_t)(0x4a) << 24) | ((uint32_t)(irq) << 31))

#define VIF0_SET_UNPACK(vuaddr, num, cmd, irq) ((uint32_t)(vuaddr) | ((uint32_t)(num) << 16) | ((uint32_t)(0x60 | (cmd)) << 24) | ((uint32_t)(irq) << 31))



/*-vif1--------------------------------------------------*/


#define VIF1_SET_CODE(immediate, num, cmd, irq) ((uint32_t)(immediate) | ((uint32_t)(num) << 16) | ((uint32_t)(cmd) << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_NOP(irq) ((uint32_t)(irq) << 31)

#define VIF1_SET_STCYCL(wl, cl, irq) ((uint32_t)(cl) | ((uint32_t)(wl) << 8)  | ((uint32_t)0x01 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_OFFSET(offset, irq) ((uint32_t)(offset) | ((uint32_t)0x02 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_BASE(base, irq) ((uint32_t)(base) | ((uint32_t)0x03 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_ITOP(itop, irq) ((uint32_t)(itop) | ((uint32_t)0x04 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_STMOD(stmod, irq) ((uint32_t)(stmod) | ((uint32_t)0x05 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_MSKPATH3(msk, irq) ((uint32_t)(msk) | ((uint32_t)0x06 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_MARK(mark, irq) ((uint32_t)(mark) | ((uint32_t)0x07 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_FLUSHE(irq) (((uint32_t)0x10 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_FLUSH(irq) (((uint32_t)0x11 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_FLUSHA(irq) (((uint32_t)0x13 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_MSCAL(vuaddr, irq) ((uint32_t)(vuaddr) | ((uint32_t)0x14 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_MSCNT(irq) (((uint32_t)0x17 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_MSCALF(vuaddr, irq) ((uint32_t)(vuaddr) | ((uint32_t)0x15 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_STMASK(irq) (((uint32_t)0x20 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_STROW(irq) (((uint32_t)0x30 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_STCOL(irq) (((uint32_t)0x31 << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_MPG(vuaddr, num, irq) ((uint32_t)(vuaddr) | ((uint32_t)(num) << 16) | ((uint32_t)(0x4a) << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_DIRECT(count, irq) ((uint32_t)(count) | ((uint32_t)(0x50) << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_DIRECTHL(count, irq) ((uint32_t)(count) | ((uint32_t)(0x51) << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_UNPACK(vuaddr, num, cmd, irq) ((uint32_t)(vuaddr) | ((uint32_t)(num) << 16) | ((uint32_t)(0x60 | (cmd)) << 24) | ((uint32_t)(irq) << 31))

#define VIF1_SET_UNPACKR(vuaddr, num, cmd, irq) ((uint32_t)((vuaddr) | 0x8000) | ((uint32_t)(num) << 16) | ((uint32_t)(0x60 | (cmd)) << 24) | ((uint32_t)(irq) << 31))

/*-gif--------------------------------------------------*/

/* Utility for making giftag */
typedef struct {
    unsigned long NLOOP:15;
    unsigned long EOP:1;
    unsigned long pad16:16;
    unsigned long id:14;
    unsigned long PRE:1;
    unsigned long PRIM:11;
    unsigned long FLG:2;
    unsigned long NREG:4;
    unsigned long REGS0:4;
    unsigned long REGS1:4;
    unsigned long REGS2:4;
    unsigned long REGS3:4;
    unsigned long REGS4:4;
    unsigned long REGS5:4;
    unsigned long REGS6:4;
    unsigned long REGS7:4;
    unsigned long REGS8:4;
    unsigned long REGS9:4;
    unsigned long REGS10:4;
    unsigned long REGS11:4;
    unsigned long REGS12:4;
    unsigned long REGS13:4;
    unsigned long REGS14:4;
    unsigned long REGS15:4;
} sceGifTag __attribute__((aligned(16)));

#define GIF_SET_TAG(nloop, eop, pre, prim, flg, nreg) \
    ((uint64_t)(nloop) | ((uint64_t)(eop)<<15) | ((uint64_t)(pre) << 46) | \
    ((uint64_t)(prim)<<47) | ((uint64_t)(flg)<<58) | ((uint64_t)(nreg)<<60))

#define GIF_CLEAR_TAG(tp) \
    (*(uint64_t128 *)(tp) = (uint64_t128)0)

#define GIF_PACKED      0
#define GIF_REGLIST     1
#define GIF_IMAGE       2
#define GIF_PACKED_AD   0x0e

/** Gif Packed Mode register struct **/

typedef struct _sceGifPackRgbaq {
    uint32_t R;
    uint32_t G;
    uint32_t B;
    uint32_t A;
} sceGifPackRgbaq;

typedef struct _sceGifPackAd {
    uint64_t DATA;
    uint64_t ADDR;
} sceGifPackAd;

typedef struct _sceGifPackSt {
    float S;
    float T;
    float Q;
    uint32_t pad96;
} sceGifPackSt;

typedef struct _sceGifPackUv {
    int  U;
    int  V;
    long pad64;
} sceGifPackUv;

typedef struct _sceGifPackXyzf {
    int   X;
    int   Y;
    uint32_t Z;
    uint32_t F:12;
    uint32_t pad108:3;
    uint32_t ADC:1;
    uint32_t pad112:16;
} sceGifPackXyzf;

typedef struct _sceGifPackXyz {
    int   X;
    int   Y;
    uint32_t Z;
    uint32_t pad96:15;
    uint32_t ADC:1;
    uint32_t pad112:16;
} sceGifPackXyz;

typedef struct _sceGifPackFog {
    uint32_t pad[3];
    uint32_t F;
} sceGifPackFog;

typedef struct _sceGifPackNop {
    uint64_t pad[2];
} sceGifPackNop;

/* Bit patterns for GS general registers */

typedef struct {
    unsigned long A:2;
    unsigned long B:2;
    unsigned long C:2;
    unsigned long D:2;
    unsigned long pad8:24;
    unsigned long FIX:8;
    unsigned long pad40:24;
} sceGsAlpha;

typedef struct {
    unsigned long SBP:14; // Source base pointer in words/64
    unsigned long pad14:2; 
    unsigned long SBW:6; // Source buffer width in pixels/64
    unsigned long pad22:2;
    unsigned long SPSM:6; // Source format
    unsigned long pad30:2;
    unsigned long DBP:14; // Destination base pointer in words/64
    unsigned long pad46:2;
    unsigned long DBW:6; // Destination buffer width in pixels/64
    unsigned long pad54:2;
    unsigned long DPSM:6; // Destination format (same as source format)
    unsigned long pad62:2;
} sceGsBitbltbuf;

typedef struct {
    unsigned long WMS:2;
    unsigned long WMT:2;
    unsigned long MINU:10;
    unsigned long MAXU:10;
    unsigned long MINV:10;
    unsigned long MAXV:10;
    unsigned long pad44:20;
} sceGsClamp;

typedef struct {
    unsigned long CLAMP:1;
    unsigned long pad01:63;
} sceGsColclamp;

typedef struct {
    unsigned long DIMX00:3;
    unsigned long pad00:1;
    unsigned long DIMX01:3;
    unsigned long pad01:1;
    unsigned long DIMX02:3;
    unsigned long pad02:1;
    unsigned long DIMX03:3;
    unsigned long pad03:1;

    unsigned long DIMX10:3;
    unsigned long pad10:1;
    unsigned long DIMX11:3;
    unsigned long pad11:1;
    unsigned long DIMX12:3;
    unsigned long pad12:1;
    unsigned long DIMX13:3;
    unsigned long pad13:1;

    unsigned long DIMX20:3;
    unsigned long pad20:1;
    unsigned long DIMX21:3;
    unsigned long pad21:1;
    unsigned long DIMX22:3;
    unsigned long pad22:1;
    unsigned long DIMX23:3;
    unsigned long pad23:1;

    unsigned long DIMX30:3;
    unsigned long pad30:1;
    unsigned long DIMX31:3;
    unsigned long pad31:1;
    unsigned long DIMX32:3;
    unsigned long pad32:1;
    unsigned long DIMX33:3;
    unsigned long pad33:1;
} sceGsDimx;

typedef struct {
    unsigned long DTHE:1;
    unsigned long pad01:63;
} sceGsDthe;

typedef struct {
    unsigned long FBA:1;
    unsigned long pad01:63;
} sceGsFba;

typedef struct {
    unsigned long pad00;
} sceGsFinish;

typedef struct {
    unsigned long pad00:56;
    unsigned long F:8;
} sceGsFog;
  
typedef struct {
    unsigned long FCR:8;
    unsigned long FCG:8;
    unsigned long FCB:8;
    unsigned long pad24:40;
} sceGsFogcol;

typedef struct {
    unsigned long FBP:9;
    unsigned long pad09:7;
    unsigned long FBW:6;
    unsigned long pad22:2;
    unsigned long PSM:6;
    unsigned long pad30:2;
    unsigned long FBMSK:32;
} sceGsFrame;

typedef struct {
    unsigned long WDATA;
} sceGsHwreg;

typedef struct {
    uint32_t ID;
    uint32_t IDMSK;
} sceGsLabel;

typedef struct {
    unsigned long TBP1:14;
    unsigned long TBW1:6;
    unsigned long TBP2:14;
    unsigned long TBW2:6;
    unsigned long TBP3:14;
    unsigned long TBW3:6;
    unsigned long pad60:4;
} sceGsMiptbp1;

typedef struct {
    unsigned long TBP4:14;
    unsigned long TBW4:6;
    unsigned long TBP5:14;
    unsigned long TBW5:6;
    unsigned long TBP6:14;
    unsigned long TBW6:6;
    unsigned long pad60:4;
} sceGsMiptbp2;

typedef struct {
    unsigned long PABE:1;
    unsigned long pad01:63;
} sceGsPabe;

typedef struct {
    unsigned long PRIM:3;
    unsigned long IIP:1;
    unsigned long TME:1;
    unsigned long FGE:1;
    unsigned long ABE:1;
    unsigned long AA1:1;
    unsigned long FST:1;
    unsigned long CTXT:1;
    unsigned long FIX:1;
    unsigned long pad11:53;
} sceGsPrim;

typedef struct {
    unsigned long pad00:3;
    unsigned long IIP:1;
    unsigned long TME:1;
    unsigned long FGE:1;
    unsigned long ABE:1;
    unsigned long AA1:1;
    unsigned long FST:1;
    unsigned long CTXT:1;
    unsigned long FIX:1;
    unsigned long pad11:53;
} sceGsPrmode;

typedef struct {
    unsigned long AC:1;
    unsigned long pad01:63;
} sceGsPrmodecont;

typedef struct {
    uint32_t R:8;
    uint32_t G:8;
    uint32_t B:8;
    uint32_t A:8;
    float Q;
} sceGsRgbaq;

typedef struct {
    unsigned long MSK:2;
    unsigned long pad02:62;
} sceGsScanmsk;

typedef struct {
    unsigned long SCAX0:11;
    unsigned long pad11:5;
    unsigned long SCAX1:11;
    unsigned long pad27:5;
    unsigned long SCAY0:11;
    unsigned long pad43:5;
    unsigned long SCAY1:11;
    unsigned long pad59:5;
} sceGsScissor;

typedef struct {
    uint32_t ID;
    uint32_t IDMSK;
} sceGsSignal;

typedef struct {
    float S;
    float T;
} sceGsSt;

typedef struct {
    unsigned long ATE:1;
    unsigned long ATST:3;
    unsigned long AREF:8;
    unsigned long AFAIL:2;
    unsigned long DATE:1;
    unsigned long DATM:1;
    unsigned long ZTE:1;
    unsigned long ZTST:2;
    unsigned long pad19:45;
} sceGsTest;

typedef struct {
    unsigned long TBP0:14;
    unsigned long TBW:6;
    unsigned long PSM:6;
    unsigned long TW:4;
    unsigned long TH:4;
    unsigned long TCC:1;
    unsigned long TFX:2;
    unsigned long CBP:14;
    unsigned long CPSM:4;
    unsigned long CSM:1;
    unsigned long CSA:5;
    unsigned long CLD:3;
} sceGsTex0;

typedef struct {
    unsigned long LCM:1;
    unsigned long pad01:1;
    unsigned long MXL:3;
    unsigned long MMAG:1;
    unsigned long MMIN:3;
    unsigned long MTBA:1;
    unsigned long pad10:9;
    unsigned long L:2;
    unsigned long pad21:11;
    unsigned long K:12;
    unsigned long pad44:20;
} sceGsTex1;

typedef struct {
    unsigned long pad00:20;
    unsigned long PSM:6;
    unsigned long pad26:11;
    unsigned long CBP:14;
    unsigned long CPSM:4;
    unsigned long CSM:1;
    unsigned long CSA:5;
    unsigned long CLD:3;
} sceGsTex2;

typedef struct {
    unsigned long TA0:8;
    unsigned long pad08:7;
    unsigned long AEM:1;
    unsigned long pad16:16;
    unsigned long TA1:8;
    unsigned long pad40:24;
} sceGsTexa;

typedef struct {
    unsigned long CBW:6;
    unsigned long COU:6;
    unsigned long COV:10;
    unsigned long pad22:42;
} sceGsTexclut;

typedef struct {
    unsigned long pad00;
} sceGsTexflush;

typedef struct {
    unsigned long XDR:2;
    unsigned long pad02:62;
} sceGsTrxdir;

typedef struct {
    unsigned long SSAX:11;
    unsigned long pad11:5;
    unsigned long SSAY:11;
    unsigned long pad27:5;
    unsigned long DSAX:11;
    unsigned long pad43:5;
    unsigned long DSAY:11;
    unsigned long DIR:2;
    unsigned long pad61:3;
} sceGsTrxpos;

typedef struct {
    unsigned long RRW:12;
    unsigned long pad12:20;
    unsigned long RRH:12;
    unsigned long pad44:20;
} sceGsTrxreg;

typedef struct {
    unsigned long U:14;
    unsigned long pad14:2;
    unsigned long V:14;
    unsigned long pad30:34;
} sceGsUv;

typedef struct {
    unsigned long OFX:16;
    unsigned long pad16:16;
    unsigned long OFY:16;
    unsigned long pad48:16;
} sceGsXyoffset;

typedef struct {
    unsigned long X:16;
    unsigned long Y:16;
    unsigned long Z:32;
} sceGsXyz;

typedef struct {
    unsigned long X:16;
    unsigned long Y:16;
    unsigned long Z:24;
    unsigned long F:8;
} sceGsXyzf;

typedef struct {
    unsigned long ZBP:9;
    unsigned long pad09:15;
    unsigned long PSM:4;
    unsigned long pad28:4;
    unsigned long ZMSK:1;
    unsigned long pad33:31;
} sceGsZbuf;


/* bit access macro for GS general registers */

/* GS_PRIM
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                         |F|C|F|A|A|F|T|I|     |
 * |                                         |I|T|S|A|B|G|M|I|PRIM |
 * |                                         |X|X|T|1|E|E|E|P|     |
 * |                                         | |T| | | | | | |     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * bit definition
 */
#define GS_PRIM_PRIM_M          (0x07<< 0)
#define GS_PRIM_IIP_M           (0x01<< 3)
#define GS_PRIM_TME_M           (0x01<< 4)
#define GS_PRIM_FGE_M           (0x01<< 5)
#define GS_PRIM_ABE_M           (0x01<< 6)
#define GS_PRIM_AA1_M           (0x01<< 7)
#define GS_PRIM_FST_M           (0x01<< 8)
#define GS_PRIM_CTXT_M          (0x01<< 9)
#define GS_PRIM_FIX_M           (0x01<<10)

#define GS_PRIM_PRIM_O          ( 0)
#define GS_PRIM_IIP_O           ( 3)
#define GS_PRIM_TME_O           ( 4)
#define GS_PRIM_FGE_O           ( 5)
#define GS_PRIM_ABE_O           ( 6)
#define GS_PRIM_AA1_O           ( 7)
#define GS_PRIM_FST_O           ( 8)
#define GS_PRIM_CTXT_O          ( 9)
#define GS_PRIM_FIX_O           (10)

/* GS_XYOFFSET_1 / XYOFFSET_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                               |                                |
 * |                               |              OFY               |
 * |                               |                                |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                               |                                |
 * |                               |              OFX               |
 * |                               |                                |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * bit definition
 */
#define GS_XYOFFSET_OFX_M       (0xffff<< 0)
#define GS_XYOFFSET_OFY_M       (0xffffL<<32)

#define GS_XYOFFSET_OFX_O       ( 0)
#define GS_XYOFFSET_OFY_O       (32)


/* GS_TEX0_1 / TEX0_2
 *
 * 63            56              48               40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     |         |C|   C   |                           |   |T|   |
 * | CLD |   CSA   |S|   P   |             CBP           |TFX|C|TH |
 * |     |         |M|   S   |                           |   |C|   |
 * |     |         | |   M   |                           |   | |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *  31            24              16              8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   |       |           |           |                           |
 * |TH |  TW   |   PSM     |   TBW     |         TBP0              |
 * |   |       |           |           |                           |
 * |   |       |           |           |                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_TEX0_TBP0_M  (0x3fff<< 0)
#define GS_TEX0_TBW_M   (0x003f<<14)
#define GS_TEX0_PSM_M   (0x003f<<20)
#define GS_TEX0_TW_M    (0x000f<<26)
#define GS_TEX0_TH_M    (0x000fL<<30)
#define GS_TEX0_TCC_M   (0x0001L<<34)
#define GS_TEX0_TFX_M   (0x0003L<<35)
#define GS_TEX0_CBP_M   (0x3fffL<<37)
#define GS_TEX0_CPSM_M  (0x000fL<<51)
#define GS_TEX0_CSM_M   (0x0001L<<55)
#define GS_TEX0_CSA_M   (0x001fL<<56)
#define GS_TEX0_CLD_M   (0x0007L<<61)

#define GS_TEX0_TBP0_O  ( 0)
#define GS_TEX0_TBW_O   (14)
#define GS_TEX0_PSM_O   (20)
#define GS_TEX0_TW_O    (26)
#define GS_TEX0_TH_O    (30)
#define GS_TEX0_TCC_O   (34)
#define GS_TEX0_TFX_O   (35)
#define GS_TEX0_CBP_O   (37)
#define GS_TEX0_CPSM_O  (51)
#define GS_TEX0_CSM_O   (55)
#define GS_TEX0_CSA_O   (56)
#define GS_TEX0_CLD_O   (61)


/* GS_TEX1_1 / TEX1_2
 *
 * 63            56              48               40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                       |                       |
 * |                                       |            K          |
 * |                                       |                       |
 * |                                       |                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16              8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                     |   |                 |M|     |M|     | |L| 
 * |                     | L |                 |T|MMIN |M| MXL | |C|
 * |                     |   |                 |B|     |A|     | |M|
 * |                     |   |                 |A|     |G|     | | |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_TEX1_LCM_M   (0x01<< 0)
#define GS_TEX1_MXL_M   (0x07<< 2)
#define GS_TEX1_MMAG_M  (0x01<< 5)
#define GS_TEX1_MMIN_M  (0x07<< 6)
#define GS_TEX1_MTBA_M  (0x01<< 9)
#define GS_TEX1_L_M     (0x03<<19)
#define GS_TEX1_K_M     (0xfffL<<32)

#define GS_TEX1_LCM_O   ( 0)
#define GS_TEX1_MXL_O   ( 2)
#define GS_TEX1_MMAG_O  ( 5)
#define GS_TEX1_MMIN_O  ( 6)
#define GS_TEX1_MTBA_O  ( 9)
#define GS_TEX1_L_O     (19)
#define GS_TEX1_K_O     (32)


/* GS_TEX2_1 / TEX2_2
 *
 * 63             56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     |         |C|   C   |                           |         |  
 * | CLD |   CSA   |S|   P   |           CBP             |         |
 * |     |         |M|   S   |                           |         |
 * |     |         | |   M   |                           |         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16              8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |           |           |                                       | 
 * |           |   PSM     |                                       |
 * |           |           |                                       |
 * |           |           |                                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_TEX2_PSM_M   (0x3f  <<20)
#define GS_TEX2_CBP_M   (0x3fffL<<37)
#define GS_TEX2_CPSM_M  (0x0fL  <<51)
#define GS_TEX2_CSM_M   (0x01L  <<55)
#define GS_TEX2_CSA_M   (0x1fL  <<56)
#define GS_TEX2_CLD_M   (0x07L  <<61)

#define GS_TEX2_PSM_O   (20)
#define GS_TEX2_CBP_O   (37)
#define GS_TEX2_CPSM_O  (51)
#define GS_TEX2_CSM_O   (55)
#define GS_TEX2_CSA_O   (56)
#define GS_TEX2_CLD_O   (61)


/* GS_TEXCLUT
 *
 * 63             56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                   |                   |           |           |
 * |                   |         COV       |    COU    |    CBW    |
 * |                   |                   |           |           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_TEXCLUT_CBW_M    (0x3f<<0)
#define GS_TEXCLUT_COU_M    (0x3f<<6)
#define GS_TEXCLUT_COV_M    (0x3ff<<12)

#define GS_TEXCLUT_CBW_O    (0)
#define GS_TEXCLUT_COU_O    (6)
#define GS_TEXCLUT_COV_O    (12)


/* GS_MIPTBP1_1 / MIPTBP1_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |       |           |                           |           | T |
 * |       |           |                           |           | B |
 * |       |    TBW3   |           TBP3            |    TBW2   | P |
 * |       |           |                           |           | 2 |
 * |       |           |                           |           |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                       |           |                           |
 * |                       |           |                           |
 * |        TBP2           |    TBW1   |           TBP1            |
 * |                       |           |                           |
 * |                       |           |                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_MIPTBP1_TBP1_M   (0x3fff<< 0)
#define GS_MIPTBP1_TBW1_M   (0x3f  <<14)
#define GS_MIPTBP1_TBP2_M   (0x3fffL<<20)
#define GS_MIPTBP1_TBW2_M   (0x3fL  <<34)
#define GS_MIPTBP1_TBP3_M   (0x3fffL<<40)
#define GS_MIPTBP1_TBW3_M   (0x3fL  <<54)

#define GS_MIPTBP1_TBP1_O   ( 0)
#define GS_MIPTBP1_TBW1_O   (14)
#define GS_MIPTBP1_TBP2_O   (20)
#define GS_MIPTBP1_TBW2_O   (34)
#define GS_MIPTBP1_TBP3_O   (40)
#define GS_MIPTBP1_TBW3_O   (54)


/* GS_MIPTBP2_1 / MIPTBP2_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |       |           |                           |           | T |
 * |       |           |                           |           | B |
 * |       |    TBW6   |           TBP6            |    TBW5   | P |
 * |       |           |                           |           | 5 |
 * |       |           |                           |           |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                       |           |                           |
 * |                       |           |                           |
 * |        TBP5           |    TBW4   |           TBP4            |
 * |                       |           |                           |
 * |                       |           |                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_MIPTBP2_TBP4_M   (0x3fff<< 0)
#define GS_MIPTBP2_TBW4_M   (0x3f  <<14)
#define GS_MIPTBP2_TBP5_M   (0x3fffL<<20)
#define GS_MIPTBP2_TBW5_M   (0x3fL  <<34)
#define GS_MIPTBP2_TBP6_M   (0x3fffL<<40)
#define GS_MIPTBP2_TBW6_M   (0x3fL  <<54)

#define GS_MIPTBP2_TBP4_O   ( 0)
#define GS_MIPTBP2_TBW4_O   (14)
#define GS_MIPTBP2_TBP5_O   (20)
#define GS_MIPTBP2_TBW5_O   (34)
#define GS_MIPTBP2_TBP6_O   (40)
#define GS_MIPTBP2_TBW6_O   (54)


/* GS_TEXA
 *
 * 63             56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                               |               |
 * |                                               |      TA1      |
 * |                                               |               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                               |A|             |               |
 * |                               |E|             |       TA0     |
 * |                               |M|             |               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_TEXA_TA0_M   (0xff<< 0)
#define GS_TEXA_AEM_M   (0x01<<15)
#define GS_TEXA_TA1_M   (0xffL<<32)

#define GS_TEXA_TA0_O   ( 0)
#define GS_TEXA_AEM_O   (15)
#define GS_TEXA_TA1_O   (32)



/* GS_CLAMP_1 / CLAMP_2
 *
 * 63             56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                       |                   |MIN|
 * |                                       |        MAXV       | V |
 * |                                       |                   |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |               |                   |                   | W | W |
 * |     MINV      |        MAXU       |        MINU       | M | M |
 * |               |                   |                   | T | S |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_CLAMP_WMS_M      (0x03 << 0)
#define GS_CLAMP_WMT_M      (0x03 << 2)
#define GS_CLAMP_MINU_M     (0x3ff<< 4)
#define GS_CLAMP_MAXU_M     (0x3ff<<14)
#define GS_CLAMP_MINV_M     (0x3ffL<<24)
#define GS_CLAMP_MAXV_M     (0x3ffL<<34)

#define GS_CLAMP_WMS_O      ( 0)
#define GS_CLAMP_WMT_O      ( 2)
#define GS_CLAMP_MINU_O     ( 4)
#define GS_CLAMP_MAXU_O     (14)
#define GS_CLAMP_MINV_O     (24)
#define GS_CLAMP_MAXV_O     (34)
 

/* GS_FOG
 *
 * 63             56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |               |                                               |
 * |      F        |                                               |
 * |               |                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_FOG_F_M              (0xff << 56)

#define GS_FOG_F_O              ( 56 )


/* GS_FOGCOL
 *
 * 63             56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |               |               |               |               |
 * |               |      FCB      |       FCG     |      FCR      |
 * |               |               |               |               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_FOGCOL_FCR_M     (0xff<< 0)
#define GS_FOGCOL_FCG_M     (0xff<< 8)
#define GS_FOGCOL_FCB_M     (0xff<<16)

#define GS_FOGCOL_FCR_O     ( 0)
#define GS_FOGCOL_FCG_O     ( 8)
#define GS_FOGCOL_FCB_O     (16)
 

/* GS_SCISSOR_1 / SCISSOR_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |         |                     |         |                     |
 * |         |        SCAY1        |         |         SCAY0       |
 * |         |                     |         |                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |         |                     |         |                     |
 * |         |        SCAX1        |         |        SCAX0        |
 * |         |                     |         |                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_SCISSOR_SCAX0_M      (0x7ff<< 0)
#define GS_SCISSOR_SCAX1_M      (0x7ff<<16)
#define GS_SCISSOR_SCAY0_M      (0x7ffL<<32)
#define GS_SCISSOR_SCAY1_M      (0x7ffL<<48)

#define GS_SCISSOR_SCAX0_O      ( 0)
#define GS_SCISSOR_SCAX1_O      (16)
#define GS_SCISSOR_SCAY0_O  (32)
#define GS_SCISSOR_SCAY1_O  (48) 


/* GS_TEST_1 / TEST_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                         | Z |Z|D|D| A |               |  A  |A|
 * |                         | T |T|A|A| F |               |  T  |T|
 * |                         | S |E|T|T| A |     AREF      |  S  |E|
 * |                         | T | |M|E| I |               |  T  | |
 * |                         |   | | | | L |               |     | |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_TEST_ATE_M       (0x01<< 0)
#define GS_TEST_ATST_M      (0x07<< 1)
#define GS_TEST_AREF_M      (0xff<< 4)
#define GS_TEST_AFAIL_M     (0x03<<12)
#define GS_TEST_DATE_M      (0x01<<14)
#define GS_TEST_DATM_M      (0x01<<15)
#define GS_TEST_ZTE_M       (0x01<<16)
#define GS_TEST_ZTST_M      (0x03<<17)



#define GS_TEST_ATE_O       ( 0)
#define GS_TEST_ATST_O      ( 1)
#define GS_TEST_AREF_O      ( 4)
#define GS_TEST_AFAIL_O     (12)
#define GS_TEST_DATE_O      (14)
#define GS_TEST_DATM_O      (15)
#define GS_TEST_ZTE_O       (16)
#define GS_TEST_ZTST_O      (17)


/* GS_ALPHA_1 / ALPHA_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                               |               |
 * |                                               |      FIX      |
 * |                                               |               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                               |   |   |   |   |
 * |                                               | D | C | B | A |
 * |                                               |   |   |   |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition 
 */
#define GS_ALPHA_A_M        (0x03<< 0)
#define GS_ALPHA_B_M        (0x03<< 2)
#define GS_ALPHA_C_M        (0x03<< 4)
#define GS_ALPHA_D_M        (0x03<< 6)
#define GS_ALPHA_FIX_M      (0xffL<<32)

#define GS_ALPHA_A_O        ( 0)
#define GS_ALPHA_B_O        ( 2)
#define GS_ALPHA_C_O        ( 4)
#define GS_ALPHA_D_O        ( 6)
#define GS_ALPHA_FIX_O      (32)


/* GS_FRAME_1 / FRAME_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * |                             FBMSK                             |
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   |           |   |           |               |               |
 * |   |    PSM    |   |    FBW    |               |      FBP      |
 * |   |           |   |           |               |               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition
 */
#define GS_FRAME_FBP_M          (0x1ff     << 0)
#define GS_FRAME_FBW_M          (0x3f      <<16)
#define GS_FRAME_PSM_M          (0x3f      <<24)
#define GS_FRAME_FBMSK_M        (0xffffffffL<<32)

#define GS_FRAME_FBP_O          ( 0)
#define GS_FRAME_FBW_O          (16)
#define GS_FRAME_PSM_O          (24)
#define GS_FRAME_FBMSK_O        (32)


/* GS_ZBUF_1 / ZBUF_2
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                             |Z|
 * |                                                             |M|
 * |                                                             |S|
 * |                                                             |K|
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |       |       |                             |                 |
 * |       |  PSM  |                             |       ZBP       |
 * |       |       |                             |                 |
 * |       |       |                             |                 |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition
 */
#define GS_ZBUF_ZBP_M           (0x1ff<< 0)
#define GS_ZBUF_PSM_M           (0x0f <<24)
#define GS_ZBUF_ZMSK_M          (0x01L <<32)

#define GS_ZBUF_ZBP_O           ( 0)
#define GS_ZBUF_PSM_O           (24)
#define GS_ZBUF_ZMSK_O          (32)


/* GS_BITBLTBUF
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   |           |   |           |   |                           |
 * |   |   DPSM    |   |    DBW    |   |             DBP           |
 * |   |           |   |           |   |                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   |           |   |           |   |                           |
 * |   |    SPSM   |   |    SBW    |   |          SBP              |
 * |   |           |   |           |   |                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition
 */
#define GS_BITBLTBUF_SBP_M              (0x3fff<< 0)
#define GS_BITBLTBUF_SBW_M              (0x3f  <<16)
#define GS_BITBLTBUF_SPSM_M             (0x3f  <<24)
#define GS_BITBLTBUF_DBP_M              (0x3fffL<<32)
#define GS_BITBLTBUF_DBW_M              (0x3fL  <<48)
#define GS_BITBLTBUF_DPSM_M             (0x3fL  <<56)

#define GS_BITBLTBUF_SBP_O              ( 0)
#define GS_BITBLTBUF_SBW_O              (16)
#define GS_BITBLTBUF_SPSM_O             (24)
#define GS_BITBLTBUF_DBP_O              (32)
#define GS_BITBLTBUF_DBW_O              (48)
#define GS_BITBLTBUF_DPSM_O             (56)



/* GS_TRXPOS
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     |   |                     |         |                     |
 * |     |DIR|        DSAY         |         |         DSAX        |
 * |     |   |                     |         |                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |         |                     |         |                     |
 * |         |         SSAY        |         |         SSAX        |
 * |         |                     |         |                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition
 */
#define GS_TRXPOS_SSAX_M            (0x7ff<< 0)
#define GS_TRXPOS_SSAY_M            (0x7ff<<16)
#define GS_TRXPOS_DSAX_M            (0x7ffL<<32)
#define GS_TRXPOS_DSAY_M            (0x7ffL<<48)
#define GS_TRXPOS_DIR_M             (0x03L <<59)

#define GS_TRXPOS_SSAX_O            ( 0)
#define GS_TRXPOS_SSAY_O            (16)
#define GS_TRXPOS_DSAX_O            (32)
#define GS_TRXPOS_DSAY_O            (48)
#define GS_TRXPOS_DIR_O             (59)


/* GS_TRXREG
 *
 *  63            56              48              40              32
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                       |                       |
 * |                                       |          RRH          |
 * |                                       |                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  
 *  31            24              16               8               0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                       |                       |
 * |                                       |           RRW         |
 * |                                       |                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * definition
 */
#define GS_TRXREG_RRW_M             (0xfff<< 0)
#define GS_TRXREG_RRH_M             (0xfffL<<32)

#define GS_TRXREG_RRW_O             ( 0)
#define GS_TRXREG_RRH_O             (32)


/* Macros for making bit pattern of GS registers */

#define GS_SET_ALPHA_1  GS_SET_ALPHA
#define GS_SET_ALPHA_2  GS_SET_ALPHA
#define GS_SET_ALPHA(a, b, c, d, fix) \
    ((uint64_t)(a)       | ((uint64_t)(b) << 2)     | ((uint64_t)(c) << 4) | \
    ((uint64_t)(d) << 6) | ((uint64_t)(fix) << 32))

#define GS_SET_BITBLTBUF(sbp, sbw, spsm, dbp, dbw, dpsm) \
    ((uint64_t)(sbp)         | ((uint64_t)(sbw) << 16) | \
    ((uint64_t)(spsm) << 24) | ((uint64_t)(dbp) << 32) | \
    ((uint64_t)(dbw) << 48)  | ((uint64_t)(dpsm) << 56))

#define GS_SET_CLAMP_1  GS_SET_CLAMP
#define GS_SET_CLAMP_2  GS_SET_CLAMP
#define GS_SET_CLAMP(wms, wmt, minu, maxu, minv, maxv) \
    ((uint64_t)(wms)         | ((uint64_t)(wmt) << 2) | \
    ((uint64_t)(minu) << 4)  | ((uint64_t)(maxu) << 14) | \
    ((uint64_t)(minv) << 24) | ((uint64_t)(maxv) << 34))

#define GS_SET_COLCLAMP(clamp) ((uint64_t)(clamp))

#define GS_SET_DIMX(dm00, dm01, dm02, dm03, dm10, dm11, dm12, dm13, \
        dm20, dm21, dm22, dm23, dm30, dm31, dm32, dm33) \
    ((uint64_t)(dm00)        | ((uint64_t)(dm01) << 4)  | \
    ((uint64_t)(dm02) << 8)  | ((uint64_t)(dm03) << 12) | \
    ((uint64_t)(dm10) << 16) | ((uint64_t)(dm11) << 20) | \
    ((uint64_t)(dm12) << 24) | ((uint64_t)(dm13) << 28) | \
    ((uint64_t)(dm20) << 32) | ((uint64_t)(dm21) << 36) | \
    ((uint64_t)(dm22) << 40) | ((uint64_t)(dm23) << 44) | \
    ((uint64_t)(dm30) << 48) | ((uint64_t)(dm31) << 52) | \
    ((uint64_t)(dm32) << 56) | ((uint64_t)(dm33) << 60))

#define GS_SET_DTHE(dthe) ((uint64_t)(dthe))

#define GS_SET_FBA_1    GS_SET_FBA
#define GS_SET_FBA_2    GS_SET_FBA
#define GS_SET_FBA(fba) ((uint64_t)(fba))

#define GS_SET_FOG(f) ((uint64_t)(f) << 56)

#define GS_SET_FOGCOL(fcr, fcg, fcb) \
    ((uint64_t)(fcr) | ((uint64_t)(fcg) << 8) | ((uint64_t)(fcb) << 16))

#define GS_SET_FRAME_1  GS_SET_FRAME
#define GS_SET_FRAME_2  GS_SET_FRAME
#define GS_SET_FRAME(fbp, fbw, psm, fbmask) \
    ((uint64_t)(fbp)        | ((uint64_t)(fbw) << 16) | \
    ((uint64_t)(psm) << 24) | ((uint64_t)(fbmask) << 32))

#define GS_SET_LABEL(id, idmsk) \
    ((uint64_t)(id) | ((uint64_t)(idmsk) << 32))

#define GS_SET_MIPTBP1_1    GS_SET_MIPTBP1
#define GS_SET_MIPTBP1_2    GS_SET_MIPTBP1
#define GS_SET_MIPTBP1(tbp1, tbw1, tbp2, tbw2, tbp3, tbw3) \
    ((uint64_t)(tbp1)        | ((uint64_t)(tbw1) << 14) | \
    ((uint64_t)(tbp2) << 20) | ((uint64_t)(tbw2) << 34) | \
    ((uint64_t)(tbp3) << 40) | ((uint64_t)(tbw3) << 54))

#define GS_SET_MIPTBP2_1    GS_SET_MIPTBP2
#define GS_SET_MIPTBP2_2    GS_SET_MIPTBP2
#define GS_SET_MIPTBP2(tbp4, tbw4, tbp5, tbw5, tbp6, tbw6) \
    ((uint64_t)(tbp4)        | ((uint64_t)(tbw4) << 14) | \
    ((uint64_t)(tbp5) << 20) | ((uint64_t)(tbw5) << 34) | \
    ((uint64_t)(tbp6) << 40) | ((uint64_t)(tbw6) << 54))

#define GS_SET_PABE(pabe) ((uint64_t)(pabe))

#define GS_SET_PRIM(prim, iip, tme, fge, abe, aa1, fst, ctxt, fix) \
    ((uint64_t)(prim)      | ((uint64_t)(iip) << 3)  | ((uint64_t)(tme) << 4) | \
    ((uint64_t)(fge) << 5) | ((uint64_t)(abe) << 6)  | ((uint64_t)(aa1) << 7) | \
    ((uint64_t)(fst) << 8) | ((uint64_t)(ctxt) << 9) | ((uint64_t)(fix) << 10))

#define GS_SET_PRMODE(iip, tme, fge, abe, aa1, fst, ctxt, fix) \
    (((uint64_t)(iip) << 3) | ((uint64_t)(tme) << 4)  | \
    ((uint64_t)(fge) << 5) | ((uint64_t)(abe) << 6)  | ((uint64_t)(aa1) << 7) | \
    ((uint64_t)(fst) << 8) | ((uint64_t)(ctxt) << 9) | ((uint64_t)(fix) << 10))

#define GS_SET_PRMODECONT(ac) ((uint64_t)(ac))

#define GS_SET_RGBAQ(r, g, b, a, q) \
    ((uint64_t)(r)        | ((uint64_t)(g) << 8) | ((uint64_t)(b) << 16) | \
    ((uint64_t)(a) << 24) | ((uint64_t)(q) << 32))

#define GS_SET_SCANMSK(msk) ((uint64_t)(msk))

#define GS_SET_SCISSOR_1    GS_SET_SCISSOR
#define GS_SET_SCISSOR_2    GS_SET_SCISSOR
#define GS_SET_SCISSOR(scax0, scax1, scay0, scay1) \
    ((uint64_t)(scax0)        | ((uint64_t)(scax1) << 16) | \
    ((uint64_t)(scay0) << 32) | ((uint64_t)(scay1) << 48))

#define GS_SET_SIGNAL(id, idmsk) \
    ((uint64_t)(id) | ((uint64_t)(idmsk) << 32))

#define GS_SET_ST(s, t) ((uint64_t)(s) |  ((uint64_t)(t) << 32))

/* datm bit is unavailable on GS Rev.1 */
#define GS_SET_TEST_1 GS_SET_TEST
#define GS_SET_TEST_2 GS_SET_TEST
#define GS_SET_TEST(ate, atst, aref, afail, date, datm, zte, ztst) \
    ((uint64_t)(ate)         | ((uint64_t)(atst) << 1) | \
    ((uint64_t)(aref) << 4)  | ((uint64_t)(afail) << 12) | \
    ((uint64_t)(date) << 14) | ((uint64_t)(datm) << 15) | \
    ((uint64_t)(zte) << 16)  | ((uint64_t)(ztst) << 17))

#define GS_SET_TEX0_1   GS_SET_TEX0
#define GS_SET_TEX0_2   GS_SET_TEX0
#define GS_SET_TEX0(tbp, tbw, psm, tw, th, tcc, tfx, \
            cbp, cpsm, csm, csa, cld) \
    ((uint64_t)(tbp)         | ((uint64_t)(tbw) << 14) | \
    ((uint64_t)(psm) << 20)  | ((uint64_t)(tw) << 26) | \
    ((uint64_t)(th) << 30)   | ((uint64_t)(tcc) << 34) | \
    ((uint64_t)(tfx) << 35)  | ((uint64_t)(cbp) << 37) | \
    ((uint64_t)(cpsm) << 51) | ((uint64_t)(csm) << 55) | \
    ((uint64_t)(csa) << 56)  | ((uint64_t)(cld) << 61))

#define GS_SET_TEX1_1   GS_SET_TEX1
#define GS_SET_TEX1_2   GS_SET_TEX1
#define GS_SET_TEX1(lcm, mxl, mmag, mmin, mtba, l, k) \
    ((uint64_t)(lcm)        | ((uint64_t)(mxl) << 2)  | \
    ((uint64_t)(mmag) << 5) | ((uint64_t)(mmin) << 6) | \
    ((uint64_t)(mtba) << 9) | ((uint64_t)(l) << 19) | \
    ((uint64_t)(k) << 32))

#define GS_SET_TEX2_1   GS_SET_TEX2
#define GS_SET_TEX2_2   GS_SET_TEX2
#define GS_SET_TEX2(psm, cbp, cpsm, csm, csa, cld) \
    (((uint64_t)(psm) << 20) | ((uint64_t)(cbp) << 37) | \
    ((uint64_t)(cpsm) << 51) | ((uint64_t)(csm) << 55) | \
    ((uint64_t)(csa) << 56)  | ((uint64_t)(cld) << 61))

#define GS_SET_TEXA(ta0, aem, ta1) \
    ((uint64_t)(ta0) | ((uint64_t)(aem) << 15) | ((uint64_t)(ta1) << 32))

#define GS_SET_TEXCLUT(cbw, cou, cov) \
    ((uint64_t)(cbw) | ((uint64_t)(cou) << 6) | ((uint64_t)(cov) << 12))

#define GS_SET_TRXDIR(xdr) ((uint64_t)(xdr))

#define GS_SET_TRXPOS(ssax, ssay, dsax, dsay, dir) \
    ((uint64_t)(ssax)        | ((uint64_t)(ssay) << 16) | \
    ((uint64_t)(dsax) << 32) | ((uint64_t)(dsay) << 48) | \
    ((uint64_t)(dir) << 59))

#define GS_SET_TRXREG(rrw, rrh) \
    ((uint64_t)(rrw) | ((uint64_t)(rrh) << 32))

#define GS_SET_UV(u, v) ((uint64_t)(u) | ((uint64_t)(v) << 16))

#define GS_SET_XYOFFSET_1   GS_SET_XYOFFSET
#define GS_SET_XYOFFSET_2   GS_SET_XYOFFSET
#define GS_SET_XYOFFSET(ofx, ofy) ((uint64_t)(ofx) | ((uint64_t)(ofy) << 32))

#define GS_SET_XYZ3 GS_SET_XYZ
#define GS_SET_XYZ2 GS_SET_XYZ
#define GS_SET_XYZ(x, y, z) \
    ((uint64_t)(x) | ((uint64_t)(y) << 16) | ((uint64_t)(z) << 32))

#define GS_SET_XYZF3 GS_SET_XYZF
#define GS_SET_XYZF2 GS_SET_XYZF
#define GS_SET_XYZF(x, y, z, f) \
    ((uint64_t)(x) | ((uint64_t)(y) << 16) | ((uint64_t)(z) << 32) | \
    ((uint64_t)(f) << 56))

#define GS_SET_ZBUF_1   GS_SET_ZBUF
#define GS_SET_ZBUF_2   GS_SET_ZBUF
#define GS_SET_ZBUF(zbp, psm, zmsk) \
    ((uint64_t)(zbp) | ((uint64_t)(psm) << 24) | \
    ((uint64_t)(zmsk) << 32))

#define GS_SET_PMODE(en1, en2, crtmd, mmod, amod, \
        slbg, alp) \
    ((uint64_t)(en1)            | ((uint64_t)(en2) << 1)      | \
    ((uint64_t)(crtmd) << 2)    | ((uint64_t)(mmod) << 5)     | \
    ((uint64_t)(amod) << 6)     | ((uint64_t)(slbg) << 7)     | \
    ((uint64_t)(alp) << 8))

#define GS_SET_DISPLAY1 GS_SET_DISPLAY
#define GS_SET_DISPLAY2 GS_SET_DISPLAY
#define GS_SET_DISPLAY  GS_SET_DISPLAY_INTERLACE

/* #define GS_PAL_MODE */

#ifdef GS_PAL_MODE

/* PAL, NOINTERLACE */
#define GS_SET_DISPLAY_NOINTERLACE(dx, dy, magh, magv, dw, dh)\
    ((uint64_t)(0x290 + (dx)) | \
    ((uint64_t)(36 + (dy)) << 12) | \
    ((uint64_t)(magh) << 23)  | ((uint64_t)(magv) << 27) | \
    ((uint64_t)(dw) << 32)    | ((uint64_t)(dh) << 44))

/* PAL, INTERLACE */
#define GS_SET_DISPLAY_INTERLACE(dx, dy, magh, magv, dw, dh)\
    ((uint64_t)(0x290 + (dx)) | \
    ((uint64_t)(72 + (dy)) << 12) | \
    ((uint64_t)(magh) << 23)  | ((uint64_t)(magv) << 27) | \
    ((uint64_t)(dw) << 32)    | ((uint64_t)(dh) << 44))

#else

/* NTSC, NOINTERLACE */
#define GS_SET_DISPLAY_NOINTERLACE(dx, dy, magh, magv, dw, dh)\
    ((uint64_t)(0x27c + (dx)) | \
    ((uint64_t)(25 + (dy)) << 12) | \
    ((uint64_t)(magh) << 23)  | ((uint64_t)(magv) << 27) | \
    ((uint64_t)(dw) << 32)    | ((uint64_t)(dh) << 44))

/* NTSC, INTERLACE */
#define GS_SET_DISPLAY_INTERLACE(dx, dy, magh, magv, dw, dh)\
    ((uint64_t)(0x27c + (dx)) | \
    ((uint64_t)(50 + (dy)) << 12) | \
    ((uint64_t)(magh) << 23)  | ((uint64_t)(magv) << 27) | \
    ((uint64_t)(dw) << 32)    | ((uint64_t)(dh) << 44))

#endif  /* GS_PAL_MODE */

#define GS_SET_DISPFB1  GS_SET_DISPFB
#define GS_SET_DISPFB2  GS_SET_DISPFB
#define GS_SET_DISPFB(fbp, fbw, psm, dbx, dby) \
    ((uint64_t)(fbp)        | ((uint64_t)(fbw) << 9)  | \
    ((uint64_t)(psm) << 15) | ((uint64_t)(dbx) << 32) | \
    ((uint64_t)(dby) << 43))

#define GS_SET_SMODE2(intr, ffmd, dpms) \
    ((uint64_t)(intr) | ((uint64_t)(ffmd) << 1) | ((uint64_t)(dpms) << 2))

#define GS_SET_EXTBUF(exbp, exbw, fbin, wffmd, emoda, emodc, wdx, wdy) \
    ((uint64_t)(exbp)         | ((uint64_t)(exbw) << 14)  | \
    ((uint64_t)(fbin) << 20)  | ((uint64_t)(wffmd) << 22) | \
    ((uint64_t)(emoda) << 23) | ((uint64_t)(emodc) << 25) | \
    ((uint64_t)(wdx) << 32)   | ((uint64_t)(wdy) << 43))

#define GS_SET_EXTDATA(sx, sy, smph, smpv, ww, wh) \
    ((uint64_t)(sx)          | ((uint64_t)(sy) << 12)   | \
    ((uint64_t)(smph) << 23) | ((uint64_t)(smpv) << 27) | \
    ((uint64_t)(ww) << 32)   | ((uint64_t)(wh) << 44))

#define GS_SET_EXTWRITE(write) ((uint64_t)(write))

#define GS_SET_BGCOLOR(r, g, b) \
    ((uint64_t)(r) | ((uint64_t)(g) << 8) | ((uint64_t)(b) << 16))

/* GS registers address */
    /*-- vertex info. reg--*/
#define GS_PRIM         0x00
#define GS_RGBAQ        0x01
#define GS_ST           0x02
#define GS_UV           0x03
#define GS_XYZF2        0x04
#define GS_XYZ2         0x05
#define GS_FOG          0x0a
#define GS_XYZF3        0x0c
#define GS_XYZ3         0x0d
#define GS_XYOFFSET_1   0x18
#define GS_XYOFFSET_2   0x19
#define GS_PRMODECONT   0x1a

    /*-- drawing attribute reg. --*/
#define GS_PRMODE       0x1b
#define GS_TEX0_1       0x06
#define GS_TEX0_2       0x07
#define GS_TEX1_1       0x14
#define GS_TEX1_2       0x15
#define GS_TEX2_1       0x16
#define GS_TEX2_2       0x17
#define GS_TEXCLUT      0x1c
#define GS_SCANMSK      0x22
#define GS_MIPTBP1_1    0x34
#define GS_MIPTBP1_2    0x35
#define GS_MIPTBP2_1    0x36
#define GS_MIPTBP2_2    0x37
#define GS_CLAMP_1      0x08
#define GS_CLAMP_2      0x09
#define GS_TEXA         0x3b
#define GS_FOGCOL       0x3d
#define GS_TEXFLUSH     0x3f

    /*-- pixel operation reg. --*/
#define GS_SCISSOR_1    0x40
#define GS_SCISSOR_2    0x41
#define GS_ALPHA_1      0x42
#define GS_ALPHA_2      0x43
#define GS_DIMX         0x44
#define GS_DTHE         0x45
#define GS_COLCLAMP     0x46
#define GS_TEST_1       0x47
#define GS_TEST_2       0x48
#define GS_PABE         0x49
#define GS_FBA_1        0x4a
#define GS_FBA_2        0x4b

    /*-- buffer reg. --*/
#define GS_FRAME_1      0x4c
#define GS_FRAME_2      0x4d
#define GS_ZBUF_1       0x4e
#define GS_ZBUF_2       0x4f

    /*-- inter-buffer transfer reg. --*/
#define GS_BITBLTBUF    0x50
#define GS_TRXPOS       0x51
#define GS_TRXREG       0x52
#define GS_TRXDIR       0x53
#define GS_HWREG        0x54

    /*-- other reg. --*/
#define GS_SIGNAL       0x60
#define GS_FINISH       0x61
#define GS_LABEL        0x62
#define GS_NOP          0x7f

// Pixel-Storage Methods
/** 32 bits per pixel. */
#define GS_PSM_32   0x00
/** 24 bits per pixel. */
#define GS_PSM_24   0x01
/** 16 bits per pixel. */
#define GS_PSM_16   0x02
/** 16 bits per pixel. */
#define GS_PSM_16S  0x0A
/** 24 bits per pixel. */
#define GS_PSM_PS24 0x12
/** 8 bits per pixel, palettized. */
#define GS_PSM_8    0x13
/** 4 bits per pixel, palettized. */
#define GS_PSM_4    0x14
/** 8 bits per pixel, 24 to 32 */
#define GS_PSM_8H   0x1B
/** 4 bits per pixel, 28 to 32 */
#define GS_PSM_4HL  0x24
/** 4 bits per pixel, 24 to 27 */
#define GS_PSM_4HH  0x2C
/** 32 bits per pixel. */
#define GS_PSMZ_32  0x30
/** 24 bits per pixel. */
#define GS_PSMZ_24  0x31
/** 16 bits per pixel. */
#define GS_PSMZ_16  0x32
/** 16 bits per pixel. */
#define GS_PSMZ_16S 0x3A

#define GS_SET_TEXFLUSH(A) (uint64_t)((A)&0xFFFFFFFF)

#ifdef __cplusplus
}
#endif

#endif

