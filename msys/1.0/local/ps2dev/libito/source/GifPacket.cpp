#include <Ito.h>
#include <GifPacket.h>
#include <kernel.h>
#include <stdio.h>
#include <malloc.h>

using namespace Ito;

#ifdef F_AllocateDmaTag
void GifPacket::AllocateDmaTag()
{
	Tag tag;

	tag.addr = 0;
	tag.size = DMA_MAX_SIZE;
	tag.addr = (u64*)memalign(16, tag.size);
	
	u32 i= 5;

	while((tag.addr == 0) && i)
	{
		tag.size /= 2;
		tag.addr = (u64*)memalign(16, tag.size);
		i--;
	}
	
	m_CurrentDmaTagEnd = tag.addr+(tag.size/8);

	m_Size += m_TagList->Last().size;

	m_TagList->Add(tag);
}
#endif

#ifdef F_Check
void GifPacket::Check()
{
	if(((u8*)m_CurrentDmaTagEnd)-((u8*)m_CurrentAddr) <= 32)
	{
		AllocateDmaTag();
		
		m_CurrentAddr[0] = DMA_TAG(0, 0, DMA_TAG_ID_NEXT, 0, ((u32)m_TagList->Last().addr) & 0x7FFFFFFF, 0);
		
		m_CurrentAddr =  m_TagList->Last().addr;
		m_LastDmaTag = m_CurrentAddr;
		
		m_LastDmaTag[0] = DMA_TAG(0, 0, DMA_TAG_ID_CNT, 0, 0, 0);
		m_LastDmaTag[1] = 0;

		m_CurrentAddr+=2;		
	}
}
#endif

#ifdef F_UpdateLastDmaTag
void GifPacket::UpdateLastDmaTag()
{
	u32 qwc = (((u8*)m_CurrentAddr - (u8*)m_LastDmaTag)/16)-1;
	
//	printf("qwc %i\n", qwc);
	
	m_LastDmaTag[0] |=  DMA_TAG(qwc, 0, 0, 0, 0, 0);
}
#endif

#ifdef F_DmaTagCnt
void GifPacket::DmaTagCnt()
{
	UpdateLastDmaTag();

	m_LastDmaTag = m_CurrentAddr;
	
	m_LastDmaTag[0] = DMA_TAG(0, 0, DMA_TAG_ID_CNT, 0, 0, 0);
	m_LastDmaTag[1] = 0;

	m_CurrentAddr+=2;
}
#endif

#ifdef F_DmaTagNext
void GifPacket::DmaTagNext(u64* nextTag)
{
	UpdateLastDmaTag();

	m_LastDmaTag = m_CurrentAddr;
	
	m_LastDmaTag[0] = DMA_TAG(0, 0, DMA_TAG_ID_NEXT, 0, ((u32)nextTag) & 0x7FFFFFFF, 0);
	m_LastDmaTag[1] = 0;

	m_CurrentAddr+=2;
	
}
#endif

#ifdef F_DmaTagRef
void GifPacket::DmaTagRef(u8* address, u32 size)
{
	UpdateLastDmaTag();
	
	m_LastDmaTag = m_CurrentAddr;
	
	m_LastDmaTag[0] = DMA_TAG(size/16, 0, DMA_TAG_ID_REF, 0, ((u32)((u8*)address)) & 0x7FFFFFFF, 0);
	m_LastDmaTag[1] = 0;

	m_CurrentAddr+=2;
}
#endif

#ifdef F_DmaTagCall
void GifPacket::DmaTagCall(u64* nextTag)
{
	UpdateLastDmaTag();

}
#endif

#ifdef F_DmaTagRet
void GifPacket::DmaTagRet();
{
	UpdateLastDmaTag();

}
#endif

#ifdef F_DmaTagEnd
void GifPacket::DmaTagEnd()
{
	UpdateLastDmaTag();

	m_CurrentAddr[0] = DMA_TAG(0, 0, DMA_TAG_ID_END, 0, 0, 0);
	m_CurrentAddr[1] = 0;

	m_CurrentAddr+=2;
}
#endif

#ifdef F_Constructor
GifPacket::GifPacket()
{
	m_Context = 0;
	m_TextureList = 0;
	m_TagList = 0;
	m_Size = 0;

	Reset();
}
#endif

#ifdef F_Destructor
GifPacket::~GifPacket()
{
	//	free(m_PacketStart);
}
#endif

#ifdef F_Giftag
void GifPacket::Giftag(s32 nloop, s32 eop, s32 pre, s32 prim, s32 flg, s32 nreg, u64 regs)
{
	m_CurrentAddr[0] = GIFTAG(nloop, eop, pre, prim, flg, nreg);
	m_CurrentAddr[1] = regs;
	m_CurrentAddr+=2;

	Check();
}
#endif

#ifdef F_SetContext
void GifPacket::SetContext(s32 context)
{
	m_Context = context-1;
}
#endif

#ifdef F_GetContext
u32 GifPacket::GetContext()
{
	return (m_Context+1);
}
#endif


#ifdef F_AddDWord
void GifPacket::AddDWord(u64 dword)
{
	m_CurrentAddr[0] = dword;
	m_CurrentAddr++;
}
#endif


#ifdef F_AddQWord
void GifPacket::AddQWord(u128 qword)
{
	*(u128*)m_CurrentAddr = qword;
	m_CurrentAddr+=2;

	Check();
}
#endif

#ifdef F_AddQWord2
void GifPacket::AddQWord(u64 high, u64 low)
{
	m_CurrentAddr[0] = low;
	m_CurrentAddr[1] = high;
	m_CurrentAddr+=2;

	Check();
}
#endif

#ifdef F_AddAD
void GifPacket::AddAD(u64 address, u64 data)
{
	//printf("context %i\n", m_Context);
	/*
	printf("data %08X\n", (u32)data);
	printf("addr %08X\n", (u32)address);
	*/
	if(m_Context > 0)
	{
		switch(address)
		{
			case GS_REG_TEX0:
			case GS_REG_CLAMP:
			case GS_REG_TEX1:
			case GS_REG_TEX2:
			case GS_REG_XYOFFSET:
			case GS_REG_MIPTBP1:
			case GS_REG_MIPTBP2:
			case GS_REG_SCISSOR:
			case GS_REG_ALPHA:
			case GS_REG_TEST:
			case GS_REG_FBA:
			case GS_REG_FRAME:
			case GS_REG_ZBUF:
				address+=m_Context;
			break;
		}
	}

//	printf("data %08X\n", (u32)data);
//	printf("addr %08X\n", (u32)address);
	
	m_CurrentAddr[0] = data;
	m_CurrentAddr[1] = address;
	m_CurrentAddr+=2;
}
#endif

#ifdef F_Reset
void GifPacket::Reset()
{
	if(m_TextureList)
	{
		delete m_TextureList;
		m_TextureList = 0;
	}
	
	if(m_TagList)
	{
		for(u32 i=0; i < m_TagList->Size(); i++)
		{
			free(m_TagList->Get(i).addr);
			m_TagList->Remove(i);
		}

		delete m_TagList;
		m_TagList = 0;
	}

	m_TagList = new LinkedList<Tag>;
	
	AllocateDmaTag();

	m_CurrentAddr =  m_TagList->Last().addr;
	m_LastDmaTag = m_CurrentAddr;

	m_LastDmaTag[0] = DMA_TAG(0, 0, DMA_TAG_ID_CNT, 0, 0, 0);
	m_LastDmaTag[1] = 0;

	m_CurrentAddr+=2;
}
#endif


#ifdef F_GetUsedSize
u32 GifPacket::GetUsedSize()
{
	return ((u8*)m_CurrentAddr)-((u8*)m_TagList->Last().addr)+m_Size;
}
#endif


#ifdef F_GetTotalSize
u32 GifPacket::GetTotalSize()
{
	return m_TagList->Last().size+m_Size;
}
#endif

#ifdef F_Wait
u32 GifPacket::Wait(u32 mode)
{
	if(mode == 0)
	{
		while(D2_CHCR & 0x100);
	}
	else
	{
		if(D2_CHCR & 0x100) return 1;
	}

	//printf("mode %i\n", mode);

	return 0;
}
#endif

/*
#ifdef F_StartChain
void GifPacket::StartChain()
{
	m_Mode = TransferSourceChain;
	m_LastDmaTag = m_CurrentAddr;
	m_CurrentAddr[1] = 0;
	m_CurrentAddr+=2;

}
#endif
*/

#ifdef F_AddData
void GifPacket::AddData(u8* address, u32 size)
{
	//u32 qwc = (((u8*)m_CurrentAddr - (u8*)m_LastDmaTag)/16)-1;
	
	//m_LastDmaTag[0]	= DMA_TAG(qwc, 0, DMA_TAG_ID_NEXT, 0, ((u32)m_CurrentAddr) & 0x7FFFFFFF, 0); // update last dma tag
	
	u32 ntags		= size / DMA_MAX_SIZE;
	u32 ntags_rest	= size % DMA_MAX_SIZE;
	
	for(u32 i=0; i < ntags; i++) 
		DmaTagRef(address+(i*DMA_MAX_SIZE), DMA_MAX_SIZE);

		/*
	{
		m_CurrentAddr[0] = DMA_TAG(DMA_MAX_SIZE/16, 0, DMA_TAG_ID_REF, 0, ((u32)((u8*)address)+(i*DMA_MAX_SIZE)) & 0x7FFFFFFF, 0); // set dmatag for transfer
		m_CurrentAddr[1] = 0;
		m_CurrentAddr+=2;
}
		*/
	

	if(ntags_rest) 
		DmaTagRef(address+(ntags*DMA_MAX_SIZE), ntags_rest);
	/*
	{

		m_CurrentAddr[0] = DMA_TAG(ntags_rest/16, 0, DMA_TAG_ID_REF, 0, ((u32)((u8*)address)+(ntags*DMA_MAX_SIZE)) & 0x7FFFFFFF, 0); // set dmatag for transfer
		m_CurrentAddr[1] = 0;
		m_CurrentAddr+=2;
	}
	*/

	DmaTagCnt();
/*
	m_LastDmaTag = m_CurrentAddr;
	m_CurrentAddr[1] = 0;	
	
	m_CurrentAddr+=2;
*/
}
#endif
/*
#ifdef F_EndChain
void GifPacket::EndChain()
{
	u32 qwc = (((u8*)m_CurrentAddr - (u8*)m_LastDmaTag)/16)-1;

	m_LastDmaTag[0] = DMA_TAG(qwc, 0, DMA_TAG_ID_NEXT, 0, ((u32)m_CurrentAddr) & 0x7FFFFFFF, 0); // update last dma tag
	m_LastDmaTag[1] = 0;

	m_CurrentAddr[0] = DMA_TAG(0, 0, DMA_TAG_ID_END, 0, 0, 0);
	m_CurrentAddr[1] = 0;

	m_CurrentAddr+=2;
}
#endif
*/

#ifdef F_Dump
void GifPacket::Dump()
{
//	u32 size = GetUsedSize()/8;

//	printf("GifPacket@0x%08X: (QWC %i)\n", (u32)m_PacketStart, GetCurrentSize()/16);

	for(u32 t=0; t < m_TagList->Size(); t++)
	{
		u32 size;
		
		if(t == (m_TagList->Size()-1))
			size = (((u8*)m_CurrentAddr)-((u8*)m_TagList->Last().addr))/8;
		else
			size = m_TagList->Get(t).size/8;
		
		u64* addr = m_TagList->Get(t).addr;
		
		for(u32 i=0; i< size; i++)
			printf("%08X: %08X : %08X_%08X\n", (u32)&addr[i], i*8, (u32)(addr[i] >> 32), (u32)(addr[i]));
	
	}

}
#endif

#ifdef F_Send

void GifPacket::Send()
{
//	printf("Used size %i\n", GetUsedSize());
	
	if(GetUsedSize() <= 32) return;

	

	FlushCache(0);

	Wait();
	/*
	printf("DUMP!\n\n");
	Dump();
*/
/*	
	if(m_Mode == TransferNormal)
	{
		D2_QWC	= GetCurrentSize()/16;
		D2_MADR	= (u32)m_PacketStart;
		D2_CHCR	= DMA_DIR_FROM_MEM | DMA_START;

		EE_SYNCP();
	}
	else
	{*/
		// Send chain packet
		D2_QWC	= 0;
		D2_TADR	= (u32)m_TagList->Get(0).addr;
		D2_CHCR	= DMA_MODE_CHAIN | DMA_DIR_FROM_MEM | DMA_START;
//	}

	if(m_TextureList)
	{
		for(u32 i=0; i < m_TextureList->Size(); i++)
		{
			m_TextureList->Get(i)->flag |= Texture::FlagInVram; 
		}
	}
}
#endif

#ifdef F_AddUploadTexture
#define GIFTAG_IMAGE_MAX_SIZE	(0x7FFF*16)

bool GifPacket::AddUploadTexture(Texture& texture)
{	
	if(texture.addr == 0) return false;

	if(texture.IsInVram() && !(texture.flag & Texture::FlagAlwaysUpload)) return true;
	
	if(texture.IsAllocated() == false)
	{
		if(texture.Allocate() == false)
			return false;
	}

	if(texture.HasClut())
	{
		if(AddUploadTexture(*texture.clut) == false)
			return false;
	}

	if(m_TextureList == 0) m_TextureList = new LinkedList<Texture*>;
	
	m_TextureList->Add(&texture);

	u32 packets			= texture.Size() / GIFTAG_IMAGE_MAX_SIZE;
	u32 packets_rest	= texture.Size() % GIFTAG_IMAGE_MAX_SIZE;

	u32 width = texture.width;
	u32 height = texture.height;
	
	u32 psm;

	if(texture.IsClut())
		psm = texture.Csm();
	else
		psm = texture.Psm();
	
	u32 tbw = texture.tbw;
	
	if(texture.flag & Texture::FlagSwizzled)
	{	
		switch(psm)
		{
			case RGBA16:
			{
				width = texture.height;
				height = texture.width/2;
			}
			break;

			case CLUT8:
			{
				width = texture.width/2;
				height = texture.height/2;
			}
			break;

			case CLUT4:
			{
				width = texture.height / 2;
				height = texture.width / 4;
			}
			break;
		}
		
		psm = RGBA32;
		tbw = width;
	}
	else
	{
		width = texture.width;
		height = texture.height;
	}

	//printf("upload %i, %i, %i\n", psm, width, height);
	

	// Setup packets which are GIFTAG_IMAGE_MAX_SIZE long.
	for(u32 i=0; i < packets; i++)
	{	
		// Setup address in GS memory
		Giftag(4, DISABLE, DISABLE, 0, GIFTAG_PACKED, 1, GIFTAG_AD);
		AddAD(GS_REG_BITBLTBUF, GS_BITBLTBUF( 0, 0, 0, texture.Base()+(i*GIFTAG_IMAGE_MAX_SIZE), tbw, psm));
		AddAD(GS_REG_TRXPOS, GS_TRXPOS(0, 0, 0, 0, 0));
		AddAD(GS_REG_TRXREG, GS_TRXREG(width, height));
		AddAD(GS_REG_TRXDIR, GS_TRXDIR(0));

		// Set part of image to transfer
		Giftag(GIFTAG_IMAGE_MAX_SIZE/16, DISABLE, 0, 0, GIFTAG_IMAGE, 0, 0);
		AddData(((u8*)texture.addr)+(i*GIFTAG_IMAGE_MAX_SIZE), GIFTAG_IMAGE_MAX_SIZE);
	}

	// Setup any remaining part of the texture which is less than GIFTAG_IMAGE_MAX_SIZE long.
	if(packets_rest)
	{
		// Setup address in GS memory
		Giftag(4, DISABLE, DISABLE, 0, GIFTAG_PACKED, 1, GIFTAG_AD);
		AddAD(GS_REG_BITBLTBUF, GS_BITBLTBUF( 0, 0, 0, texture.Base()+(packets*GIFTAG_IMAGE_MAX_SIZE), tbw, psm));
		AddAD(GS_REG_TRXPOS, GS_TRXPOS(0, 0, 0, 0, 0));
		AddAD(GS_REG_TRXREG, GS_TRXREG(width, height));
		AddAD(GS_REG_TRXDIR, GS_TRXDIR(0));

		// Set part of image to transfer
		Giftag(packets_rest/16, DISABLE, 0, 0, GIFTAG_IMAGE, 0, 0);
		AddData(((u8*)texture.addr)+(packets*GIFTAG_IMAGE_MAX_SIZE), packets_rest);
	}

	// Texture Flush, also set EOP (End Of Packet) bit
	Giftag(1, ENABLE, DISABLE, 0, GIFTAG_PACKED, 1, GIFTAG_AD);
	AddAD(GS_REG_TEXFLUSH, 0);

	return true;
}
#endif

#ifdef F_UpdateTextures
bool GifPacket::UpdateTextures()
{
	/*
	if(m_TextureList == 0) return true;

	for(u32 i=0; i < m_TextureList->Size(); i++)
	{
		
		if(!m_TextureList->Get(i).texture->IsAllocated())
				m_TextureList->Get(i).texture->Allocated();

		u64* addr = m_TextureList->Get(i).addr;

		while(*addr != Giftag(1, ENABLE, DISABLE, 0, GIFTAG_PACKED, 1))
		{
			if(*addr == GIFTAG(4, DISABLE, DISABLE, 0, GIFTAG_PACKED, 1))
			{
				addr+=2;
				

			}


			addr++;
		}

		


	}

	*/


	return true;
}
#endif

#ifdef F_AddSetRegister
void GifPacket::AddSetRegister(u64 address, u64 data)
{
	Giftag(1, ENABLE, DISABLE, 0, GIFTAG_PACKED, 1, GIFTAG_AD);
	AddAD(address, data);
	Check();
}
#endif


#ifdef F_AddSetTexture
bool GifPacket::AddSetTexture(Texture& texture)
{	
	AddSetRegister(GS_REG_TEX0, GS_TEX0(texture.tbp<<8, texture.tbw, texture.psm, texture.width, texture.height, 0, 1, 0, 0, 0, 0, 0));

	return true;
}
#endif


#ifdef F_End
void GifPacket::End()
{	
	if((m_CurrentAddr-2) == m_LastDmaTag)
	{
		m_CurrentAddr-=2;
		m_CurrentAddr[0] = DMA_TAG(0, 0, DMA_TAG_ID_END, 0, 0, 0);
		m_CurrentAddr[1] = 0;	
	}
	else
	{
		DmaTagEnd();
	}

	Tag last_tag = m_TagList->Last();
	
	u32 size = (((u8*)m_CurrentAddr)-((u8*)last_tag.addr));

	last_tag.size = size;
	
	realloc(last_tag.addr, last_tag.size);

	m_TagList->Set(m_TagList->Size()-1, last_tag);

//	printf("GifPacket::End()\n");

}
#endif
