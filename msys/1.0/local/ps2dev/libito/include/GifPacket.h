#ifndef _ITO_GIFPACKET_H
#define _ITO_GIFPACKET_H

#include <ItoTypes.h>
#include <LinkedList.h>
#include <TextureManager.h>

namespace Ito
{
	class GifPacket;
}

/** GS InterFace Packet (PATH3). 

	\par About
	GifPacket is an easy to use and automatic packet allocator for PATH3 (EE->GS) using DMA chains.
	You do not need to worry about packet allocation or creating DMA chains, everything is handled automaticly
	by GifPacket.

	\par Usage
	Simple create a GifPacket, add all the data you wish to it and then call End() and once you are
	ready to send the packet use Send() to send it. You can also add the packet to a master packet with
	AddPacket and only send the master packet which will also send all the packets added to it.

*/
class Ito::GifPacket
{
	private:
		typedef struct
		{	
			u64*	addr;
			u32		size;
		} Tag;
		
		typedef struct
		{
			Texture* texture;
			u64*	 addr; // Address in packet where upload gifpacket starts.

		} TextureRef;

		LinkedList<Tag>			*m_TagList;
		LinkedList<Texture*>	*m_TextureList;
		u64* m_LastDmaTag;
		u64* m_CurrentDmaTagEnd;
		u64* m_CurrentAddr;
		u32 m_Size;
		u16 m_Context;
	private:
		void Check();
		void AllocateDmaTag();
		void UpdateLastDmaTag();
		/* It is possible to add data after this tag, data is next to the tag.  */
		void DmaTagCnt();
		/* It is possible to add data after this tag, data is next to the tag.  */
		void DmaTagNext(u64* nextTag);
		/* The next dmatag comes right after this tag. */		
		void DmaTagRef(u8* address, u32 size);
		/* It is possible to add data after this tag, data is next to the tag.  */
		void DmaTagCall(u64* nextTag);
		/* It is possible to add data after this tag, data is next to the tag.  */
		void DmaTagRet();
		/* It is possible to add data after this tag, data is next to the tag.  */
		void DmaTagEnd();
	public:
		/** This construct does not allocate any memory for the packet, it only resets
			all the members. **/
		GifPacket();
		/** Allocates a packet with size. 
			\param size size of packet in bytes. */	
		//GifPacket(u32 maxSize);
		/** Destructor */
		~GifPacket();
		/** Add a GifTag to the GIF Packet, takes up 128 bits. Please refer to the GS User Manual for details. */
		void Giftag(s32 nloop, s32 eop, s32 pre, s32 prim, s32 flg, s32 nreg, u64 regs);
		/** Image Transfer */
		void Image(void* src, u32 size);
		/** Resets the packet, clears the packet. */
		void Reset();
		/** Add set register to the packet. Sets a GPR GS register, A+D mode*/
		void AddSetRegister(u64 address, u64 data);
		/** Add set texture to packet, to simplify texture handling.*/
		bool AddSetTexture(Texture& texture);
		/** Add Double Word (64 bits) to packet. The packet must at all times be 16 byte aligned and a dword
		is only 8 bytes.*/
		void AddDWord(u64 dword);
		/** Add a Quad Word (128 bits) to the packet */
		void AddQWord(u128 qword);
		/** Add a Quad Word (128 bits) as 2 Double Words (64 bits) to the packet.*/
		void AddQWord(u64 high, u64 low);
		/** Add a Quad Word (128 bits) as 2 Double Words (64 bits) to the packet, especially meant for A+D mode.
			This method is different from addQWord in the sense that it will address the correct context for
			the register, set with setContext.

			\param address The register address. 
			<tt> GS_REG_PRIM, GS_REG_RGBAQ, GS_REG_ST, GS_REG_UV, GS_REG_XYZ2, GS_REG_XYZ3</tt>
			\param data data to set to the register (64 bits). */
		void AddAD(u64 address, u64 data);
		/** Add data to packet. Add data you also wish to send at this point in the packet, main usage is
		for image transfer on PATH3. */
		void AddData(u8* address, u32 size);
		void AddPacket(GifPacket* packet);
		/** Add Texture upload to chain.  */
		bool AddUploadTexture(Texture& texture);
		/** Update all textures added with AddUploadTexture. Since textures might have new addresses in VRAM
			for everytime you re-send a packet of textures, the addresses in VRAM the textures get uploaded to need
			to be updated. */
		bool UpdateTextures();
		/** Add FpuVector */
		void AddVector(Vector v, s32 reg = 0);	
		/* Add IntVector */
		void AddVector(IntVector v, s32 reg = 0);
		/** Set GS Context, used by AddAD. */
		void SetContext(s32 context);
		/** Get GS Context. */
		u32	GetContext();
		/** Get used size of the total packet size. */
		u32 GetUsedSize();
		/** Before End() is called, you can get the amount of allocated size for the packet. */
		u32 GetTotalSize();
		/** Send the packet to GS. This function also flushes the cache. */
		void Send();
		/** End packet, must be called when doing creating a packet. This also trimes the packet to the size it occupies (total size = used size). */
		void End();
		/** Wait until GIF path (PATH3) is done sending. 
			\param mode Set block mode. 0 (default) will not return until the transfer is completed. 
										1 will return 0 if done sending or 1 if still transfering. */
		u32	Wait(u32 mode = 0);

		/** Uses printf to print the contents of the packet. */
		void Dump();
	
};
#endif
