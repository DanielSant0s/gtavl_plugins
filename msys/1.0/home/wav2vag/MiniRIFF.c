/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#include <stdio.h>
#include <stdint.h>
/**************************************/
#include "MiniRIFF.h"
/**************************************/

//! Read RIFF chunk
int RIFF_CkRead(FILE *File, void *User, const struct RIFF_CkHdl_t *CkHdl, const struct RIFF_CkListHdl_t *ListHdl) {
	int RetVal = 0;

	//! Read header
	//! NOTE: Size needs to be aligned to 2 bytes as per specification.
	struct RIFF_CkHeader_t Ck; fread(&Ck, sizeof(Ck), 1, File);
	int CkDataBeg = ftell(File);
	int CkDataEnd = CkDataBeg + ((Ck.Size+1) &~ 1);

	//! RIFF/LIST?
	if(Ck.Type == RIFF_FOURCC('R','I','F','F') || Ck.Type == RIFF_FOURCC('L','I','S','T')) {
		if(ListHdl) {
			//! Read list type, find handler
			uint32_t Type; fread(&Type, sizeof(Type), 1, File);
			for(;ListHdl->Type;ListHdl++) if(ListHdl->Type == Type) {
				//! Issue begin callback, parse list, then issue end callback
				if(ListHdl->ListCbBeg) {
					RetVal = ListHdl->ListCbBeg(File, User);
					if(RetVal < 0) break;
				}
				while(ftell(File) < CkDataEnd) {
					RetVal = RIFF_CkRead(File, User, ListHdl->CkHdl, ListHdl->ListHdl);
					if(RetVal < 0) break;
				}
				if(ListHdl->ListCbEnd) {
					RetVal = ListHdl->ListCbEnd(File, User);
					if(RetVal < 0) break;
				}
				break;
			}
		}
	} else if(CkHdl) {
		//! Not a RIFF/LIST, so interpret as a normal chunk
		for(;CkHdl->Type;CkHdl++) if(CkHdl->Type == Ck.Type) {
			RetVal = CkHdl->Func(File, User, &Ck);
			break;
		}
	}

	//! Seek to next chunk
	fseek(File, CkDataEnd, SEEK_SET);
	return RetVal;
}

/**************************************/
//! EOF
/**************************************/
