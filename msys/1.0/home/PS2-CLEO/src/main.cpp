/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/************************************************************************/
/*							Address Wrappers							*/
/************************************************************************/

#include "wrappers.h"

char * erl_id = "cleo_ps2";

#if 0
#define DEBUG_TRACE(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG_TRACE(a...) do { } while(0)
#endif

/************************************************************************/
/*						CLEO New functions								*/
/************************************************************************/

static inline void SkipUnusedParameters(CRunningScript *thread)
{
	while (*thread->GetBytePointer()) GetScriptParams(thread, 1);	// skip parameters
	thread->ReadDataByte();
}

static inline void ThreadJump(CRunningScript *thread, int off)
{
	thread->SetIp(off < 0 ? thread->GetBasePointer() - off : (BYTE*)&CTheScripts_ScriptSpace + off);
}


struct ScmFunction
{
	unsigned short prevScmFunctionId, thisScmFunctionId;
	BYTE *retnAddress;
	SCRIPT_VAR savedTls[32];
	char stringParams[3][128]; // texts with this scope lifetime
	int lastStringIndex;
	bool savedCondResult;
	WORD savedLogicalOp;
	bool savedNotFlag;
	static const size_t store_size = 0x400;
	static ScmFunction *Store[store_size];
	static size_t allocationPlace;			// contains an index of last allocated object

	void *operator new(size_t size)
	{
		size_t start_search = allocationPlace;
		while (Store[allocationPlace])	// find first unused position in store
		{
			if (++allocationPlace >= store_size) allocationPlace = 0;		// end of store reached
			//if (allocationPlace == start_search) throw std::bad_alloc();	// the store is filled up
		}
		ScmFunction *obj = reinterpret_cast<ScmFunction *>(::operator new(size));
		Store[allocationPlace] = obj;
		return obj;
	}

	void operator delete(void *mem)
	{
		Store[reinterpret_cast<ScmFunction *>(mem)->thisScmFunctionId] = (ScmFunction *)NULL;
		::operator delete(mem);
	}

	ScmFunction(CRunningScript *thread) : prevScmFunctionId(reinterpret_cast<CRunningScript*>(thread)->GetScmFunction())
	{
		CRunningScript* cs = reinterpret_cast<CRunningScript*>(thread);

		// create snapshot of current scope
		SCRIPT_VAR* scope = cs->IsMission() ? missionLocals : cs->LocalVar;
		memcpy(savedTls, scope, 32*sizeof(SCRIPT_VAR));
		savedCondResult = cs->bCondResult;
		savedLogicalOp = cs->LogicalOp;
		savedNotFlag = cs->NotFlag;

		cs->bCondResult = false;
		cs->LogicalOp = NONE;
		cs->NotFlag = false;

		cs->SetScmFunction(thisScmFunctionId = allocationPlace);
	}

	void Return(CRunningScript *thread)
	{
		// restore parent scope's local variables
		CRunningScript* cs = reinterpret_cast<CRunningScript*>(thread);
		memcpy((cs->IsMission() ? missionLocals : cs->LocalVar), savedTls, 32*sizeof(SCRIPT_VAR));

		// process conditional result of just ended function in parent scope
		bool condResult = cs->bCondResult;
		if (savedNotFlag) condResult = !condResult;

		if (savedLogicalOp >= AND_2 && savedLogicalOp < AND_END)
		{
			cs->bCondResult = savedCondResult && condResult;
			cs->LogicalOp = DecLogicalOperation(savedLogicalOp);
		}
		else if(savedLogicalOp >= OR_2 && savedLogicalOp < OR_END)
		{
			cs->bCondResult = savedCondResult || condResult;
			cs->LogicalOp = DecLogicalOperation(savedLogicalOp);
		}
		else // eLogicalOperation::NONE
		{
			cs->bCondResult = condResult;
			cs->LogicalOp = savedLogicalOp;
		}

		cs->SetIp(retnAddress);
		cs->SetScmFunction(prevScmFunctionId);
	}
};

ScmFunction *ScmFunction::Store[store_size] = { /* default initializer - nullptr */ };
size_t ScmFunction::allocationPlace = 0;

void ResetScmFunctionStore()
{
	for (int i = 0; i < ScmFunction::store_size; i++) {
		if (ScmFunction::Store[i]) delete ScmFunction::Store[i];
	}
	ScmFunction::allocationPlace = 0;
}


enum OpcodeResult
{
    OR_CONTINUE,
    OR_INTERRUPT
};

typedef OpcodeResult(* CustomOpcodeHandler)(CRunningScript*);

static OpcodeResult opcode_defa(CRunningScript *thread);
static OpcodeResult opcode_0A8C(CRunningScript *thread);
static OpcodeResult opcode_0A8D(CRunningScript *thread);
static OpcodeResult opcode_0A8E(CRunningScript *thread);
static OpcodeResult opcode_0A8F(CRunningScript *thread);
static OpcodeResult opcode_0A90(CRunningScript *thread);
static OpcodeResult opcode_0A91(CRunningScript *thread);
static OpcodeResult opcode_0A96(CRunningScript *thread);
static OpcodeResult opcode_0A97(CRunningScript *thread);
static OpcodeResult opcode_0A98(CRunningScript *thread);
static OpcodeResult opcode_0A99(CRunningScript *thread);
static OpcodeResult opcode_0A9A(CRunningScript *thread);
static OpcodeResult opcode_0A9B(CRunningScript *thread);
static OpcodeResult opcode_0A9C(CRunningScript *thread);
static OpcodeResult opcode_0A9D(CRunningScript *thread);
static OpcodeResult opcode_0A9F(CRunningScript *thread);
static OpcodeResult opcode_0AA0(CRunningScript *thread);
static OpcodeResult opcode_0AA1(CRunningScript *thread);
static OpcodeResult opcode_0AA2(CRunningScript *thread);
static OpcodeResult opcode_0AA3(CRunningScript *thread);
static OpcodeResult opcode_0AA4(CRunningScript *thread);
static OpcodeResult opcode_0AA5(CRunningScript *thread);
static OpcodeResult opcode_0AA6(CRunningScript *thread);
static OpcodeResult opcode_0AA7(CRunningScript *thread);
static OpcodeResult opcode_0AA8(CRunningScript *thread);
static OpcodeResult opcode_0AA9(CRunningScript *thread);
static OpcodeResult opcode_0AB1(CRunningScript *thread);
static OpcodeResult opcode_0AB2(CRunningScript *thread);
static OpcodeResult opcode_0AB3(CRunningScript *thread);
static OpcodeResult opcode_0AB4(CRunningScript *thread);
static OpcodeResult opcode_0AB6(CRunningScript *thread);
static OpcodeResult opcode_0AB8(CRunningScript *thread);
static OpcodeResult opcode_0ABE(CRunningScript *thread);
static OpcodeResult opcode_0AC6(CRunningScript *thread);
static OpcodeResult opcode_0AC7(CRunningScript *thread);
static OpcodeResult opcode_0AC8(CRunningScript *thread);
static OpcodeResult opcode_0AC9(CRunningScript *thread);
static OpcodeResult opcode_0ACA(CRunningScript *thread);
static OpcodeResult opcode_0ACB(CRunningScript *thread);
static OpcodeResult opcode_0ACC(CRunningScript *thread);
static OpcodeResult opcode_0ACD(CRunningScript *thread);
static OpcodeResult opcode_0ACE(CRunningScript *thread);
static OpcodeResult opcode_0ACF(CRunningScript *thread);
static OpcodeResult opcode_0AD0(CRunningScript *thread);
static OpcodeResult opcode_0AD1(CRunningScript *thread);
static OpcodeResult opcode_0AD3(CRunningScript *thread);
static OpcodeResult opcode_0AD4(CRunningScript *thread);
static OpcodeResult opcode_0AD5(CRunningScript *thread);
static OpcodeResult opcode_0ADB(CRunningScript* thread);
static OpcodeResult opcode_0ADD(CRunningScript* thread);
static OpcodeResult opcode_0ADE(CRunningScript* thread);
static OpcodeResult opcode_0AE1(CRunningScript *thread);
static OpcodeResult opcode_0AE2(CRunningScript *thread);
static OpcodeResult opcode_0AE3(CRunningScript *thread);
static OpcodeResult opcode_0AE9(CRunningScript *thread);
static OpcodeResult opcode_0AEA(CRunningScript *thread);
static OpcodeResult opcode_0AEB(CRunningScript *thread);
static OpcodeResult opcode_0AEC(CRunningScript *thread);
static OpcodeResult opcode_0AEE(CRunningScript *thread);
static OpcodeResult opcode_0AEF(CRunningScript *thread);

static CustomOpcodeHandler customOpcodeHandlers[100] =
{
	opcode_0A8C, opcode_0A8D, opcode_0A8E, opcode_0A8F, opcode_0A90,
	opcode_0A91, opcode_defa, opcode_defa, opcode_defa, opcode_defa,
	opcode_0A96, opcode_0A97, opcode_0A98, opcode_0A99, opcode_0A9A,
	opcode_0A9B, opcode_0A9C, opcode_0A9D, opcode_defa, opcode_0A9F,
	opcode_0AA0, opcode_0AA1, opcode_0AA2, opcode_0AA3, opcode_0AA4,
	opcode_0AA5, opcode_0AA6, opcode_0AA7, opcode_0AA8, opcode_0AA9,
	opcode_defa, opcode_defa, opcode_defa, opcode_defa, opcode_defa,
	opcode_defa, opcode_defa, opcode_0AB1, opcode_0AB2, opcode_0AB3,
	opcode_0AB4, opcode_defa, opcode_0AB6, opcode_defa, opcode_0AB8,
	opcode_defa, opcode_defa, opcode_defa, opcode_defa, opcode_defa,
	opcode_0ABE, opcode_defa, opcode_defa, opcode_defa, opcode_defa,
	opcode_defa, opcode_defa, opcode_defa, opcode_0AC6, opcode_0AC7,
	opcode_0AC8, opcode_0AC9, opcode_0ACA, opcode_0ACB, opcode_0ACC,
	opcode_0ACD, opcode_0ACE, opcode_0ACF, opcode_0AD0, opcode_0AD1,
	opcode_defa, opcode_0AD3, opcode_0AD4, opcode_0AD5, opcode_defa,
	opcode_defa, opcode_defa, opcode_defa, opcode_defa, opcode_0ADB,
	opcode_defa, opcode_0ADD, opcode_0ADE, opcode_defa, opcode_defa,
	opcode_0AE1, opcode_0AE2, opcode_0AE3, opcode_defa, opcode_defa,
	opcode_defa, opcode_defa, opcode_defa, opcode_0AE9, opcode_0AEA,
	opcode_0AEB, opcode_0AEC, opcode_defa, opcode_0AEE, opcode_0AEF,
};

typedef OpcodeResult(*_OpcodeHandler)(CRunningScript *thread, unsigned short opcode);

static char *readString(CRunningScript *thread, char* buf = (char*)0, uint8_t size = 0)
{
    BYTE paramType = *thread->GetBytePointer();
    if (!paramType) return (char*)0;
    if (paramType >= 1 && paramType <= 8)
    {
        // process parameter as a pointer to string
        GetScriptParams(thread, 1);
        if (buf != (char*)0)
        {
            size = size > 128 || !size ? 128 : size;
            strncpy(buf, opcodeParams[0].pcParam, size - 1);
            buf[size - 1] = 0;
        }
        return opcodeParams[0].pcParam;
    }
    else
    {
        // process as scm string
        if (!buf)
        {
            static char result[128];
            memset(result, 0, 128);
            GetScriptStringParam(thread, result, 128);
            return result;
        }
        else
        {
            size = size > 128 || !size ? 128 : size;
            memset(buf, 0, size);
            GetScriptStringParam(thread, buf, size);
            return buf;
        }
    }
}

inline bool check_digit (char c) {
    return (c>='0') && (c<='9');
}

static int format(CRunningScript *thread, char *str, size_t len, const char *format)
{
	unsigned int written = 0;
	const char *iter = format;
	char bufa[256], fmtbufa[64], *fmta;

	while (*iter)
	{
		while (*iter && *iter != '%')
		{
			if (written++ >= len)
				return -1;
			*str++ = *iter++;
		}
		if (*iter == '%')
		{
			if (iter[1] == '%')
			{
				if (written++ >= len)
					return -1;
				*str++ = '%'; /* "%%"->'%' */
				iter += 2;
				continue;
			}
			//get flags and width specifier
			fmta = fmtbufa;
			*fmta++ = *iter++;
			while (*iter == '0' ||
				   *iter == '+' ||
				   *iter == '-' ||
				   *iter == ' ' ||
				   *iter == '*' ||
				   *iter == '#')
			{
				if (*iter == '*')
				{
					char *buffiter = bufa;
					//get width
					GetScriptParams(thread, 1);
					sprintf(buffiter, "%d", opcodeParams[0].dwParam);
					while (*buffiter)
						*fmta++ = *buffiter++;
				}
				else
					*fmta++ = *iter;
				iter++;
			}
			//get immidiate width value
			while (check_digit(*iter))
				*fmta++ = *iter++;
			//get precision
			if (*iter == '.')
			{
				*fmta++ = *iter++;
				if (*iter == '*')
				{
					char *buffiter = bufa;
					GetScriptParams(thread, 1);
					sprintf(buffiter, "%d", opcodeParams[0].dwParam);
					while (*buffiter)
						*fmta++ = *buffiter++;
				}
				else
					while (check_digit(*iter))
						*fmta++ = *iter++;
			}
			//get size
			if (*iter == 'h' || *iter == 'l')
				*fmta++ = *iter++;
			switch (*iter)
			{
			case 's':
			{
				const char *astr = readString(thread);
				const char *striter = astr ? astr : "(null)";
				while (*striter)
				{
					if (written++ >= len)
						return -1;
					*str++ = *striter++;
				}
				iter++;
				break;
			}
			case 'c':
				if (written++ >= len)
					return -1;
				GetScriptParams(thread, 1);
				*str++ = (char)opcodeParams[0].nParam;
				iter++;
				break;
			default:
			{
				/* For non wc types, use system sprintf and append to wide char output */
				/* FIXME: for unrecognised types, should ignore % when printing */
				char *bufaiter = bufa;
				if (*iter == 'p' || *iter == 'P')
				{
					GetScriptParams(thread, 1);
					sprintf(bufaiter, "%08X", opcodeParams[0].dwParam);
				}
				else
				{
					*fmta++ = *iter;
					*fmta = '\0';
					if (*iter == 'a' || *iter == 'A' ||
						*iter == 'e' || *iter == 'E' ||
						*iter == 'f' || *iter == 'F' ||
						*iter == 'g' || *iter == 'G')
					{
						GetScriptParams(thread, 1);
						sprintf(bufaiter, "%f", opcodeParams[0].fParam);
					}
					else
					{
						GetScriptParams(thread, 1);
						sprintf(bufaiter, fmtbufa, opcodeParams[0].pParam);
					}
				}
				while (*bufaiter)
				{
					if (written++ >= len)
						return -1;
					*str++ = *bufaiter++;
				}
				iter++;
				break;
			}
			}
		}
	}
	if (written >= len)
		return -1;
	*str++ = 0;
	return (int)written;
}

// opcode handler for custom opcodes
static OpcodeResult customOpcodeHandler(CRunningScript *thread, unsigned short opcode)
{
	return customOpcodeHandlers[opcode - 0x0A8C](thread);
}

/************************************************************************/
/*						Opcode definitions								*/
/************************************************************************/

OpcodeResult opcode_defa(CRunningScript *thread)
{
	return (OpcodeResult)-1;
}

//0A8C=3,write_memory %1d% size %2d% value %3d%
OpcodeResult opcode_0A8C(CRunningScript *thread)
{
	GetScriptParams(thread, 4);
	DWORD *Address = (DWORD*)opcodeParams[0].dwParam;
	DWORD size = opcodeParams[1].dwParam;
	uint32_t align_diff;
	DWORD aligned_addr;
	DWORD temp[2];
	BYTE bytes[8];

	switch(size){
	case 1:
		align_diff = (DWORD)Address % 4;

		if(align_diff > 0){
			aligned_addr = (DWORD)Address;
			aligned_addr = ((DWORD)aligned_addr - align_diff);
			temp[0] = *(DWORD*)(aligned_addr);

			bytes[0] = temp[0] & 0xFF; 
			bytes[1] = (temp[0] >> 8) & 0xFF; 
			bytes[2] = (temp[0] >> 16) & 0xFF; 
			bytes[3] = (temp[0] >> 24) & 0xFF;

			bytes[align_diff] = opcodeParams[2].ucParam;

			*(DWORD*)aligned_addr =  bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
		} else {
			*Address = opcodeParams[2].ucParam;
		}

		break;

	case 2:
		align_diff = (DWORD)Address % 4;

		if(align_diff > 0){
			aligned_addr = (DWORD)Address;
			aligned_addr = ((DWORD)aligned_addr - align_diff);
			temp[0] = *(DWORD*)(aligned_addr);
			temp[1] = *(DWORD*)(aligned_addr+4);

			bytes[0] = temp[0] & 0xFF; 
			bytes[1] = (temp[0] >> 8) & 0xFF; 
			bytes[2] = (temp[0] >> 16) & 0xFF; 
			bytes[3] = (temp[0] >> 24) & 0xFF;

			if(align_diff > 2) {
				bytes[4] = temp[1] & 0xFF; 
				bytes[5] = (temp[1] >> 8) & 0xFF; 
				bytes[6] = (temp[2] >> 16) & 0xFF; 
				bytes[7] = (temp[1] >> 24) & 0xFF;
			}

			bytes[align_diff] = opcodeParams[2].wParam & 0xFF; 
			bytes[align_diff+1] = (opcodeParams[2].wParam >> 8) & 0xFF;

			*(DWORD*)aligned_addr =  bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
			if(align_diff > 2) *(DWORD*)(aligned_addr+4) =  bytes[4] | (bytes[5] << 8) | (bytes[6] << 16) | (bytes[7] << 24);

		} else {
			*Address = opcodeParams[2].wParam;
		}
		
		break;

	case 4:
		*Address = opcodeParams[2].dwParam;
		break;
	}
	return OR_CONTINUE;
}

//0A8D=3,%3d% = read_memory %1d% size %2d%
OpcodeResult opcode_0A8D(CRunningScript *thread)
{
	DWORD *Address;
	DWORD size;

	GetScriptParams(thread, 2);
	Address = (DWORD*)opcodeParams[0].pParam;
	size = opcodeParams[1].dwParam;

	GetScriptParams(thread, 1);

	switch(size){
	default:
		opcodeParams[0].ucParam = *(BYTE*)Address;
		break;
	case 2:
		opcodeParams[0].wParam = *(WORD*)Address;
		break;
	case 4:
		opcodeParams[0].dwParam = *Address;
		break;
	}

	SetScriptParams(thread, 1);

	return OR_CONTINUE;
}

//0A8E=3,%3d% = %1d% + %2d% ; int
OpcodeResult   opcode_0A8E(CRunningScript *thread)
{
	GetScriptParams(thread, 2);
	opcodeParams[0].nParam += opcodeParams[1].nParam;
	SetScriptParams(thread, 1);	
	return OR_CONTINUE;
}

//0A8F=3,%3d% = %1d% - %2d% ; int
OpcodeResult   opcode_0A8F(CRunningScript *thread)
{
	GetScriptParams(thread, 2);
	opcodeParams[0].nParam -= opcodeParams[1].nParam;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0A90=3,%3d% = %1d% * %2d% ; int
OpcodeResult   opcode_0A90(CRunningScript *thread)
{
	GetScriptParams(thread, 2);
	opcodeParams[0].nParam *= opcodeParams[1].nParam;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0A91=3,%3d% = %1d% / %2d% ; int
OpcodeResult   opcode_0A91(CRunningScript *thread)
{
	GetScriptParams(thread, 2);
	opcodeParams[0].nParam /= opcodeParams[1].nParam;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0A96=2,%2d% = actor %1d% struct
OpcodeResult   opcode_0A96(CRunningScript *thread)
{
	DWORD handle;
	GetScriptParams(thread, 1);
	handle = opcodeParams[0].dwParam;

	opcodeParams[0].pParam = GetPedStruct(pedPool, handle);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0A97=2,%2d% = car %1d% struct
OpcodeResult   opcode_0A97(CRunningScript *thread)
{
	DWORD handle;
	GetScriptParams(thread, 1);
	handle = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = (DWORD)GetVehStruct(vehPool, handle);
	SetScriptParams(thread, 1);

	return OR_CONTINUE;
}

//0A98=2,%2d% = object %1d% struct
OpcodeResult   opcode_0A98(CRunningScript *thread)
{
	DWORD handle;
	GetScriptParams(thread, 1);
	handle = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = (DWORD)GetObjStruct(objPool, handle);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0A99=1,chdir %1b:userdir/rootdir%
OpcodeResult opcode_0A99(CRunningScript *thread)
{
	// string param
	char buf[256];
	memset(buf, '\0', sizeof(buf));
	GetScriptStringParam(thread, buf, sizeof(buf));
	setDirectory(buf);

	return OR_CONTINUE;
}


//0A9A=3,%3d% = openfile %1d% mode %2d% // IF and SET
OpcodeResult opcode_0A9A(CRunningScript *thread)
{
	const char *fname = readString(thread);
	char mode[0x10];

	// string param
	GetScriptStringParam(thread, mode, sizeof(mode));

	if (FILE* hfile = rw_open(fname, mode))
	{
		opcodeParams[0].pParam = hfile;
		SetScriptParams(thread, 1);
		SetScriptCondResult(thread, true);
	}
	else
	{
		opcodeParams[0].pParam = (DWORD)0;
		SetScriptParams(thread, 1);
		SetScriptCondResult(thread, false);
	}
	return OR_CONTINUE;
}

//0A9B=1,closefile %1d%
OpcodeResult opcode_0A9B(CRunningScript *thread)
{
	DWORD hFile;
	GetScriptParams(thread, 1);
	hFile = opcodeParams[0].dwParam;

	rw_close((FILE*)hFile);
	return OR_CONTINUE;
}

int CFileMgr_GetFileLength(FILE* a1)
{
  RwFileFunction *v1; // eax
  int v2; // edi
  RwFileFunction *v3; // eax
  RwFileFunction *v4; // eax
  int v5; // ebx
  RwFileFunction *v6; // eax

  v1 = RwOsGetFileInterface();
  v2 = v1->rwftell(a1);
  v3 = RwOsGetFileInterface();
  v3->rwfseek(a1, 0, 2);
  v4 = RwOsGetFileInterface();
  v5 = v4->rwftell(a1);
  v6 = RwOsGetFileInterface();
  v6->rwfseek(a1, v2, 0);
  return v5;
}

//0A9C=2,%2d% = file %1d% size
OpcodeResult opcode_0A9C(CRunningScript *thread)
{
	
	DWORD hFile;
	GetScriptParams(thread, 1);
	hFile = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = CFileMgr_GetFileLength((FILE*)hFile);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0A9D=3,readfile %1d% size %2d% to %3d%
OpcodeResult opcode_0A9D(CRunningScript *thread)
{
	DWORD hFile;
	DWORD size;
	void *buf;
	GetScriptParams(thread, 2);
	hFile = opcodeParams[0].dwParam;
	size = opcodeParams[1].dwParam;

	buf = GetScriptParamPointer(thread, 1);
	rw_read((FILE*)hFile, buf, size);
	return OR_CONTINUE;
}


//0A9F=1,%1d% = current_thread_pointer
OpcodeResult opcode_0A9F(CRunningScript *thread)
{
	opcodeParams[0].dwParam = (int)thread;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AA0=1,gosub_if_false %1p%
OpcodeResult opcode_0AA0(CRunningScript *thread)
{
	int off;
	GetScriptParams(thread, 1);
	off = opcodeParams[0].dwParam;

	if (thread->GetConditionResult()) return OR_CONTINUE;
	thread->PushStack(thread->GetBytePointer());
	ThreadJump(thread, off);
	return OR_CONTINUE;
}

//0AA1=0,return_if_false
OpcodeResult opcode_0AA1(CRunningScript *thread)
{
	if (thread->GetConditionResult()) return OR_CONTINUE;
	thread->SetIp(thread->PopStack());
	return OR_CONTINUE;
}

struct erl_record_t {
    uint8_t * bytes;
    uint32_t fullsize;
    char * name;
    char ** dependancies;
    uint32_t flags;

    /* Private */
    struct htab * symbols;
    struct erl_record_t * next, * prev;
}; // 32 bytes.

struct symbol_t {
    struct erl_record_t * provider;
    uint32_t address;
};

extern "C" {
	erl_record_t * load_plugin(const char * fname, char * erl_id);
	int unload_erl(erl_record_t * erl);
	symbol_t * erl_find_local_symbol(const char * symbol, erl_record_t * erl);
	symbol_t * erl_find_symbol(const char * symbol);

	void load_code_overlay(const char*, uint32_t);
	void unload_code_overlay(void);
}



//0AA2=2,%2h% = load_library %1d% // IF and SET
OpcodeResult opcode_0AA2(CRunningScript *thread)
{
	load_code_overlay("relocator.nm", 0x88d880);
	erl_record_t *libHandle = load_plugin(readString(thread), (char*)NULL);
	unload_code_overlay();

	opcodeParams[0].pParam = (void *)libHandle;
	SetScriptParams(thread, 1);

	SetScriptCondResult(thread, libHandle != NULL);
	return OR_CONTINUE;
}

//0AA3=1,free_library %1h%
OpcodeResult opcode_0AA3(CRunningScript *thread)
{
	erl_record_t *libHandle;
	GetScriptParams(thread, 1);
	libHandle = (erl_record_t *)opcodeParams[0].pParam;

	load_code_overlay("relocator.nm", 0x88d880);
	unload_erl(libHandle);
	unload_code_overlay();

	return OR_CONTINUE;
}

//0AA4=3,%3d% = get_proc_address %1d% library %2d% // IF and SET
OpcodeResult opcode_0AA4(CRunningScript *thread)
{
	char *funcName = readString(thread);
	struct erl_record_t *libHandle;
	uint32_t funcAddr = 0;

	GetScriptParams(thread, 1);
	libHandle = (erl_record_t *)opcodeParams[0].pParam;

	load_code_overlay("relocator.nm", 0x88d880);

	if (libHandle) {
		funcAddr = (uint32_t)erl_find_local_symbol(funcName, libHandle)->address;
	} else {
		funcAddr = (uint32_t)erl_find_symbol(funcName)->address;
	}

	unload_code_overlay();

	opcodeParams[0].dwParam = funcAddr;
	SetScriptParams(thread, 1);

	SetScriptCondResult(thread, funcAddr != 0);
	return OR_CONTINUE;
}

static char textParams[2][128];
static unsigned char currTextParam;
static SCRIPT_VAR arguments[15];

void process_float_arg(float val, int pos) {
	switch(pos)
	{
	case 0:
		asm volatile ("mov.s $f12, %0" : : "f" (val));
		break;
	case 1:  
		asm volatile ("mov.s $f13, %0" : : "f" (val));
		break;
	case 2:  
		asm volatile ("mov.s $f14, %0" : : "f" (val));
		break;
	case 3:  
		asm volatile ("mov.s $f15, %0" : : "f" (val));
		break;
	case 4:  
		asm volatile ("mov.s $f16, %0" : : "f" (val));
		break;
	case 5:  
		asm volatile ("mov.s $f17, %0" : : "f" (val));
		break;
	case 6:  
		asm volatile ("mov.s $f18, %0" : : "f" (val));
		break;
	case 7: 
		asm volatile ("mov.s $f19, %0" : : "f" (val));
		break;
	case 8:  
		asm volatile ("mov.s $f20, %0" : : "f" (val));
		break;
	case 9:  
		asm volatile ("mov.s $f21, %0" : : "f" (val));
		break;
	case 10: 
		asm volatile ("mov.s $f22, %0" : : "f" (val));
		break;
	case 11: 
		asm volatile ("mov.s $f23, %0" : : "f" (val));
		break;
	case 12: 
		asm volatile ("mov.s $f24, %0" : : "f" (val));
		break;
	case 13: 
		asm volatile ("mov.s $f25, %0" : : "f" (val));
		break;
	case 14: 
		asm volatile ("mov.s $f26, %0" : : "f" (val));
		break;
	}
}

//0AA5=-1,call_function %1d% num_params %2h% pop %3h%
OpcodeResult opcode_0AA5(CRunningScript *thread)
{
	void (*func)(SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR);
	DWORD numParams;
	DWORD stackAlign;

	GetScriptParams(thread, 3);

	func = (void (*)(SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR))opcodeParams[0].dwParam;

	numParams = opcodeParams[1].dwParam;
	stackAlign = opcodeParams[2].dwParam;

	if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
	stackAlign *= 4;
	SCRIPT_VAR	*arguments_end = arguments + numParams;

	// retrieve parameters
	int i = 0;
	for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
	{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
				GetScriptParams(thread, 1);
				arg->fParam = opcodeParams[0].fParam;
				process_float_arg(arg->fParam, i);
				i++;
				break;
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				GetScriptParams(thread, 1);
				arg->dwParam = opcodeParams[0].dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread, 1);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				(*arg).pcParam = readString(thread, textParams[currTextParam++], 128);
			}
	}

	// call function
	func(arguments[0], arguments[1],  arguments[2],  arguments[3], arguments[4], 
		 arguments[5], arguments[6], arguments[7], arguments[8], arguments[9], 
		 arguments[10], arguments[11], arguments[12], arguments[13], arguments[14]);

	SkipUnusedParameters(thread);

	return OR_CONTINUE;
}

//0AA6=-1,call_method %1d% struct %2d% num_params %3h% pop %4h%
OpcodeResult opcode_0AA6(CRunningScript *thread)
{

	void(*func)(void*, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
			SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
			SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR);
			
	void *struc;
	DWORD numParams;
	DWORD stackAlign;
	GetScriptParams(thread, 4);

	func = (void (*)(void*, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR))opcodeParams[0].dwParam;

	struc = opcodeParams[1].pParam;
	numParams = opcodeParams[2].dwParam;
	stackAlign = opcodeParams[3].dwParam;

	if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
	stackAlign *= 4;
	SCRIPT_VAR	*arguments_end = arguments + numParams;

	// retrieve parameters
	int i = 0;
	for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
	{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
				GetScriptParams(thread, 1);
				arg->fParam = opcodeParams[0].fParam;
				process_float_arg(arg->fParam, i);
				i++;
				break;
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				GetScriptParams(thread, 1);
				arg->dwParam = opcodeParams[0].dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread, 1);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				(*arg).pcParam = readString(thread, textParams[currTextParam++], 128);
			}
	}

	// call method
	func(struc, arguments[0], arguments[1],  arguments[2],  arguments[3], arguments[4], 
	 arguments[5], arguments[6], arguments[7], arguments[8], arguments[9], 
	 arguments[10], arguments[11], arguments[12], arguments[13], arguments[14]);

	SkipUnusedParameters(thread);

	return OR_CONTINUE;
}

//0AA7=-1,call_function_return %1d% num_params %2h% pop %3h%
OpcodeResult opcode_0AA7(CRunningScript *thread)
{
	DWORD (*func)(SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
			SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
			SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR);
	DWORD numParams;
	DWORD stackAlign;
	GetScriptParams(thread, 3);

	func = (DWORD (*)(SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR))opcodeParams[0].dwParam;

	numParams = opcodeParams[1].dwParam;
	stackAlign = opcodeParams[2].dwParam;

	if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
	stackAlign *= 4;
	SCRIPT_VAR	*arguments_end = arguments + numParams;

	// retrieve parameters
	int i = 0;
	for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
	{
			switch (*thread->GetBytePointer())
			{
			case DT_FLOAT:
				GetScriptParams(thread, 1);
				arg->fParam = opcodeParams[0].fParam;
				process_float_arg(arg->fParam, i);
				i++;
				break;
			case DT_DWORD:
			case DT_WORD:
			case DT_BYTE:
			case DT_VAR:
			case DT_LVAR:
			case DT_VAR_ARRAY:
			case DT_LVAR_ARRAY:
				GetScriptParams(thread, 1);
				arg->dwParam = opcodeParams[0].dwParam;
				break;
			case DT_VAR_STRING:
			case DT_LVAR_STRING:
			case DT_VAR_TEXTLABEL:
			case DT_LVAR_TEXTLABEL:
				arg->pParam = GetScriptParamPointer(thread, 1);
				break;
			case DT_VARLEN_STRING:
			case DT_TEXTLABEL:
				(*arg).pcParam = readString(thread, textParams[currTextParam++], 128);
			}
	}

	// call function
	DWORD result;

	result =  func(arguments[0], arguments[1],  arguments[2],  arguments[3], arguments[4], 
		 			arguments[5], arguments[6], arguments[7], arguments[8], arguments[9], 
		 			arguments[10], arguments[11], arguments[12], arguments[13], arguments[14]);

	opcodeParams[0].dwParam = result;
	SetScriptParams(thread, 1);

	SkipUnusedParameters(thread);

	return OR_CONTINUE;
}

//0AA8=-1,call_method_return %1d% struct %2d% num_params %3h% pop %4h%
OpcodeResult opcode_0AA8(CRunningScript *thread)
{
	DWORD (*func)(void*, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
			SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
			SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR);
	void *struc;
	DWORD numParams;
	DWORD stackAlign;
	
	GetScriptParams(thread, 4);

	func = (DWORD (*)(void*, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, 
				SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR, SCRIPT_VAR))opcodeParams[0].dwParam;

	struc = opcodeParams[1].pParam;
	numParams = opcodeParams[2].dwParam;
	stackAlign = opcodeParams[3].dwParam;

	if (numParams > (sizeof(arguments) / sizeof(SCRIPT_VAR))) numParams = sizeof(arguments) / sizeof(SCRIPT_VAR);
	stackAlign *= 4;
	SCRIPT_VAR	*arguments_end = arguments + numParams;

	// retrieve parameters
	int i = 0;
	for (SCRIPT_VAR *arg = arguments; arg != arguments_end; ++arg)
	{
		switch (*thread->GetBytePointer())
		{
		case DT_FLOAT:
			GetScriptParams(thread, 1);
			arg->fParam = opcodeParams[0].fParam;
			process_float_arg(arg->fParam, i);
			i++;
			break;
		case DT_DWORD:
		case DT_WORD:
		case DT_BYTE:
		case DT_VAR:
		case DT_LVAR:
		case DT_VAR_ARRAY:
		case DT_LVAR_ARRAY:
			GetScriptParams(thread, 1);
			arg->dwParam = opcodeParams[0].dwParam;
			break;
		case DT_VAR_STRING:
		case DT_LVAR_STRING:
		case DT_VAR_TEXTLABEL:
		case DT_LVAR_TEXTLABEL:
			arg->pParam = GetScriptParamPointer(thread, 1);
			break;
		case DT_VARLEN_STRING:
		case DT_TEXTLABEL:
			(*arg).pcParam = readString(thread, textParams[currTextParam++], 128);
		}
	}

	DWORD result;

	result = func(struc, arguments[0], arguments[1],  arguments[2],  arguments[3], arguments[4], 
		 			arguments[5], arguments[6], arguments[7], arguments[8], arguments[9], 
		 			arguments[10], arguments[11], arguments[12], arguments[13], arguments[14]);

	opcodeParams[0].dwParam = result;
	SetScriptParams(thread, 1);

	SkipUnusedParameters(thread);

	return OR_CONTINUE;
}

//0AA9=0,  is_game_version_original
OpcodeResult opcode_0AA9(CRunningScript *thread)
{
	SetScriptCondResult(thread, true);
	return OR_CONTINUE;
}

static SCRIPT_VAR CleoVariables[0x400] = {};

//0AB1=-1,call_scm_func %1p%
OpcodeResult opcode_0AB1(CRunningScript *thread)
{
	int		label;
	DWORD	nParams;


	GetScriptParams(thread, 2);
	label = opcodeParams[0].dwParam;
	nParams = opcodeParams[1].dwParam;

	ScmFunction* scmFunc = new ScmFunction(thread);
	
	static SCRIPT_VAR arguments[32];
	SCRIPT_VAR* locals = thread->IsMission() ? missionLocals : thread->GetVarPtr();
	SCRIPT_VAR* localsEnd = locals + 32;
	SCRIPT_VAR* storedLocals = scmFunc->savedTls;

	scmFunc->lastStringIndex = 0;

	// collect arguments
	DWORD paramSize = (nParams < 32) ? nParams : 32;
	for (DWORD i = 0; i < paramSize; i++)
	{
		SCRIPT_VAR* arg = arguments + i;
			
		switch (*thread->GetBytePointer())
		{
		case DT_FLOAT:
		case DT_DWORD:
		case DT_WORD:
		case DT_BYTE:
		case DT_VAR:
		case DT_LVAR:
		case DT_VAR_ARRAY:
		case DT_LVAR_ARRAY:
			GetScriptParams(thread, 1);
			arg->dwParam = opcodeParams[0].dwParam;
			break;

		case DT_VAR_STRING:
		case DT_LVAR_STRING:
		case DT_VAR_TEXTLABEL:
		case DT_LVAR_TEXTLABEL:
			arg->pParam = GetScriptParamPointer(thread, 1);
			if (arg->pParam >= locals && arg->pParam < localsEnd) // correct scoped variable's pointer
			{
				arg->dwParam -= (DWORD)locals;
				arg->dwParam += (DWORD)storedLocals;
			}
			break;

		case DT_STRING:
		case DT_TEXTLABEL:
		case DT_VARLEN_STRING:
			const char* string = readString(thread);
			strcpy(scmFunc->stringParams[scmFunc->lastStringIndex], string);
			arg->pcParam = (char*)scmFunc->stringParams[scmFunc->lastStringIndex++];
			break;
		}
	}

	// skip unused args
	if (nParams > 32) 
		GetScriptParams(thread, nParams - 32);

	// all areguments read
	scmFunc->retnAddress = thread->GetBytePointer();

	// pass arguments as new scope local variables
	memcpy(locals, arguments, nParams * sizeof(SCRIPT_VAR));

	// initialize rest of new scope local variables
	CRunningScript* cs = reinterpret_cast<CRunningScript*>(thread);

	for (DWORD i = nParams; i < 32; i++)
	{
		cs->SetIntVar(i, 0); // fill with zeros
	}

	// jump to label
	ThreadJump(thread, label);

	
	return OR_CONTINUE;
}

//0AB2=-1,ret
OpcodeResult opcode_0AB2(CRunningScript *thread)
{
	ScmFunction *scmFunc = ScmFunction::Store[reinterpret_cast<CRunningScript*>(thread)->GetScmFunction()];
	DWORD nRetParams;
	GetScriptParams(thread, 1);
	nRetParams = opcodeParams[0].dwParam;
	if (nRetParams) GetScriptParams(thread, nRetParams);
	scmFunc->Return(thread);
	if (nRetParams) SetScriptParams(thread, nRetParams);
	SkipUnusedParameters(thread);
	delete scmFunc;
	return OR_CONTINUE;
}

//0AB3=2,var %1d% = %2d%
OpcodeResult opcode_0AB3(CRunningScript *thread)
{
	DWORD varId, value;
	GetScriptParams(thread, 2);
	varId = opcodeParams[0].dwParam;
	value = opcodeParams[1].dwParam;
	CleoVariables[varId].dwParam = value;
	return OR_CONTINUE;
}

//0AB4=2,%2d% = var %1d%
OpcodeResult opcode_0AB4(CRunningScript *thread)
{
	DWORD varId;
	GetScriptParams(thread, 1);
	varId = opcodeParams[0].dwParam;
	opcodeParams[0].dwParam = CleoVariables[varId].dwParam;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AB6=3,store_target_marker_coords_to %1d% %2d% %3d% // IF and SET
OpcodeResult opcode_0AB6(CRunningScript *thread)
{
	DWORD hMarker = FrontEndMenuManager_m_nTargetBlipi;
	tRadarTrace *pMarker;
	if (hMarker && (pMarker = &CRadar_ms_RadarTrace[LOWORD(hMarker)]) && /*pMarker->m_nPooli == HIWORD(hMarker) && */pMarker->m_nBlipDisplay)
	{
		opcodeParams[0].fParam = pMarker->m_vecPos.x;
		opcodeParams[1].fParam = pMarker->m_vecPos.y;
		opcodeParams[2].fParam = CWorld_FindGroundZForCoord(pMarker->m_vecPos.x, pMarker->m_vecPos.y);
		SetScriptParams(thread, 3);

		SetScriptCondResult(thread, true);
	} else {
		GetScriptParams(thread, 3);
		SetScriptCondResult(thread, false);
	}
	return OR_CONTINUE;
}

//0AB8=2,get_vehicle %1d% current_gear_to %2d%
OpcodeResult opcode_0AB8(CRunningScript *thread)
{
	GetScriptParams(thread, 1);
	DWORD *vehicle = GetVehStruct(vehPool, opcodeParams[0].dwParam);

	opcodeParams[0].dwParam = *(uint8_t*)((uint32_t)vehicle + 0x4BC); // CurrentGear
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0ABE=1,  vehicle %1d% engine_on
OpcodeResult opcode_0ABE(CRunningScript *thread)
{
	DWORD hVehicle;
	GetScriptParams(thread, 1);

	SetScriptCondResult(thread, getVehicleFlags(GetVehStruct(vehPool, opcodeParams[0].dwParam))->bEngineOn);
	return OR_CONTINUE;
}

//0AC6=2,%2d% = label %1p% offset
OpcodeResult opcode_0AC6(CRunningScript *thread)
{
	int label;
	GetScriptParams(thread, 1);
	label = opcodeParams[0].dwParam;

	opcodeParams[0].pParam = (label < 0 ? thread->GetBasePointer() - label : (BYTE*)CTheScripts_ScriptSpace + label);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AC7=2,%2d% = var %1d% offset
OpcodeResult opcode_0AC7(CRunningScript *thread)
{
	opcodeParams[0].pParam = GetScriptParamPointer(thread, 1);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AC8=2,%2d% = allocate_memory_size %1d%
OpcodeResult opcode_0AC8(CRunningScript *thread)
{
	DWORD size;
	GetScriptParams(thread, 1);
	size = opcodeParams[0].dwParam;

	void *mem = malloc(size);

	opcodeParams[0].pParam = mem;
	SetScriptParams(thread, 1);
	SetScriptCondResult(thread, mem != 0);
	return OR_CONTINUE;
}

//0AC9=1,free_allocated_memory %1d%
OpcodeResult opcode_0AC9(CRunningScript *thread)
{
	void *mem;
	GetScriptParams(thread, 1);
	mem = opcodeParams[0].pParam;

	free(mem);

	return OR_CONTINUE;
}

//0ACA=1,show_text_box %1s%
OpcodeResult opcode_0ACA(CRunningScript *thread)
{	
	PrintHelp(readString(thread), 0, 0, 0);
	return OR_CONTINUE;
}

//0ACB=3,show_styled_text %1d% time %2d% style %3d%
OpcodeResult opcode_0ACB(CRunningScript *thread)
{
	const char *text = readString(thread);
	DWORD time, style;
	GetScriptParams(thread, 2);
	time = opcodeParams[0].dwParam;
	style = opcodeParams[1].dwParam;
	
	PrintBig(text, time, style-1);
	return OR_CONTINUE;
}

//0ACC=2,show_text_lowpriority %1d% time %2d%
OpcodeResult opcode_0ACC(CRunningScript *thread)
{
	const char *text = readString(thread);
	DWORD time;
	GetScriptParams(thread, 1);
	time = opcodeParams[0].dwParam;

	Print(text, time, false, false);
	return OR_CONTINUE;
}

//0ACD=2,show_text_highpriority %1d% time %2d%
OpcodeResult opcode_0ACD(CRunningScript *thread)
{
	const char *text = readString(thread);
	DWORD time;
	GetScriptParams(thread, 1);
	time = opcodeParams[0].dwParam;

	PrintNow(text, time, false, false);
	return OR_CONTINUE;
}

static char print_help_buffer[128];
static char print_big_buffer[128];
static char print_buffer[128];
static char print_now_buffer[128];

//0ACE=-1,show_formatted_text_box %1d%
OpcodeResult opcode_0ACE(CRunningScript *thread)
{
	char fmt[128]; char text[128];

	readString(thread, fmt, sizeof(fmt));
	format(thread, text, sizeof(text), fmt);

	strcpy(print_help_buffer, text);

	PrintHelp(print_help_buffer, 0, 0, 0);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}


//0ACF=-1,show_formatted_styled_text %1d% time %2d% style %3d%
OpcodeResult opcode_0ACF(CRunningScript *thread)
{
	char fmt[128]; char text[128];

	DWORD time, style;
	readString(thread, fmt, sizeof(fmt));
	time = opcodeParams[0].dwParam;
	style = opcodeParams[1].dwParam;
	format(thread, text, sizeof(text), fmt);

	strcpy(print_big_buffer, text);

	PrintBig(print_big_buffer, time, style-1);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

//0AD0=-1,show_formatted_text_lowpriority %1d% time %2d%
OpcodeResult opcode_0AD0(CRunningScript *thread)
{
	char fmt[128]; char text[128];
	DWORD time;
	readString(thread, fmt, sizeof(fmt));
	GetScriptParams(thread, 1);
	time = opcodeParams[0].dwParam;
	format(thread, text, sizeof(text), fmt);

	strcpy(print_buffer, text);

	Print(print_buffer, time, false, false);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

//0AD1=-1,show_formatted_text_highpriority %1d% time %2d%
OpcodeResult opcode_0AD1(CRunningScript *thread)
{
	char fmt[128]; char text[128];
	DWORD time;
	readString(thread, fmt, sizeof(fmt));
	GetScriptParams(thread, 1);
	time = opcodeParams[0].dwParam;

	format(thread, text, sizeof(text), fmt);

	strcpy(print_now_buffer, text);

	PrintNow(print_now_buffer, time, false, false);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

//0AD3=-1,%1d% = string_format %2s%
OpcodeResult opcode_0AD3(CRunningScript *thread)
{
	char fmt[128], *dst;

	if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8) {
		GetScriptParams(thread, 1);
		dst = opcodeParams[0].pcParam;
	}
	else {
		dst = &GetScriptParamPointer(thread, 1)->cParam;
	}

	readString(thread, fmt, sizeof(fmt));
	format(thread, dst, -1, fmt);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

//0AD4=-1,%3d% = scan_string %1d% format %2d%  //IF and SET
OpcodeResult opcode_0AD4(CRunningScript *thread)
{
	char fmt[128], *format, *src;
	src = readString(thread);
	format = readString(thread, fmt, sizeof(fmt));
	size_t cExParams = 0;
	int *result = (int *)GetScriptParamPointer(thread, 1);
	SCRIPT_VAR *ExParams[35];
	// read extra params
	for (int i = 0; i < 35; i++)
	{
		if (*thread->GetBytePointer())
		{
			ExParams[i] = GetScriptParamPointer(thread, 1);
			cExParams++;
		}
		else ExParams[i]->pParam = NULL;
	}

	thread->IncPtr();

	*result = sscanf(src, format,
					 /* extra parameters (will be aligned automatically, but the limit of 35 elements maximum exists) */
					 ExParams[0], ExParams[1], ExParams[2], ExParams[3], ExParams[4], ExParams[5],
					 ExParams[6], ExParams[7], ExParams[8], ExParams[9], ExParams[10], ExParams[11],
					 ExParams[12], ExParams[13], ExParams[14], ExParams[15], ExParams[16], ExParams[17],
					 ExParams[18], ExParams[19], ExParams[20], ExParams[21], ExParams[22], ExParams[23],
					 ExParams[24], ExParams[25], ExParams[26], ExParams[27], ExParams[28], ExParams[29],
					 ExParams[30], ExParams[31], ExParams[32], ExParams[33], ExParams[34]);

	SetScriptCondResult(thread, cExParams == *result);
	return OR_CONTINUE;
}

//0AD5=3,file %1d% seek %2d% from_origin %3d% //IF and SET
OpcodeResult opcode_0AD5(CRunningScript *thread)
{
	DWORD hFile;
	int seek, origin;

	GetScriptParams(thread, 3);
	hFile = opcodeParams[0].dwParam;
	seek = opcodeParams[1].dwParam;
	origin = opcodeParams[2].dwParam;

	SetScriptCondResult(thread, rw_seek((FILE*)hFile, seek, origin) == 0);
	return OR_CONTINUE;
}

//0ADB=2,%2d% = car_model %1o% name
OpcodeResult opcode_0ADB(CRunningScript *thread)
{
	DWORD mi;
	char *buf;
	GetScriptParams(thread, 1);
	mi = opcodeParams[0].dwParam;
	DWORD model;
	model = (DWORD)CModelInfo_ms_modelInfoPtrs[mi];
	if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8) {
		GetScriptParams(thread, 1);
		buf = opcodeParams[0].pcParam;
	} else {
		buf = (char *)GetScriptParamPointer(thread, 1);
	}
	memcpy(buf, (char*)(model + 0x32), 8);
	return OR_CONTINUE;
}

void (*CCheat_VehicleCheat)(int) = (void (*)(int))0x59E480;

//0ADD=1,spawn_car_with_model %1o% at_player_location 
OpcodeResult opcode_0ADD(CRunningScript *thread)
{
	DWORD mi;
	GetScriptParams(thread, 1);
	mi = opcodeParams[0].dwParam;

	DWORD model;

	model = CModelInfo_ms_modelInfoPtrs[mi];

	if (*(DWORD*)(model + 0x3C) != VEHICLE_TYPE_TRAIN && *(DWORD*)(model + 0x3C) != VEHICLE_TYPE_UNKNOWN) CCheat_VehicleCheat(mi); // m_nVehicleType
	return OR_CONTINUE;
}

//0ADE=2,%2d% = text_by_GXT_entry %1d%
OpcodeResult opcode_0ADE(CRunningScript *thread)
{
	const char *gxt = readString(thread);
	if (*thread->GetBytePointer() >= 1 && *thread->GetBytePointer() <= 8) {
		opcodeParams[0].pcParam = (char*)CText_Get(gameTexts, gxt);
		SetScriptParams(thread, 1);
	} else {
		strcpy((char *)GetScriptParamPointer(thread, 1), CText_Get(gameTexts, gxt));
	}
	return OR_CONTINUE;
}

float DistanceBetweenPointsSqr(CVector* v1, CVector* v2) {
    return (v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y);
}

CVector* getCharCoordinates(uint32_t* ped) {
    uint32_t v58;
    uint32_t v57;
    uint32_t v60;
    CVector* ret;

    if ( (*(uint8_t *)((uint32_t)ped + 1189) & 1) != 0 )
        v57 = *(uint32_t *)((uint32_t)ped + 1484);
    else
        v57 = 0;

    if ( v57 )
    {
        v58 = *(uint32_t *)(v57 + 20);
        if ( v58 )
            ret = (CVector *)(v58 + 48);
        else
            ret = (CVector *)(v57 + 4);
    }
    else
    {
        v60 = *(uint32_t *)((uint32_t)ped + 20);
        if ( v60 )
            ret = (CVector *)(v60 + 48);
        else
            ret = (CVector *)((uint32_t)ped + 4);
    }

    return ret;
}

CVector* getVehCoordinates(uint32_t* veh) {
    CVector* ret;
    uint32_t v90;

    v90 = *(uint32_t *)((uint32_t)veh + 20);
    if ( v90 )
        ret = (CVector *)((uint32_t)v90 + 48);
    else
        ret = (CVector *)((uint32_t)veh + 4);
    
    return ret;
} 

//0AE1=7,%7d% = find_actor_near_point %1d% %2d% %3d% in_radius %4d% find_next %5h% pass_deads %6h% //IF and SET
OpcodeResult opcode_0AE1(CRunningScript *thread)
{
	CVector center, *ped_coords;
	float radius;
	DWORD next, pass_deads;
	static DWORD stat_last_found = 0;
	CPool* pool = *(CPool**)0x0066B918; // pedPool

	GetScriptParams(thread, 6);

	center.x = opcodeParams[0].fParam;
	center.y = opcodeParams[1].fParam;
	center.z = opcodeParams[2].fParam;

	radius = opcodeParams[3].fParam;

	next = opcodeParams[4].dwParam;
	pass_deads = opcodeParams[5].dwParam;

	DWORD last_found = stat_last_found;

	if (!next) last_found = 0;

	for (int i = last_found; i < pool->size; i++)
	{
		void* obj = (void*)0;

		if (!pool->m_byteMap[i].bEmpty)
			obj = (void*)((DWORD)(pool->objects) + (i*sizeof_CPed));

		if (obj)
		{
			if (pass_deads != -1 && (IsPlayer(obj) || (pass_deads && *(float *)((uint32_t)obj + 1408) <= 0.0f)))
				continue;

			ped_coords = getCharCoordinates((uint32_t*)obj);

			if (radius >= 1000.0f || (DistanceBetweenPointsSqr(ped_coords, &center) <= (radius * radius)))
			{
				last_found = i + 1;	// on next opcode call start search from next i
										//if(last_found >= (unsigned)pool->GetSize()) last_found = 0;
										//obj->PedCreatedBy = 2; // add reference to found actor

				opcodeParams[0].dwParam = GetPedHandle(pedPool, (int)obj);
				SetScriptParams(thread, 1);
				SetScriptCondResult(thread, true);
				return OR_CONTINUE;
			}
		}
	}

	opcodeParams[0].dwParam = -1;
	SetScriptParams(thread, 1);
	last_found = 0;
	SetScriptCondResult(thread, false);
	return OR_CONTINUE;
}

//0AE2=7,%7d% = find_vehicle_near_point %1d% %2d% %3d% in_radius %4d% find_next %5h% pass_wrecked %6h% //IF and SET
OpcodeResult opcode_0AE2(CRunningScript *thread)
{
	CVector center, *veh_coords;
	float radius;
	DWORD next, pass_wrecked;
	static DWORD stat_last_found = 0;
	CPool* pool = *(CPool**)0x0066B91C; // vehPool

	GetScriptParams(thread, 6);

	center.x = opcodeParams[0].fParam;
	center.y = opcodeParams[1].fParam;
	center.z = opcodeParams[2].fParam;

	radius = opcodeParams[3].fParam;

	next = opcodeParams[4].dwParam;
	pass_wrecked = opcodeParams[5].dwParam;

	DWORD last_found = stat_last_found;

	if (!next) last_found = 0;

	for (int i = last_found; i < pool->size; i++)
	{
		void* obj = (void*)0;

		if (!pool->m_byteMap[i].bEmpty)
			obj = (void*)((DWORD)(pool->objects) + (i*sizeof_CVehicle));

		if (obj)
		{
			if (pass_wrecked != -1 && (getVehicleFlags(obj)->bFadeOut || (pass_wrecked && (getVehicleFlags(obj)->bEngineBroken || getVehicleFlags(obj)->bIsDrowning))))
				continue;

			veh_coords = getVehCoordinates((uint32_t*)obj);

			if (radius >= 1000.0f || (DistanceBetweenPointsSqr(veh_coords, &center) <= (radius * radius)))
			{
				last_found = i + 1;	// on next opcode call start search from next i
										//if(last_found >= (unsigned)pool->GetSize()) last_found = 0;
										//obj->PedCreatedBy = 2; // add reference to found actor

				opcodeParams[0].dwParam = GetVehHandle(vehPool, (int)obj);
				SetScriptParams(thread, 1);
				stat_last_found = last_found;
				SetScriptCondResult(thread, true);
				return OR_CONTINUE;
			}
		}
	}

	opcodeParams[0].dwParam = -1;
	SetScriptParams(thread, 1);
	last_found = 0;
	stat_last_found = last_found;
	SetScriptCondResult(thread, false);
	return OR_CONTINUE;
}

//0AE3=6,%6d% = find_object_near_point %1d% %2d% %3d% in_radius %4d% find_next %5h% //IF and SET
OpcodeResult opcode_0AE3(CRunningScript *thread)
{
	CVector center, *obj_coords;
	float radius;
	DWORD next;
	static DWORD stat_last_found = 0;
	CPool* pool = *(CPool**)0x0066B924; // vehPool

	GetScriptParams(thread, 5);

	center.x = opcodeParams[0].fParam;
	center.y = opcodeParams[1].fParam;
	center.z = opcodeParams[2].fParam;

	radius = opcodeParams[3].fParam;

	next = opcodeParams[4].dwParam;

	DWORD last_found = stat_last_found;

	if (!next) last_found = 0;

	for (int i = last_found; i < pool->size; i++)
	{
		void* obj = (void*)0;

		if (!pool->m_byteMap[i].bEmpty)
			obj = (void*)((DWORD)(pool->objects) + (i*sizeof_CObject));

		if (obj)
		{
			obj_coords = getVehCoordinates((uint32_t*)obj);

			if (radius >= 1000.0f || (DistanceBetweenPointsSqr(obj_coords, &center) <= (radius * radius)))
			{
				last_found = i + 1;	// on next opcode call start search from next i
										//if(last_found >= (unsigned)pool->GetSize()) last_found = 0;
										//obj->PedCreatedBy = 2; // add reference to found actor

				opcodeParams[0].dwParam = GetObjHandle(objPool, (int)obj);
				SetScriptParams(thread, 1);
				SetScriptCondResult(thread, true);
				return OR_CONTINUE;
			}
		}
	}

	opcodeParams[0].dwParam = -1;
	SetScriptParams(thread, 1);
	last_found = 0;
	SetScriptCondResult(thread, false);
	return OR_CONTINUE;
}

//0AE9=0,pop_float
OpcodeResult opcode_0AE9(CRunningScript *thread)
{
	float result;
	asm volatile( "mov.s %0, $f0" : "=f" (result) );
	opcodeParams[0].fParam = result;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}


//0AEA=2,%2d% = actor_struct %1d% handle
OpcodeResult opcode_0AEA(CRunningScript *thread)
{
	DWORD struc;
	GetScriptParams(thread, 1);
	struc = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = GetPedHandle(pedPool, struc);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AEB=2,%2d% = car_struct %1d% handle
OpcodeResult opcode_0AEB(CRunningScript *thread)
{
	DWORD struc;
	GetScriptParams(thread, 1);
	struc = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = GetVehHandle(vehPool, struc);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AEC=2,%2d% = object_struct %1d% handle
OpcodeResult opcode_0AEC(CRunningScript *thread)
{
	DWORD struc;
	GetScriptParams(thread, 1);
	struc = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = GetObjHandle(objPool, struc);
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

//0AEE=3,%3d% = %1d% exp %2d% //all floats
OpcodeResult opcode_0AEE(CRunningScript *thread)
{
	float base, arg;
	GetScriptParams(thread, 2);
	base = opcodeParams[0].fParam;
	arg = opcodeParams[1].fParam;

	opcodeParams[0].fParam = powf(base, arg);
	SetScriptParams(thread, 1);

	return OR_CONTINUE;
}

//0AEF=3,%3d% = log %1d% base %2d% //all floats
OpcodeResult opcode_0AEF(CRunningScript *thread)
{
	float base, arg;
	GetScriptParams(thread, 2);
	arg = opcodeParams[0].fParam;
	base = opcodeParams[1].fParam;

	opcodeParams[0].fParam = (logf(arg) / logf(base));
	SetScriptParams(thread, 1);

	return OR_CONTINUE;
}

/*
struct CleoSafeHeader
{
    const static unsigned sign;
    unsigned signature;
    unsigned n_saved_threads;
    unsigned n_stopped_threads;
};

void AddScriptToQueue(CRunningScript *cs, CRunningScript **queue)
{
  cs->Next = *queue;
  cs->Previous = 0;
  if ( *queue )
    (*queue)->Previous = cs;
  *queue = cs;
}

CRunningScript **activeThreadQueue = (CRunningScript**)0x0066B508;

void CCustomScripts_Init(){
	struct sce_dirent dirbuf;
    char* section_names = NULL;
    const char* plugin_dir = "mass:PS2_SA/cleo/";
    char* dest_file = NULL;

    int dirfd = sceDopen(plugin_dir);

    sceDread(dirfd, &dirbuf);
    sceDread(dirfd, &dirbuf);

    while (sceDread(dirfd, &dirbuf) > 0){
        dest_file = (char*)malloc(strlen(plugin_dir) + strlen(dirbuf.d_name) + 1);

        strcpy(dest_file, plugin_dir);
        strcat(dest_file, dirbuf.d_name);
 
        int file = open(dest_file, 1);

    	size_t file_size = lseek(file, 0, SEEK_END);
    	lseek(file, 0, SEEK_SET);

    	BYTE* file_data = (BYTE*)malloc(file_size);

    	read(file, file_data, file_size);

		close(file);

		CRunningScript* cs = static_cast<CRunningScript*>(malloc(sizeof(CRunningScript)));
		*cs = CRunningScript();

		cs->BaseIP = cs->CurrentIP = file_data;

		memcpy(cs->Name, dirbuf.d_name, sizeof(cs->Name));
		cs->Name[7] = '\0';

        free(dest_file);

	    AddScriptToQueue(cs, activeThreadQueue);
        cs->SetActive(true);

    }

}

*/


/*
============================================================

					 CLEOPlus opcodes

============================================================
*/

static OpcodeResult SET_CAR_DOOR_WINDOW_STATE(CRunningScript* thread);
static OpcodeResult GET_CAR_DUMMY_COORD(CRunningScript* thread);
static OpcodeResult SET_CAR_ALARM(CRunningScript* thread);
static OpcodeResult GET_TIME_CHAR_IS_DEAD(CRunningScript* thread);
static OpcodeResult ADD_FX_SYSTEM_PARTICLE(CRunningScript* thread);
static OpcodeResult GET_CAR_PEDALS(CRunningScript* thread);
static OpcodeResult SET_CHAR_ARRESTED(CRunningScript* thread);
static OpcodeResult COPY_MEMORY(CRunningScript* thread);
static OpcodeResult WRITE_STRUCT_PARAM(CRunningScript* thread);
static OpcodeResult READ_STRUCT_PARAM(CRunningScript* thread);
static OpcodeResult READ_STRUCT_OFFSET(CRunningScript* thread);
static OpcodeResult WRITE_STRUCT_OFFSET(CRunningScript* thread);
static OpcodeResult READ_STRUCT_OFFSET_MULTI(CRunningScript* thread);
static OpcodeResult WRITE_STRUCT_OFFSET_MULTI(CRunningScript* thread);
static OpcodeResult GET_TIME_NOT_TOUCHING_PAD(CRunningScript* thread);
static OpcodeResult GET_CHAR_SIMPLEST_ACTIVE_TASK(CRunningScript* thread);

static OpcodeResult cleoPlusOpcodeHandler(CRunningScript *thread, unsigned short opcode)
{
	switch (opcode) {
		case 0xD27:
			return COPY_MEMORY(thread);
		case 0xD33:
			return SET_CAR_DOOR_WINDOW_STATE(thread);
		case 0xD37:
			return WRITE_STRUCT_PARAM(thread);
		case 0xD38:
			return READ_STRUCT_PARAM(thread);
		case 0xD4E:
			return READ_STRUCT_OFFSET(thread);
		case 0xE28:
			return WRITE_STRUCT_OFFSET(thread);
		case 0xE5B:
			return GET_CAR_DUMMY_COORD(thread);
		case 0xE61:
			return SET_CAR_ALARM(thread);
		case 0xEAA:
			return SET_CHAR_ARRESTED(thread);
		case 0xEC0:
			return ADD_FX_SYSTEM_PARTICLE(thread);
		case 0xECE:
			return GET_TIME_CHAR_IS_DEAD(thread);
		case 0xEE2:
			return READ_STRUCT_OFFSET_MULTI(thread);
		case 0xEE3:
			return WRITE_STRUCT_OFFSET_MULTI(thread);
		case 0xEFD:
			return GET_CAR_PEDALS(thread);
		case 0xEFF:
			return GET_CHAR_SIMPLEST_ACTIVE_TASK(thread);
		case 0xF13:
			return GET_TIME_NOT_TOUCHING_PAD(thread);
		default:
			return opcode_defa(thread);
	}
}

extern "C" {
	void* CTaskManager_GetSimplestActiveTask(DWORD);
}

uint32_t getVirtualMethod(void* obj, int i) {
	return (*(uint32_t**)obj)[i+2];
}

int CTask_GetId(void* task) {
	return ((int (*)(void *))getVirtualMethod(task, 4))(task);
}

OpcodeResult GET_CHAR_SIMPLEST_ACTIVE_TASK(CRunningScript* thread)
{
	DWORD pPed;
	GetScriptParams(thread, 1);
	pPed = (DWORD)GetPedStruct(pedPool, opcodeParams[0].dwParam);

	void *task = CTaskManager_GetSimplestActiveTask(*(DWORD *)(pPed + 1204) + 4);

	if (task) {
		 opcodeParams[0].dwParam = CTask_GetId(task);
		 opcodeParams[1].dwParam = (DWORD)task;

		SetScriptCondResult(thread, true);
	} else {
		 opcodeParams[0].dwParam = -1;
		 opcodeParams[1].dwParam = -1;

		SetScriptCondResult(thread, false);
	}

	SetScriptParams(thread, 2);

	return OR_CONTINUE;
}

OpcodeResult SET_CAR_DOOR_WINDOW_STATE(CRunningScript* thread)
{
	GetScriptParams(thread, 3);
	DWORD *vehicle = GetVehStruct(vehPool, opcodeParams[0].dwParam);
	unsigned int door = opcodeParams[1].dwParam;
	unsigned int state = opcodeParams[2].dwParam;

	if (state) CVehicle_SetWindowOpenFlag(vehicle, door); else CVehicle_ClearWindowOpenFlag(vehicle, door);
	return OR_CONTINUE;
}

CVector TransformFromObjectSpace(uint32_t entity, CVector offset) {
    CVector ret;

    if ( *(uint32_t *)(entity + 20) ) {
        CMatrix_Attach(&ret, *(uint32_t *)(entity + 20), &offset);
    } else {
        sub_25F980(&ret, (CVector *)(entity + 4), &offset);
    }

    return ret;
}

inline void getVehicleDummyPos(CVector* ret, uint16_t veh_id, uint16_t dummy_id) {
	CVector* base = &(*(CVector**)(CModelInfo_ms_modelInfoPtrs[veh_id] + 0x5C))[dummy_id];
	ret->x = base->x;
	ret->y = base->y;
	ret->z = base->z;
}

OpcodeResult GET_CAR_DUMMY_COORD(CRunningScript* thread)
{
	bool bResult = false;
	GetScriptParams(thread, 4);
	CEntity *vehicle = (CEntity*)GetVehStruct(vehPool, opcodeParams[0].dwParam);
	int dummyId = opcodeParams[1].dwParam;
	int worldCoords = opcodeParams[2].dwParam;
	int invertX = opcodeParams[3].dwParam;
	
	CVector posn;
	posn.x = 0.0f;
	posn.y = 0.0f;
	posn.z = 0.0f;

	if (CModelInfo_ms_modelInfoPtrs[vehicle->m_nModelIndex]) {
		getVehicleDummyPos(&posn, vehicle->m_nModelIndex, dummyId);
	}

	if (posn.x != 0.0f && posn.y != 0.0f && posn.z != 0.0f)
		bResult = true;

	if (invertX)
		posn.x *= -1.0f;

	if (worldCoords)
		posn = TransformFromObjectSpace((uint32_t)vehicle, posn);

	opcodeParams[0].fParam = posn.x;
	opcodeParams[1].fParam = posn.y;
	opcodeParams[2].fParam = posn.z;
	SetScriptParams(thread, 3);
	SetScriptCondResult(thread, bResult);
	return OR_CONTINUE;
}

OpcodeResult SET_CAR_ALARM(CRunningScript* thread)
{
	GetScriptParams(thread, 2);
	DWORD *vehicle = GetVehStruct(vehPool, opcodeParams[0].dwParam);
	int mode = opcodeParams[1].dwParam;

	if (mode == 0) {
		*(unsigned short*)((uint32_t)vehicle + 0x464) = 0;
	}
	else {
		if (mode == 1) {
			*(unsigned short*)((uint32_t)vehicle + 0x464) = 65535;
		}
		else {
			if (mode == 2) {
				*(unsigned short*)((uint32_t)vehicle + 0x464) = 65534;
			}
		}
	}
	return OR_CONTINUE;
}

OpcodeResult GET_CAR_PEDALS(CRunningScript* thread)
{
	GetScriptParams(thread, 1);
	DWORD *vehicle = GetVehStruct(vehPool, opcodeParams[0].dwParam);

	opcodeParams[0].fParam = *(float*)((uint32_t)vehicle + 0x4A4); // GasPedal
	opcodeParams[1].fParam = *(float*)((uint32_t)vehicle + 0x4A8); // BreakPedal
	SetScriptParams(thread, 2);
	return OR_CONTINUE;
}

struct tScriptEffectSystem
{
    char bUsed;
    short wUniqueID;
    void *m_pFxSystem;
};

tScriptEffectSystem *ScriptEffectSystemArray = (tScriptEffectSystem *)0x6FD6D0;

void* (*new_FxPrtMult_c)(float red, float green, float blue, float alpha, float size, float arg5, float lastFactor) = (void* (*)(float red, float green, float blue, float alpha, float size, float arg5, float lastFactor))0x3D6C70;
void (*FxSystem_c_AddParticle)(void* fxSystem, CVector* position, CVector* velocity, float arg2, void* prtMult, float arg4, float brightness, float arg6, unsigned char arg7) = (void (*)(void* fxSystem, CVector* position, CVector* velocity, float arg2, void* prtMult, float arg4, float brightness, float arg6, unsigned char arg7))0x3D5900;

OpcodeResult ADD_FX_SYSTEM_PARTICLE(CRunningScript* thread)
{
	GetScriptParams(thread, 14);

	int handle = opcodeParams[0].dwParam;

	CVector pos;
	pos.x = opcodeParams[1].fParam;
	pos.y = opcodeParams[2].fParam;
	pos.z = opcodeParams[3].fParam;

	CVector vel;
	vel.x = opcodeParams[4].fParam;
	vel.y = opcodeParams[5].fParam;
	vel.z = opcodeParams[6].fParam;

	float size = opcodeParams[7].fParam;

	float brightness = opcodeParams[8].fParam;

	float r = opcodeParams[9].fParam;
	float g = opcodeParams[10].fParam;
	float b = opcodeParams[11].fParam;
	float a = opcodeParams[12].fParam;

	float lastFactor = opcodeParams[13].fParam;

	if (handle >= 0) {
		handle = CTheScripts_GetActualScriptThingIndex(handle, 1);
		if (handle >= 0) {
			void *fx = (void *)ScriptEffectSystemArray[handle].m_pFxSystem;

			void *prtMult = new_FxPrtMult_c(r, g, b, a, size, 0.0, lastFactor);

			FxSystem_c_AddParticle(fx, &pos, &vel, 0.0f, prtMult, -1.0f, brightness, 0.0f, (unsigned char)0);
		}
	}
	return OR_CONTINUE;
}


enum ePedState
{
	PEDSTATE_NONE,
	PEDSTATE_IDLE,
	PEDSTATE_LOOK_ENTITY,
	PEDSTATE_LOOK_HEADING,
	PEDSTATE_WANDER_RANGE,
	PEDSTATE_WANDER_PATH,
	PEDSTATE_SEEK_POSITION,
	PEDSTATE_SEEK_ENTITY,
	PEDSTATE_FLEE_POSITION,
	PEDSTATE_FLEE_ENTITY,
	PEDSTATE_PURSUE,
	PEDSTATE_FOLLOW_PATH,
	PEDSTATE_SNIPER_MODE,
	PEDSTATE_ROCKETLAUNCHER_MODE,
	PEDSTATE_DUMMY,
	PEDSTATE_PAUSE,
	PEDSTATE_ATTACK,
	PEDSTATE_FIGHT,
	PEDSTATE_FACE_PHONE,
	PEDSTATE_MAKE_PHONECALL,
	PEDSTATE_CHAT,
	PEDSTATE_MUG,
	PEDSTATE_AIMGUN,
	PEDSTATE_AI_CONTROL,
	PEDSTATE_SEEK_CAR,
	PEDSTATE_SEEK_BOAT_POSITION,
	PEDSTATE_FOLLOW_ROUTE,
	PEDSTATE_CPR,
	PEDSTATE_SOLICIT,
	PEDSTATE_BUY_ICE_CREAM,
	PEDSTATE_INVESTIGATE_EVENT,
	PEDSTATE_EVADE_STEP,
	PEDSTATE_ON_FIRE,
	PEDSTATE_SUNBATHE,
	PEDSTATE_FLASH,
	PEDSTATE_JOG,
	PEDSTATE_ANSWER_MOBILE,
	PEDSTATE_HANG_OUT,
	PEDSTATE_STATES_NO_AI,
	PEDSTATE_ABSEIL_FROM_HELI,
	PEDSTATE_SIT,
	PEDSTATE_JUMP,
	PEDSTATE_FALL,
	PEDSTATE_GETUP,
	PEDSTATE_STAGGER,
	PEDSTATE_EVADE_DIVE,
	PEDSTATE_STATES_CAN_SHOOT,
	PEDSTATE_ENTER_TRAIN,
	PEDSTATE_EXIT_TRAIN,
	PEDSTATE_ARREST_PLAYER,
	PEDSTATE_DRIVING,
	PEDSTATE_PASSENGER,
	PEDSTATE_TAXI_PASSENGER,
	PEDSTATE_OPEN_DOOR,
	PEDSTATE_DIE,
	PEDSTATE_DEAD,
	PEDSTATE_DIE_BY_STEALTH,
	PEDSTATE_CARJACK,
	PEDSTATE_DRAGGED_FROM_CAR,
	PEDSTATE_ENTER_CAR,
	PEDSTATE_STEAL_CAR,
	PEDSTATE_EXIT_CAR,
	PEDSTATE_HANDS_UP,
	PEDSTATE_ARRESTED,
	PEDSTATE_DEPLOY_STINGER
};

OpcodeResult SET_CHAR_ARRESTED(CRunningScript* thread)
{
	GetScriptParams(thread, 1);
	DWORD *ped = GetPedStruct(pedPool, opcodeParams[0].dwParam);
	*(uint32_t *)((uint32_t)(ped) + 1384) = PEDSTATE_ARRESTED;
	return OR_CONTINUE;
}

OpcodeResult GET_TIME_CHAR_IS_DEAD(CRunningScript* thread)
{
	GetScriptParams(thread, 1);
	DWORD *ped = GetPedStruct(pedPool, opcodeParams[0].dwParam);
	int time = -1;
	if (*(float *)((uint32_t)(ped) + 1408) <= 0.0f) time = CTimer_m_snTimeInMilliseconds - *(uint32_t *)((uint32_t)(ped) + 0x790);

	opcodeParams[0].dwParam = time;
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

OpcodeResult READ_STRUCT_OFFSET(CRunningScript* thread)
{
	GetScriptParams(thread, 3);
	unsigned int struc = opcodeParams[0].dwParam;
	unsigned int offset = opcodeParams[1].dwParam;
	unsigned int size = opcodeParams[2].dwParam;
	unsigned int result = 0;

	if (*thread->GetBytePointer() > 8) { // is string
		opcodeParams[0].dwParam = (DWORD)((char *)(struc + offset));
	} else { 
		memcpy(&result, (void *)(struc + offset), size);
		opcodeParams[0].dwParam = result;
	}

	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

OpcodeResult WRITE_STRUCT_OFFSET(CRunningScript* thread)
{
	GetScriptParams(thread, 4);
	unsigned int struc = opcodeParams[0].dwParam;
	unsigned int offset = opcodeParams[1].dwParam;
	unsigned int size = opcodeParams[2].dwParam;
	unsigned int value;

	if (*thread->GetBytePointer() > 8) { // is string
		*(char **)(struc + offset) = opcodeParams[3].pcParam;
	}
	else {
		value = opcodeParams[3].dwParam;
		memcpy((void *)(struc + offset), &value, size);
	}
	return OR_CONTINUE;
}

OpcodeResult COPY_MEMORY(CRunningScript* thread)
{
	GetScriptParams(thread, 3);
	unsigned int from = opcodeParams[0].dwParam;
	unsigned int to = opcodeParams[1].dwParam;
	unsigned int size = opcodeParams[2].dwParam;
	memcpy((void*)to, (void*)from, size);
	return OR_CONTINUE;
}

OpcodeResult READ_STRUCT_OFFSET_MULTI(CRunningScript* thread)
{
	GetScriptParams(thread, 4);
	unsigned int struc = opcodeParams[0].dwParam;
	unsigned int offset = opcodeParams[1].dwParam;
	unsigned int count = opcodeParams[2].dwParam;
	unsigned int size = opcodeParams[3].dwParam;

	struc += offset;

	for (unsigned int i = 0; i < (count * size); i += size)
	{
		SCRIPT_VAR *pointer = GetScriptParamPointer(thread, 1);
		if (pointer != NULL) {
			*(DWORD*)pointer = 0;
			memcpy(pointer, (void*)(struc + i), size);
		}
	}
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

OpcodeResult WRITE_STRUCT_OFFSET_MULTI(CRunningScript* thread)
{
	GetScriptParams(thread, 4);
	unsigned int struc = opcodeParams[0].dwParam;
	unsigned int offset = opcodeParams[1].dwParam;
	unsigned int count = opcodeParams[2].dwParam;
	unsigned int size = opcodeParams[3].dwParam;
	unsigned int value;

	struc += offset;

	for (unsigned int i = 0; i < (count * size); i += size)
	{
		GetScriptParams(thread, 1);
		value = opcodeParams[0].dwParam;
		memcpy((void *)(struc + i), &value, size);
	}
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

OpcodeResult WRITE_STRUCT_PARAM(CRunningScript* thread)
{
	GetScriptParams(thread, 3);
	unsigned int *struc = (unsigned int *)opcodeParams[0].dwParam;
	int param = opcodeParams[1].dwParam;

	struc[param] = opcodeParams[2].dwParam;
	return OR_CONTINUE;
}

OpcodeResult READ_STRUCT_PARAM(CRunningScript* thread)
{
	GetScriptParams(thread, 2);
	unsigned int *struc = (unsigned int *)opcodeParams[0].dwParam;
	int param = opcodeParams[1].dwParam;

	opcodeParams[0].dwParam = struc[param];
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

typedef struct {
    signed short LeftStickX; // move/steer left (-128?)/right (+128)
    signed short LeftStickY; // move back(+128)/forwards(-128?)
    signed short RightStickX; // numpad 6(+128)/numpad 4(-128?)
    signed short RightStickY;
    
    signed short LeftShoulder1;
    signed short LeftShoulder2;
    signed short RightShoulder1; // target / hand brake
    signed short RightShoulder2; 
    
    signed short DPadUp; // radio change up           Next radio station / Call gang forward/Recruit gang member
    signed short DPadDown; // radio change down       Previous radio station / Gang stay back/Release gang (hold)
    signed short DPadLeft; //                         Skip trip/Action / Negative talk reply
    signed short DPadRight; //                        Next user MP3 track / Positive talk reply
    
    signed short Start;                             //Pause
    signed short Select;                            //Camera modes
    
    signed short ButtonSquare; // jump / reverse      Break/Reverse / Jump/Climb
    signed short ButtonTriangle; // get in/out        Exit vehicle / Enter veihcle
    signed short ButtonCross; // sprint / accelerate  Accelerate / Sprint/Swim
    signed short ButtonCircle; // fire                Fire weapon
    
    signed short ShockButtonL;
    signed short ShockButtonR; // look behind

} CControllerState;

typedef struct {
    CControllerState NewState;
    CControllerState OldState;
    short SteeringLeftRightBuffer[10];
    int DrunkDrivingBufferUsed;
    CControllerState PCTempJoyState;
    char Phase;

    char _pad109;

    short Mode;
    short ShakeDur;

    union
    {
        struct
        {
            unsigned short unk1 : 1; // eCamMode::MODE_1STPERSON leftover?
            unsigned short unk2 : 1; // unused
            unsigned short bPlayerAwaitsInGarage : 1;
            unsigned short bPlayerOnInteriorTransition : 1;
            unsigned short unk3 : 1; // unused
            unsigned short bPlayerSafe : 1;
            unsigned short bPlayerTalksOnPhone : 1; // bPlayerSafeForPhoneCall?
            unsigned short bPlayerSafeForCutscene : 1;
            unsigned short bPlayerSkipsToDestination : 1; // bPlayerSafeForDestination?
        };
        unsigned short DisablePlayerControls; 
    };
    char ShakeFreq;
    char bHornHistory[5];
    char iCurrHornHistory;
    char JustOutOfFrontEnd;
    char bApplyBrakes;
    char bDisablePlayerEnterCar;
    char bDisablePlayerDuck;
    char bDisablePlayerFireWeapon;
    char bDisablePlayerFireWeaponWithL1;
    char bDisablePlayerCycleWeapon;
    char bDisablePlayerJump;
    char bDisablePlayerDisplayVitalStats;
    int LastTimeTouched;
    int AverageWeapon;
    int AverageEntries;
    int NoShakeBeforeThis;
    char NoShakeFreq;

    char _pad131[3];
} CPad;

extern "C" {
	CPad* CPad_GetPad(int);

	int CPad_GetLastTouched(CPad *);
}



OpcodeResult GET_TIME_NOT_TOUCHING_PAD(CRunningScript* thread)
{
	GetScriptParams(thread, 1);
	int playerId = opcodeParams[0].dwParam;

	opcodeParams[0].dwParam = CPad_GetLastTouched(CPad_GetPad(playerId));
	SetScriptParams(thread, 1);
	return OR_CONTINUE;
}

/*
============================================================

			PS2 opcode extender handler redirections

============================================================
*/

static OpcodeResult CPRINTLN(CRunningScript* thread);
static OpcodeResult SET_EXTENDED_VAR_BUFFER(CRunningScript* thread);
static OpcodeResult READ_MEMORY_BOOL(CRunningScript *thread);

static OpcodeResult skyOpcodeHandler(CRunningScript *thread, unsigned short opcode)
{
	switch (opcode) {
		case 0x1388:
			return CPRINTLN(thread);
		case 0x1389:
			return SET_EXTENDED_VAR_BUFFER(thread);
		case 0x138A:
			return READ_MEMORY_BOOL(thread);
		default:
			return opcode_defa(thread);
	}
}

//1388=-1,cprintln %1d% format %2d% 
OpcodeResult CPRINTLN(CRunningScript *thread)
{
	char fmt[128], text[256];

	readString(thread, fmt, sizeof(fmt));
	format(thread, text, sizeof(text), fmt);

	scePrintf("%s\n", text);

	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

//1389=1,set_extended_var_buffer %1d%
OpcodeResult SET_EXTENDED_VAR_BUFFER(CRunningScript *thread)
{
	char fmt[128], text[256];

	readString(thread, fmt, sizeof(fmt));
	format(thread, text, sizeof(text), fmt);

	scePrintf("%s\n", text);

	SkipUnusedParameters(thread);
	return OR_CONTINUE;
}

//138A=1,read_memory_bool %1d% %2d% // IF
OpcodeResult READ_MEMORY_BOOL(CRunningScript *thread)
{
	DWORD *Address;
	DWORD size;

	GetScriptParams(thread, 2);
	Address = (DWORD*)opcodeParams[0].pParam;
	size = opcodeParams[1].dwParam;

	switch(size){
	case 1:
		SetScriptCondResult(thread, *(BYTE*)Address);
		break;
	case 2:
		SetScriptCondResult(thread, *(WORD*)Address);
		break;
	case 4:
		SetScriptCondResult(thread, *Address);
		break;
	}

	return OR_CONTINUE;
}

/*
============================================================

			PS2 opcode extender handler redirections

============================================================
*/


static int (*customOpcodeHandlerTable[])(CRunningScript*, WORD) = { 
(int (*)(CRunningScript*, WORD))0x1FAF00,
(int (*)(CRunningScript*, WORD))0x1FC470,
(int (*)(CRunningScript*, WORD))0x1FE500,
(int (*)(CRunningScript*, WORD))0x317060,
(int (*)(CRunningScript*, WORD))0x318910,
(int (*)(CRunningScript*, WORD))0x30BEA0,
(int (*)(CRunningScript*, WORD))0x30D2F0,
(int (*)(CRunningScript*, WORD))0x30DE40,
(int (*)(CRunningScript*, WORD))0x3101B0,
(int (*)(CRunningScript*, WORD))0x313390,
(int (*)(CRunningScript*, WORD))0x32FE20,
(int (*)(CRunningScript*, WORD))0x3310A0,
(int (*)(CRunningScript*, WORD))0x3A61F0,
(int (*)(CRunningScript*, WORD))0x3A7EA0,
(int (*)(CRunningScript*, WORD))0x3B8A20,
(int (*)(CRunningScript*, WORD))0x3BB520,
(int (*)(CRunningScript*, WORD))0x49AA20,
(int (*)(CRunningScript*, WORD))0x49D730,
(int (*)(CRunningScript*, WORD))0x4D0F90,
(int (*)(CRunningScript*, WORD))0x4D4380,
(int (*)(CRunningScript*, WORD))0x501690,
(int (*)(CRunningScript*, WORD))0x504430,
(int (*)(CRunningScript*, WORD))0x5237E0,
(int (*)(CRunningScript*, WORD))0x525710,
(int (*)(CRunningScript*, WORD))0x527460,
(int (*)(CRunningScript*, WORD))0x599D80,
(int (*)(CRunningScript*, WORD))0x59BDF0,
(int (*)(CRunningScript*, WORD))customOpcodeHandler, // CLEO, starting from 2700
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))cleoPlusOpcodeHandler, // CLEOplus, starting from 3300
(int (*)(CRunningScript*, WORD))cleoPlusOpcodeHandler,
(int (*)(CRunningScript*, WORD))cleoPlusOpcodeHandler,
(int (*)(CRunningScript*, WORD))cleoPlusOpcodeHandler,
(int (*)(CRunningScript*, WORD))cleoPlusOpcodeHandler,
(int (*)(CRunningScript*, WORD))cleoPlusOpcodeHandler,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))NULL,
(int (*)(CRunningScript*, WORD))skyOpcodeHandler, // PS2 CLEO exclusive opcodes, starting from 5000
};

/*

int min(int a, int b) {
    return (a < b) ? a : b;
}


SCRIPT_VAR* GetPointerToLocalVariable(CRunningScript* pScript, int varIndex) {
    if (pScript->IsMission())
        return &missionLocals[varIndex];
    else
        return pScript->GetVarPtr(varIndex);
}

SCRIPT_VAR *GetPointerToLocalArrayElement(CRunningScript *pScript, int off, WORD idx, BYTE mul)
{
    int final_index = off + mul * idx;

    if ( pScript->IsMission() )
        return &missionLocals[final_index];
    return pScript->GetVarPtr(final_index);
}

void ReadArrayInformation(CRunningScript* pScript, WORD* outArrVarOffset, int* outArrElemIdx) {
    BYTE* ip = pScript->GetBytePointer();
    *outArrVarOffset = pScript->ReadDataWord();
    WORD arrayIndexVar = pScript->ReadDataVarIndex();
    bool isGlobalIndexVar = pScript->ReadDataVarIndex() < 0; // high bit set
    if (isGlobalIndexVar) {
        *outArrElemIdx = *reinterpret_cast<int*>(&CTheScripts_ScriptSpace[arrayIndexVar]);
    }
    else {
        *outArrElemIdx = GetPointerToLocalVariable(pScript, arrayIndexVar)->dwParam;
    }
}

void CollectParams(CRunningScript* pScript, int count) {
    WORD arrVarOffset;
    int arrElemIdx;
	DEBUG_TRACE("Collecting %d parameters from %s\n", count, pScript->GetName());

    for (int i = 0; i < count; i++) {
        switch (pScript->ReadDataByte()) {
        case DT_DWORD:
            opcodeParams[i].dwParam = pScript->ReadDataInt();
			DEBUG_TRACE("INT %d\n", opcodeParams[i].dwParam);
            break;
        case DT_VAR:
        {
            WORD index = pScript->ReadDataVarIndex();
            opcodeParams[i] = *(SCRIPT_VAR*)(&CTheScripts_ScriptSpace[index]);
			DEBUG_TRACE("VAR %d addr 0x%x\n", opcodeParams[i].dwParam, &CTheScripts_ScriptSpace[index]);
            break;
        }
        case DT_LVAR:
        {
            WORD index = pScript->ReadDataVarIndex();
            opcodeParams[i] = *GetPointerToLocalVariable(pScript, index);
			DEBUG_TRACE("LVAR %d\n", opcodeParams[i].dwParam);
            break;
        }
        case DT_BYTE:
            opcodeParams[i].dwParam = pScript->ReadDataByte();
			DEBUG_TRACE("BYTE %d\n", opcodeParams[i].dwParam);
            break;
        case DT_WORD:
            opcodeParams[i].dwParam = pScript->ReadDataWord();
			DEBUG_TRACE("WORD %d\n", opcodeParams[i].dwParam);
            break;
        case DT_FLOAT:
            opcodeParams[i].fParam = pScript->ReadDataFloat();
			DEBUG_TRACE("FLOAT %f\n", opcodeParams[i].fParam);
            break;
        case DT_VAR_ARRAY:
            ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
            opcodeParams[i] = *reinterpret_cast<SCRIPT_VAR*>(&CTheScripts_ScriptSpace[arrVarOffset + (4 * arrElemIdx)]);
			DEBUG_TRACE("VAR_ARRAY %d\n", opcodeParams[i].dwParam);
            break;
        case DT_LVAR_ARRAY:
            ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
            opcodeParams[i] = *GetPointerToLocalVariable(pScript, arrVarOffset + arrElemIdx);
			DEBUG_TRACE("LVAR_ARRAY %d\n", opcodeParams[i].dwParam);
            break;
        }
    }
}

void StoreParams(CRunningScript* pScript, WORD count) {
    WORD arrVarOffset;
    int  arrElemIdx;

    for (int i = 0; i < count; i++) {
        switch (pScript->ReadDataByte()) {
        case DT_VAR:
        {
            WORD index = pScript->ReadDataVarIndex();
            *(SCRIPT_VAR*)(&CTheScripts_ScriptSpace[index]) = opcodeParams[i];
            break;
        }
        case DT_LVAR:
        {
            WORD index = pScript->ReadDataVarIndex();
            *GetPointerToLocalVariable(pScript, index) = opcodeParams[i];
            break;
        }
        case DT_VAR_ARRAY:
            ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
            *(SCRIPT_VAR*)(&CTheScripts_ScriptSpace[arrVarOffset + 4 * arrElemIdx]) = opcodeParams[i];
            break;
        case DT_LVAR_ARRAY:
            ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
            *GetPointerToLocalVariable(pScript, arrVarOffset + arrElemIdx) = opcodeParams[i];
            break;
        }
    }
}

#define TEXTLABEL_SIZE 8
#define STRING_SIZE 16

void ReadTextLabelFromScript(CRunningScript* pScript, char* buffer, BYTE nBufferLength) {
    WORD arrVarOffset;
    int  arrElemIdx;

    BYTE type = pScript->ReadDataType();
    switch (type) {
    case DT_TEXTLABEL:
        for (int i = 0; i < TEXTLABEL_SIZE; i++)
            buffer[i] = pScript->ReadDataByte();
        break;

    case DT_VAR_TEXTLABEL:
    {
        WORD index = pScript->ReadDataVarIndex();
        strncpy(buffer, (char*)&CTheScripts_ScriptSpace[index], TEXTLABEL_SIZE);
        break;
    }

    case DT_LVAR_TEXTLABEL:
    {
        WORD index = pScript->ReadDataVarIndex();
        strncpy(buffer, (char*)GetPointerToLocalVariable(pScript, index), TEXTLABEL_SIZE);
        break;
    }

    case DT_VAR_TEXTLABEL_ARRAY:
    case DT_VAR_STRING_ARRAY:
        ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
        if (type == DT_VAR_TEXTLABEL_ARRAY)
            strncpy(buffer, (char*)&CTheScripts_ScriptSpace[TEXTLABEL_SIZE * arrElemIdx + arrVarOffset], TEXTLABEL_SIZE);
        else
            strncpy(buffer, (char*)&CTheScripts_ScriptSpace[STRING_SIZE * arrElemIdx + arrVarOffset], (BYTE)min(nBufferLength, STRING_SIZE));
        break;

    case DT_LVAR_TEXTLABEL_ARRAY:
    case DT_LVAR_STRING_ARRAY:
        ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
        if (type == DT_LVAR_TEXTLABEL_ARRAY)
            strncpy(buffer, (char*) GetPointerToLocalArrayElement(pScript, arrVarOffset, arrElemIdx, 2), TEXTLABEL_SIZE);
        else {
            BYTE bufferLength = (BYTE)min(nBufferLength, STRING_SIZE);
            strncpy(buffer, (char*)GetPointerToLocalArrayElement(pScript, arrVarOffset, arrElemIdx, 4), bufferLength);
        }
        break;

    case DT_VARLEN_STRING:
    {
        short nStringLen = pScript->ReadDataByte(); // sign extension. max size = 127, not 255
        for (int i = 0; i < nStringLen; i++)
            buffer[i] = pScript->ReadDataByte();

        if (nStringLen < nBufferLength)
            memset(&buffer[(BYTE)nStringLen], 0, (BYTE)(nBufferLength - nStringLen));
        break;
    }

    case DT_STRING:
        // slightly changed code: original code is a bit messy and calls Read1ByteFromScript
        // in a loop and does some additional checks to ensure that buffer can hold the data
        strncpy(buffer, (const char*)pScript->CurrentIP, min(nBufferLength, STRING_SIZE));
        pScript->CurrentIP += STRING_SIZE;
        break;

    case DT_VAR_STRING:
    {
        WORD index = pScript->ReadDataVarIndex();
        strncpy(buffer, (char*)&CTheScripts_ScriptSpace[index], min(nBufferLength, STRING_SIZE));
        break;
    }

    case DT_LVAR_STRING:
    {
        WORD index = pScript->ReadDataVarIndex();
        strncpy(buffer, (char*)GetPointerToLocalVariable(pScript, index), min(nBufferLength, STRING_SIZE));
        break;
    }

    default:
        break;
    }
}


SCRIPT_VAR CollectNextParameterWithoutIncreasingPC(CRunningScript* pScript) {
    WORD arrVarOffset;
    int arrElemIdx;
	DEBUG_TRACE("Collecting %d parameters from %s\n", count, pScript->GetName());

	SCRIPT_VAR result;


	BYTE* ip = pScript->GetBytePointer();

    switch (pScript->ReadDataByte()) {
    case DT_DWORD:
        result.dwParam = pScript->ReadDataInt();
		DEBUG_TRACE("INT %d\n", opcodeParams[i].dwParam);
        break;
    case DT_VAR:
    {
        WORD index = pScript->ReadDataVarIndex();
        result = *(SCRIPT_VAR*)(&CTheScripts_ScriptSpace[index]);
        break;
    }
    case DT_LVAR:
    {
        WORD index = pScript->ReadDataVarIndex();
        result = *GetPointerToLocalVariable(pScript, index);
        break;
    }
    case DT_BYTE:
        result.dwParam = pScript->ReadDataByte();
        break;
    case DT_WORD:
        result.dwParam = pScript->ReadDataWord();
        break;
    case DT_FLOAT:
        result.fParam = pScript->ReadDataFloat();
        break;
    case DT_VAR_ARRAY:
        ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
        result = *reinterpret_cast<SCRIPT_VAR*>(&CTheScripts_ScriptSpace[arrVarOffset + (4 * arrElemIdx)]);
        break;
    case DT_LVAR_ARRAY:
        ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
        result = *GetPointerToLocalVariable(pScript, arrVarOffset + arrElemIdx);
        break;
    }

	pScript->SetIp(ip);

	return result;
}

SCRIPT_VAR* GetPointerToScriptVariable(CRunningScript* pScript) {
    BYTE  arrElemSize;
    WORD arrVarOffset;
    int  arrElemIdx;

    BYTE type = pScript->ReadDataByte();
    switch (type) {
    case DT_VAR:
    case DT_VAR_TEXTLABEL:
    case DT_VAR_STRING:
    {
        WORD index = pScript->ReadDataWord();
        return reinterpret_cast<SCRIPT_VAR*>(&CTheScripts_ScriptSpace[index]);
    }
    case DT_LVAR:
    case DT_LVAR_TEXTLABEL:
    case DT_LVAR_STRING:
    {
        WORD index = pScript->ReadDataWord();
        return GetPointerToLocalVariable(pScript, index);
    }

    case DT_VAR_ARRAY:
    case DT_VAR_TEXTLABEL_ARRAY:
    case DT_VAR_STRING_ARRAY:
        ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
        if (type == DT_VAR_STRING_ARRAY)
            return reinterpret_cast<SCRIPT_VAR*>(&CTheScripts_ScriptSpace[STRING_SIZE * arrElemIdx + arrVarOffset]);
        else if (type == DT_VAR_TEXTLABEL_ARRAY)
            return reinterpret_cast<SCRIPT_VAR*>(&CTheScripts_ScriptSpace[TEXTLABEL_SIZE * arrElemIdx + arrVarOffset]);
        else // SCRIPT_PARAM_GLOBAL_NUMBER_ARRAY
            return reinterpret_cast<SCRIPT_VAR*>(&CTheScripts_ScriptSpace[4 * arrElemIdx + arrVarOffset]);

    case DT_LVAR_ARRAY:
    case DT_LVAR_TEXTLABEL_ARRAY:
    case DT_LVAR_STRING_ARRAY:
        ReadArrayInformation(pScript, &arrVarOffset, &arrElemIdx);
        if (type == DT_LVAR_STRING_ARRAY)
            arrElemSize = 4;
        else if (type == DT_LVAR_TEXTLABEL_ARRAY)
            arrElemSize = 2;
        else // SCRIPT_PARAM_LOCAL_NUMBER_ARRAY
            arrElemSize = 1;
        return GetPointerToLocalArrayElement(pScript, arrVarOffset, arrElemIdx, arrElemSize);
    }
}

*/

#define offsetof(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

int CRunningScript_Process(CRunningScript *thread)
{
    void *v2; // eax
    bool v3; // zf
    WORD v4; // ax
    BYTE *scriptPC;
    WORD opcode;
	int* CommandsExecuted = (int*)0x66B53C;

    if ( thread->SceneSkipIP && CCutsceneMgr_IsCutsceneSkipButtonBeingPressed() )
    {
        _StyledText_2 = 0;
        v2 = thread->SceneSkipIP;
        if ( (int)v2 >= 0 )
            thread->CurrentIP = (BYTE*)&CTheScripts_ScriptSpace + (DWORD)v2;
        else
            thread->CurrentIP = (BYTE*)((DWORD)thread->BaseIP - (DWORD)v2);
        thread->SceneSkipIP = 0;
        thread->WakeTime = 0;
    }
    if ( thread->bUseMissionCleanup )
        CRunningScript_DoDeatharrestCheck(thread);
    if ( thread->bIsMission && CTheScripts_FailCurrentMission == 1 )
    {
        v3 = thread->SP == 1;
        if ( thread->SP > 1u )
        {
            v4 = thread->SP;
            do
                --v4;
            while ( v4 > 1u );
            thread->SP = v4;
            v3 = v4 == 1;
        }
        if ( v3 )
        {
            thread->SP = 0;
            thread->CurrentIP = thread->Stack[offsetof(CRunningScript, Previous)];
        }
    }
    CTheScripts_ReinitialiseSwitchStatementData();
    if ( CTimer_m_snTimeInMilliseconds >= thread->WakeTime )
    {
		
        do
        {
			++*CommandsExecuted;
            scriptPC = thread->CurrentIP;

			DWORD align_diff = (DWORD)scriptPC % 4;
			DWORD aligned_addr = (DWORD)scriptPC;

			if(align_diff > 0){
				aligned_addr = ((DWORD)aligned_addr - align_diff);
				opcode = (WORD)(*(DWORD*)(aligned_addr) >> (8*align_diff));
				opcode += (WORD)(*(DWORD*)(aligned_addr+4) << (8*(4-align_diff)));
			} else {
				opcode = *(WORD*)scriptPC;
			}

			DEBUG_TRACE("CRunningScript::Proccess - CurrentIP pointer: 0x%x - Opcode: %04x\n", scriptPC, opcode);

            thread->CurrentIP = (BYTE*)(scriptPC + 2);
            thread->NotFlag = (opcode & 0x8000) != 0;
        }
        while ( !customOpcodeHandlerTable[((opcode & 0x7FFF) / 100)](thread, (opcode & 0x7FFF)) );
    }

	return 0;
}

#define jal(fun) 0x0C000000 + (fun/4)

static void RedirectCall(unsigned int call_addr, void* addr){
    *(unsigned int*)call_addr = jal((uint32_t)addr);
}

/*static void RedirectFunction(void* old, void* new_func) {
    uint32_t* function = (uint32_t*)old;
    uint32_t patch = (uint32_t)new_func;

    function[0] = 0x08000000+(patch/4);
    function[1] = 0;

}*/

extern "C" {
	int _start()
	{
		RedirectCall(0x1FA25C, (void*)CRunningScript_Process);
		//RedirectFunction((void*)GetScriptParams, (void*)CollectParams);
		//RedirectFunction((void*)SetScriptParams, (void*)StoreParams);
		//RedirectFunction((void*)0x1F7200, (void*)ReadTextLabelFromScript);
		//RedirectFunction((void*)0x1F7AA0, (void*)CollectNextParameterWithoutIncreasingPC);
		//RedirectFunction((void*)GetScriptParamPointer, (void*)GetPointerToScriptVariable);
		
		return 0;
	}
}
