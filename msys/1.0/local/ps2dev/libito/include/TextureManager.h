#ifndef _ITO_TEXTUREMANAGER_H
#define _ITO_TEXTUREMANAGER_H

#include <ItoTypes.h>
#include <LinkedList.h>


namespace Ito
{
	/** Texture description class. All public members of this class are readable, but are not for writing, use the constructors and functions
	for setting the values.
	*/
	class Texture;
	class TextureManager;
}

/** Texture class for easier texture handling. 
	
	\par About
	One of the annoying parts about graphics programming on the PS2 is manual texture management,
	which is why libito has automatic texture management which can be either transparent or with some
	control.
	The texture management uses a Least-Recently-Used (LRU) algorithm which is used when replacing
	textures in VRAM, the textures which have been in VRAM for the longest time are most likely to
	be replaced. For instance if a texture A hasn't been used for 20 frames and texture B needs to
	be uploaded to VRAM and they are both the same size, its likely that texture B will replace
	texture A. However it is possible to keep texture in VRAM, meaning that they can not be replaced,
	this could be useful for a feedback buffer for instance.

	\par Transparent Control VS User Control
	The difference between transparent control and use control is that in user control the user
	free's the texture himself from VRAM and with transparent control he lets the texture manager
	free textures. It can sometimes be an advantage to use manual control on when alot
	of free'ing is needed, with the texture manager you could encounter gaps in VRAM which could have
	been avoided with manual control. However for most small apps and demos, the texture manager will
	do just fine.

	\par Usage
	

*/
class Ito::Texture
{
	public:
		/* Used with flag (internal */
		static const u32 FlagAllocated	= (1 << 0);
		/* Used with flag (internal */
		static const u32 FlagInVram		= (1 << 1);
		/* Used with flag (internal) */
		static const u32 FlagClut		= (1 << 2);
		/* Used with flag (internal) */
		static const u32 FlagFile		= (1 << 3);
		/* Used with flag (internal) */
		static const u32 FlagKeep		= (1 << 4);
		/* Used with flag (internal) */
		static const u32 FlagSwizzled	= (1 << 5);
		/* Used with flag (internal) */
		static const u32 FlagAlwaysUpload = (1 << 6);

		/** ee memory address of the texture */
		void*	addr ALIGN(4);	
		/** VRAM texture base pointer / 256 */
		u16	tbp;
		/** Texture buffer size / 256 */
		u16	tbs;	
		/** Texture pixel/clut storage mode */
		u16	psm;	
		/** Texture width */
		u16	width;	
		/** Texture height */
		u16	height; 
		/** Texturebuffer width */
		u16	tbw;	
		/** Log2(texture width) */
		u16	tw;
		/** Log2(texture height) */
		u16	th;	
		/* Flag (internal)*/
		u32	flag;
		/* Logical time (internal)*/
		u32	time;
		/** Pointer to possible clut */
		Texture*	clut;
	public:
		/** Texture Constructor. */
		Texture();
		/** Destructor. */
		~Texture();	
		
		bool Allocate();
		void Free();
		bool LoadImageBuffer(u8* buffer, bool allocate = true);
		bool LoadImageFile(char* filename, bool allocate = true);
		
		/** Create Texture. 
			Setups the texture class as a texture (as opposed to a CLUT)
			\param addr pointer to the texture
			\param psm Pixel Storage Mode
			\param width width of texture
			\param height height of texture
			\param clut pointer to a possible CLUT.
		*/
		bool CreateTexture(void* addr, u16 psm, u16 width, u16 height, Texture& clut);
		
		bool CreateTexture(void* addr, u16 psm, u16 width, u16 height);
		/** Create CLUT.
			Setups the texture class as a CLUT (as opposed to a texture)
			\param addr pointer to the clut
			\param csm CLUT (PSM) Storage Mode of the CLUT
			\param psm Pixel Storage Mode of the CLUT
		*/
		bool CreateClut(void* addr, u16 csm, u16 psm);
		
		/** Get texture size in bytes. This returns the size requires for the texture in VRAM. 
			\return size 
		*/

		u32 Psm();
		u32 Csm();
		bool HasClut();
		bool IsClut();

		u32 Size();
		/** Get base at which the texture is allocated (if at all). 
			\return -1 if not allocated other otherwise the base.
		*/
		u32 Base();
		/** Check if texture is allocated. 
			\return true if allocated otherwise false
		*/
		bool IsAllocated();

		

		bool IsInVram();
		/** Check if the texture object is created.
		\param return false if the object does not point at a texture, true if the opposite.
		*/
		bool IsCreated();
		/** Keep texture in VRAM. This basicly means that this texture will never
		    get overwritten by the texture allocator.
			\param enable true to enable, false to disable.
		*/
		void Keep(bool enable);
		/** Alawys upload texture when set (SetTexture). 
		\param enable true to enable, false to disable.
		*/
		void AlwaysUpload(bool enable);

		void Dump();

		void SetClut(Texture& texture);
		Texture GetClut();

		

};

/** Texture Manager */
class Ito::TextureManager
{
	private:
		LinkedList<Texture*> m_TextureList;
	public:
		u32 m_Time;
	private:
		TextureManager();
	public:
		static TextureManager* Instance();
		~TextureManager();
		bool Allocate(Texture& texture);
		void Free(Texture& texture);
		void FreeAll();
		void Tick();
};

#endif
