#ifndef _ITO_DMADEFS_H
#define _ITO_DMADEFS_H

#define DMA_MAX_SIZE				(0xFFFF*16)

// DMA Channel 2 - GIF
#define D2_CHCR						*((volatile u32*)0x1000A000)
#define D2_MADR						*((volatile u32*)0x1000A010)
#define D2_QWC						*((volatile u32*)0x1000A020)
#define D2_TADR						*((volatile u32*)0x1000A030)
#define D2_ASR0						*((volatile u32*)0x1000A040)
#define D2_ASR1						*((volatile u32*)0x1000A050)

// CHCR
#define	DMA_DIR_TO_MEM				0
#define DMA_DIR_FROM_MEM			1

#define DMA_MODE_NORMAL				0
#define DMA_MODE_CHAIN				(1 << 2)
#define DMA_MODE_INTERLEAVE			(2 << 2)

#define DMA_ASP_NO_PUSH				0
#define DMA_ASP_1_PUSH				(1 << 4)
#define DMA_ASP_2_PUSH				(2 << 4)

#define DMA_NO_TAG_TRANSFER			0
#define DMA_TAG_TRANSFER			(1 << 6)

#define DMA_TAG_INT_DISABLE			0
#define DMA_TAG_INT_ENABLE			(1 << 7)

#define DMA_STOP					0
#define DMA_START					(1 << 8)

// MADR / TADR / ASR0 / ASR1

#define DMA_MEMORY_ADDR				0
#define DMA_SPR_ADDR				(1 << 31)

// TAG

#define DMA_TAG(qwc, pce, id, irq, addr, spr) \
				(((u64)(spr) << 63) | ((u64)addr) << 32 | ((u64)(irq) << 31) | ((u64)(id) << 28) | ((u64)(pce) << 26) | (qwc))

#define	DMA_TAG_ID_REFE				0
#define	DMA_TAG_ID_CNT				1
#define	DMA_TAG_ID_NEXT				2
#define	DMA_TAG_ID_REF				3
#define	DMA_TAG_ID_REFS				4
#define	DMA_TAG_ID_CALL				5
#define	DMA_TAG_ID_RET				6
#define	DMA_TAG_ID_END				7

#endif

