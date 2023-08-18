#include <TextureManager.h>
#include <GsDefs.h>
#include <Gs.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

// #define DEBUG 1

using namespace Ito;

#ifdef F_Constructor
TextureManager::TextureManager()
{
	m_Time = 0;
}
#endif

#ifdef F_Destructor
TextureManager::~TextureManager()
{
	
}
#endif

#ifdef F_GetInstance
static TextureManager* instance = 0;
TextureManager* TextureManager::Instance()
{
	if(instance == 0)
	{
		instance = new TextureManager();
	}
	
	return instance;
}
#endif

#ifdef F_Tick
void TextureManager::Tick()
{
	m_Time++;
}
#endif

#ifdef F_Allocate
bool TextureManager::Allocate(Texture& texture)
{	

	#ifdef DEBUG
	printf("TextureManager::Allocate");
	#endif

	if(texture.IsAllocated()) return true;
	
	if((Gs::GetTextureBufferBase()+texture.Size()) > VRAM_SIZE)
		return false;
	
	#ifdef DEBUG
	printf(":Start\n");
	#endif

	if(m_TextureList.IsEmpty())
	{
		#ifdef DEBUG
		printf("Space found, list is emtpy\n");
		#endif

		texture.tbp = Gs::GetTextureBufferBase()/256;
		texture.time = m_Time;
		texture.flag |= Texture::FlagAllocated;
		m_TextureList.Add(&texture);
		
		return true;	
	}
	else
	{
		// Check for space after last texture
		//printf("Space After Last : %08X\n", Gs::GetTextureBufferBase()+m_TextureList.Last()->Base()+m_TextureList.Last()->Size()+texture.Size()

		if((m_TextureList.Last()->Base()+m_TextureList.Last()->Size()+texture.Size()) <= VRAM_SIZE)
		{
			#ifdef DEBUG
			printf("Found Space After Last Texture\n");
			#endif

			texture.tbp = (m_TextureList.Last()->Base()+m_TextureList.Last()->Size())>> 8;
			texture.time = m_Time;
			texture.flag |= Texture::FlagAllocated;

			m_TextureList.Add(&texture);
			
			return true;
		}
		else
		{
			// Search for space between textures and at the same find out which texture to replace in case
			// of no space
			
			#ifdef DEBUG
			printf("Search For Space\n");
			#endif

			// TODO: check for space before first texture

			Texture* texture_cur;
			Texture* texture_next = m_TextureList.Get(0);
			
			u32 replace_at_tbb = 0; // tbb = texturebufferbase

			u32 space_before_first = (texture_next->Base() - Gs::GetTextureBufferBase());
			
			u32 replace_size =  0; 

			if(space_before_first)
			{
				if(space_before_first >= texture.Size())
				{
					#ifdef DEBUG
					printf("Found Space Before First Texture, 0x%08X\n bytes", space_before_first);
					#endif
					texture.tbp = Gs::GetTextureBufferBase() >> 8;
					texture.time = m_Time;
					texture.flag |= Texture::FlagAllocated;
					m_TextureList.Add(0, &texture);

					return true;
				}
				else
				{	
					replace_at_tbb = 1;
					replace_size = space_before_first;
				}
			}

			u32 replace_single = 0;
			
			u32 replace_count = 0;
			u32 replace_index = 0;
			Texture* replace_texture = 0;
			
			u32 lowest_time = m_Time;
			u32 count = m_TextureList.Size();
			u32 i = 1;


			
			while(i < count)
			{	
				texture_cur = texture_next;
				texture_next = m_TextureList.Get(i);

				u32 space =	texture_next->Base()-(texture_cur->Base()+texture_cur->Size());
				
				if(space >= texture.Size())
				{
					#ifdef DEBUG
					printf("Space Found, Between 2 Textures, 0x%08X Bytes\n", space);
					#endif
					texture.tbp = (texture_cur->Base()+texture_cur->Size())/256;
					texture.time = m_Time;
					texture.flag |= Texture::FlagAllocated;
					
					m_TextureList.Add(i, &texture);
					
					return true;
				}
				else
				{
					// Find texture(s) to replace
					if((texture_cur->time < m_Time) && ((texture_cur->flag & Texture::FlagKeep) == 0))
					{
						if((texture_cur->Size() >= texture.Size()) && (texture_cur->time < lowest_time))
						{
							replace_index = i;
							replace_texture = texture_cur;
							replace_single = 1;
							lowest_time = texture_cur->time;
						}
						else
						{
							if(!replace_single && (replace_size < texture.Size()))
							{
								if(replace_count == 0)
								{
									replace_index = i;
									replace_texture = texture_cur;
								}
								

								replace_count++;
								replace_size += texture_cur->Size()+space;
							}
						
						}
					}	
					else
					{
						if(replace_size < texture.Size())
						{
							replace_count = 0;
							replace_size = 0;
							replace_at_tbb = 0;
						}
					}
				}
				i++;
			}

			// There was no space, now to check if there is a texture(s) which can be replaced

			// Single texture to replace
			if(replace_single)
			{
				#ifdef DEBUG
				printf("Space Found, Replacing A Single Texture, 0x%08X bytes, Time 0x%08X\n", replace_texture->Size(), replace_texture->time);
				#endif

				texture.tbp = replace_texture->tbp;
				texture.time = m_Time;
				m_TextureList.Set(replace_index, &texture);

				replace_texture->flag &= ~Texture::FlagAllocated;
			
				return true;
			}

			if((replace_size < texture.Size()) && (texture_next->time < m_Time))
			{
				replace_count++;
				replace_size += texture_next->Size();

				if(texture_next == m_TextureList.Last())
					replace_size += VRAM_SIZE-(Gs::GetTextureBufferBase()+m_TextureList.Last()->Base()+m_TextureList.Last()->Size());

			}
		
			if(replace_size >= texture.Size())
			{
				#ifdef DEBUG
				printf("Space Found, %i Textures Replaced, Total Of 0x%08X Bytes\n", replace_count, replace_size);
				#endif

				if(replace_at_tbb)
					texture.tbp = Gs::GetTextureBufferBase();
				else
					texture.tbp = replace_texture->tbp;
				
				texture.time = m_Time;
				m_TextureList.Set(replace_index, &texture);
				replace_texture->flag &= ~(Texture::FlagAllocated | Texture::FlagInVram);
				
				replace_count--;

				for(u32 i=0; i < replace_count; i++)
				{
					m_TextureList.Remove(replace_index);
				}	
			
				return true;
			}

			// Worst case, no space and no textures(s) which can be replaced.
			#ifdef DEBUG
			printf("No Space Found!\n");
			#endif

			return false;

		}

	}

	return false;	
}
#endif

#ifdef F_Free
void TextureManager::Free(Texture& texture)
{
	if(texture.IsAllocated())
	{
		Texture *curtex = 0;

		for(u32 i=0; i < m_TextureList.Size(); i++)
		{
			curtex = m_TextureList.Get(i);
			
			if(curtex == &texture)
			{
				m_TextureList.Remove(i);
				break;
			}
		}
	}
	
	texture.flag &= ~(Texture::FlagAllocated | Texture::FlagInVram);
}
#endif

#ifdef F_FreeAll
void TextureManager::FreeAll()
{
	m_Time = 0;
}
#endif

#ifdef F_TextureConstructor3
Texture::Texture()
{
	addr = 0;
	tbp = 0;
	tbs = 0;
	psm = 0;
	width = 0;
	height = 0;
	tbw = 0;
	tw = 0;
	th = 0;
	flag = 0;
	time = 0;
	clut = 0;
}
#endif

#ifdef F_TextureDestructor
Texture::~Texture()
{
	if(flag & FlagFile) free(addr);
	
	Free();

	memset(this, 0, sizeof(Texture));
}
#endif

#ifdef F_CreateTexture2
bool Texture::CreateTexture(void* addr, u16 psm, u16 width, u16 height, Texture& clut)
{
	bool res;

	res = CreateTexture(addr, psm, width, height);

	if(res) this->clut = &clut;
	
	return res;
}
#endif


#ifdef F_CreateTexture
bool Texture::CreateTexture(void* addr, u16 psm, u16 width, u16 height)
{
	if(addr == 0) return false;

	TextureManager::Instance()->Free(*this);

	// Get texture buffer width
	if((psm == CLUT4) || (psm == CLUT8))
	{
		if(width % 128)
			tbw = width + 128 - (width % 128);
		else
			tbw = width;
	}
	else
	{
		if(width % 64)
			tbw = width + 64 - (width % 64);
		else
			tbw = width;
	}

	// Texturebuffer size
	u32 tbs;

	if(psm == RGB24)
		tbs = 4*tbw*height; // RGB24 actually takes up 4 bytes pr. pixel in vram.
	else
		tbs = Gs::GetTextureSize(psm, tbw, height);
	
	if(tbs % 256)
		tbs = (tbs + 256) - (tbs % 256);
	
	this->tbs = tbs >> 8;
	
	// Get log2(width)
	u32 bit = 10;
	while(((width >> bit) & 1) == 0) bit--;

	if(width == (1 << bit)) 
		tw = bit;
	else
		tw = bit+1;

	// Get log2(height)
	bit = 10;
	while(((height >> bit) & 1) == 0) bit--;

	
	if(height == (1 << bit))
		th = bit;
	else
		th = bit+1;
	

	this->psm		&= 0xFF;
	this->psm		|= psm;
	this->width		= width;
	this->height	= height;
	this->addr		= addr;

	time = TextureManager::Instance()->m_Time;

	return true;
}
#endif

#ifdef F_CreateClut
bool Texture::CreateClut(void* addr, u16 csm, u16 psm)
{
	if(addr == 0) return false;

	if(psm == CLUT8)
		width = 16;	
	else
		width = 8;

	if(psm == CLUT8)
		height = 16;
	else
		height = 2;
	
	this->psm  = (csm << 8) | psm;
	//addr = addr;
	tbw = 64;

//	printf("psm %i\n", psm);

	u32 size = Gs::GetTextureSize(csm, tbw, height);
	tbs = size >> 8;
	
	tbp = 0;

	this->addr = addr;

	flag |= FlagClut;
	time = TextureManager::Instance()->m_Time;

	return true;
}
#endif

#ifdef F_TextureSize
u32 Texture::Size()
{
	return (tbs << 8);
}
#endif

#ifdef F_TextureBase
u32 Texture::Base()
{
	return (tbp << 8);
}
#endif


#ifdef F_TextureKeep
void Texture::Keep(bool enable)
{
	if(enable)
		flag |= FlagKeep;
	else
		flag &= ~FlagKeep;
}
#endif

#ifdef F_TextureIsClut
bool Texture::IsClut()
{
	return (bool)((flag & FlagClut) > 0);
}
#endif

#ifdef F_TextureIsAllocated
bool Texture::IsAllocated()
{
	return (bool)((flag & FlagAllocated) > 0);
}
#endif

#ifdef F_TextureIsInVram
bool Texture::IsInVram()
{
	return (bool)((flag & FlagInVram) > 0);
}
#endif

#ifdef F_TextureIsCreated
bool Texture::IsCreated()
{
	if(addr)
		return true;
	else
		return false;
}
#endif

#ifdef F_TextureAlwaysUpload
void Texture::AlwaysUpload(bool enable)
{
	if(enable)
		flag |= FlagAlwaysUpload;
	else
		flag &= ~FlagAlwaysUpload;
}
#endif

#ifdef F_TexturePsm
u32 Texture::Psm()
{
	return (psm & 0xFF);
}
#endif

#ifdef F_TextureCsm
u32 Texture::Csm()
{
	return ((psm >> 8) & 0xFF);
}
#endif

#ifdef F_TextureHasClut
bool Texture::HasClut()
{
	return (bool)clut;
}
#endif

#ifdef F_TextureLoadImageBuffer

#define IIF_RGBA32			0
#define IIF_RGB24			1
#define IIF_RGBA16			2

#define IIF_CLUT8_RGBA32	3
#define IIF_CLUT8_RGBA16	4
#define IIF_CLUT4_RGBA32	5
#define IIF_CLUT4_RGBA16	6

#define IIF_OPTION_SWIZZLE	(1 << 1)
#define IIF_OPTION_NOCLUT	(1 << 2)
#define IIF_OPTION_ONLYCLUT	(1 << 3)

// IIF v1 - Ito Image Format
typedef struct{
	u32 identifier; // 'IIF1' - IIFv (v = version of iif)
	u32 width;
	u32 height;
	u32 psm;
} IIFHeader;

// IIF v2 - Ito Image Format
typedef struct{
	u32 identifier; // 'IIF2'
	u32 headerSize;
	u16 width;
	u16 height;
	u16 psm;
	u16 options;
	char   name[16];
} IIF2Header;

static u32 IIFGetPSM(u32 psm)
{
	switch(psm)
	{
		case IIF_CLUT8_RGBA32:
		case IIF_CLUT8_RGBA16:
			return CLUT8;
		
		case IIF_CLUT4_RGBA32:
		case IIF_CLUT4_RGBA16:
			return CLUT4;

		default: return psm;
	}
}

static u32 IIFGetCSM(u32 psm)
{
	switch(psm)
	{
		case IIF_CLUT8_RGBA32:
		case IIF_CLUT4_RGBA32:
			return RGBA32;
		
		
		case IIF_CLUT8_RGBA16:
		case IIF_CLUT4_RGBA16:
			return RGBA16;

		default: return 0;
	}
}

static bool IIFHasClut(u32 psm)
{
	if(psm > IIF_RGBA16)
		return true;
	else
		return false;
}	

static u32 IIFGetClutSize(u32 psm)
{
	switch(psm)
	{
		case IIF_CLUT8_RGBA32: return (256*4);
		case IIF_CLUT8_RGBA16: return (256*2);
		case IIF_CLUT4_RGBA32: return (16*4);
		case IIF_CLUT4_RGBA16: return (16*2);
		default: return 0;
	}
}


bool Texture::LoadImageBuffer(u8* buffer, bool allocate)
{
	u32* pBuffer = (u32*)buffer;
	u32 width = 0, height = 0, psm = 0, csm = 0;
	u8* addr = 0;
	u8* clut_addr = 0;
	u32 iif_psm = 0;

	switch(pBuffer[0])
	{
		case 0x31464949: // IIF (Ito Image Format v1)
		{
			IIFHeader *header = (IIFHeader*)buffer;
			
			width = header->width;
			height = header->height;
			psm = IIFGetPSM(header->psm);
			addr = buffer+sizeof(IIFHeader)+IIFGetClutSize(header->psm);
		
			if(IIFHasClut(header->psm))
			{
				csm = IIFGetCSM(header->psm);
				clut_addr = buffer+sizeof(IIFHeader);
			}
		} break;

		case 0x32464949: // IIF (Ito Image Format v2)
		{
			IIF2Header *header = (IIF2Header*)buffer;

			iif_psm = header->psm;
	
			psm = IIFGetPSM(header->psm);

		//	printf("PSM %i\n", psm);
			
			if(IIFHasClut(header->psm) && (!(header->options & IIF_OPTION_NOCLUT)))
			{
				csm = IIFGetCSM(header->psm);
				clut_addr = buffer+header->headerSize+8;
			}

			if(header->options & IIF_OPTION_SWIZZLE)
				flag |= FlagSwizzled;

			if(header->options & IIF_OPTION_ONLYCLUT)
			{
				bool result = CreateClut(clut_addr, csm, psm);

				if(!result) return result;
	
				if(allocate)
					return Allocate();
				else
					return true;
			}

			width = header->width;
			height = header->height;
			addr = buffer+header->headerSize+8;

			if((header->options & IIF_OPTION_NOCLUT) == 0)
			{
				addr += IIFGetClutSize(header->psm);
				//printf("SIZE %i %i\n", IIFGetClutSize(header->psm), sizeof(IIF2Header));
			}

		} break;

		default: 
			return false; 

	}

	if(IIFHasClut(iif_psm))
	{
		Texture *tex_clut = new Texture();
				
		if(tex_clut->CreateClut(clut_addr, csm, psm))
		{
			// Set clut
			clut = tex_clut;
			
			if(allocate)
			{
				if(tex_clut->Allocate() == false)
					return false;
			}
		}
		else
		{
			delete clut;
			return false;
		}	
		
	}

	if(!CreateTexture(addr, psm, width, height))
		return false;
	
	if(allocate)
		return Allocate();
	else
		return true;
}
#endif

#ifdef F_TextureLoadImageFile
bool Texture::LoadImageFile(char* filename, bool allocate)
{

	s32 fd = fioOpen(filename, O_RDONLY);

	if(fd < 0) return false;

	s32 size = fioLseek(fd, 0, SEEK_END);
	
	if(size < 0) return false;

	void* addr = memalign(16, size);

	fioLseek(fd, 0, SEEK_SET);

	if(fioRead(fd, addr, size) < 0)
		return false;

	fioClose(fd);

	if(LoadImageBuffer((u8*)addr, allocate) == 0) return false;

	flag |= Texture::FlagFile;

	return true;
}
#endif

#ifdef F_TextureAllocate
bool Texture::Allocate()
{
	if(addr == 0) return false;
	
	return TextureManager::Instance()->Allocate(*this);
}
#endif

#ifdef F_TextureFree
void Texture::Free()
{
	TextureManager::Instance()->Free(*this);
}
#endif

#ifdef F_TextureDump
void Texture::Dump()
{
	printf("[Texture : %08X]\n", (u32)this);
	printf("Addr : %08X\n", (u32)addr);
	printf("tbp : %08X\n", Base());
	printf("size : %08X\n", Size());
	printf("psm : %i\n", psm & 0xFF);
	printf("csm : %i\n", (psm >> 8) & 0xFF);
	printf("width : %i\n", width);
	printf("height : %i\n", height);
	printf("tbw : %i\n", tbw);
	
	if(!IsClut())
	{
		printf("tw : %i\n", (1 << tw));
		printf("th : %i\n", (1 << th));
	}
	
	printf("flag : ");
	
	if(flag & FlagAllocated)
		printf("Allocated, ");

	if(flag & FlagInVram)
		printf("In VRAM, ");

	if(flag & FlagClut)
		printf("Clut, ");

	if(flag & FlagFile)
		printf("File, ");

	if(flag & FlagKeep)
		printf("Keep, ");

	if(flag & FlagSwizzled)
		printf("Swizzled, ");
	
	printf("\ntime : %08X\n", time);

	if(clut)
	{
		printf("Clut\n");
		clut->Dump();
	}
}
#endif


