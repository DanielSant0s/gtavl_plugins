/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //! memset
/**************************************/
#include "WAVE.h"
#include "MiniRIFF.h"
/**************************************/

//! RIFF(WAVE) -> LIST(adtl) -> labl
static int RIFF_WAVE_LIST_adtl_labl(FILE *WavFile, void *User, const struct RIFF_CkHeader_t *Ck) {
	struct WAVE_t *WavData = User;

	//! Expand adtl_labl pointer array
	uint32_t nLabels = WavData->adtl_labl_nLabels;
	struct WAVE_adtl_labl_t **lablList = realloc(WavData->adtl_labl, (nLabels+1) * sizeof(*lablList));
	if(!lablList) return WAVE_ENOMEM;
	WavData->adtl_labl = lablList;

	//! Allocate space for this label and append to list
	struct WAVE_adtl_labl_t *labl = malloc(Ck->Size);
	if(!labl) return WAVE_ENOMEM;
	fread(labl, Ck->Size, 1, WavFile);
	lablList[nLabels] = labl; WavData->adtl_labl_nLabels = nLabels+1;
	WavData->CkFlags |= WAVE_CHUNKFLAG_ADTL_LABL;
	return 0;
}

/**************************************/

//! RIFF(WAVE) -> data
static int RIFF_WAVE_data(FILE *WavFile, void *User, const struct RIFF_CkHeader_t *Ck) {
	struct WAVE_t *WavData = User;

	//! Allocate/expand data and read at the end
	//! NOTE: I'm not sure if this is correct behaviour, but meh.
	char *Data = realloc(WavData->data, WavData->data_Size + Ck->Size);
	if(!Data) return WAVE_ENOMEM;
	fread(Data + WavData->data_Size, Ck->Size, 1, WavFile);
	WavData->data       = Data;
	WavData->data_Size += Ck->Size;
	WavData->CkFlags |= WAVE_CHUNKFLAG_DATA;
	return 0;
}

/**************************************/

//! RIFF(WAVE) -> smpl
static int RIFF_WAVE_smpl(FILE *WavFile, void *User, const struct RIFF_CkHeader_t *Ck) {
	struct WAVE_t *WavData = User;

	//! On encountering a new smpl chunk, override the old one
	struct WAVE_smpl_t *smpl = realloc(WavData->smpl, Ck->Size);
	if(!smpl) return WAVE_ENOMEM;
	fread(smpl, Ck->Size, 1, WavFile);
	WavData->smpl     = smpl;
	WavData->CkFlags |= WAVE_CHUNKFLAG_SMPL;
	return 0;
}

/**************************************/

//! RIFF(WAVE) -> cue
static int RIFF_WAVE_cue(FILE *WavFile, void *User, const struct RIFF_CkHeader_t *Ck) {
	struct WAVE_t *WavData = User;
	(void)Ck;

	//! Read number of cue points and early exit when none exist
	uint32_t nNewCuePoints; fread(&nNewCuePoints, sizeof(nNewCuePoints), 1, WavFile);
	if(!nNewCuePoints) return 0;

	//! Merge these cue points with the old ones
	//! NOTE: Again, I'm not sure if this is correct behaviour.
	uint32_t nOldCuePoints = WavData->cue_nCuePoints;
	struct WAVE_cue_point_t *cues = realloc(WavData->cue, (nOldCuePoints + nNewCuePoints) * sizeof(struct WAVE_cue_point_t));
	if(!cues) return WAVE_ENOMEM;
	fread(&cues[nOldCuePoints], sizeof(struct WAVE_cue_point_t), nNewCuePoints, WavFile);
	WavData->cue_nCuePoints = nOldCuePoints + nNewCuePoints;
	WavData->cue            = cues;
	WavData->CkFlags       |= WAVE_CHUNKFLAG_CUE;
	return 0;
}

/**************************************/

//! RIFF(WAVE) -> fmt
static int RIFF_WAVE_fmt(FILE *WavFile, void *User, const struct RIFF_CkHeader_t *Ck) {
	struct WAVE_t *WavData = User;
	(void)Ck;

	//! Having more than one fmt chunk should be incorrect, right?
	if((WavData->CkFlags & WAVE_CHUNKFLAG_FMT)) return WAVE_EINVALID;
	struct WAVE_fmt_t *fmt = malloc(Ck->Size);
	if(!fmt) return WAVE_ENOMEM;
	fread(fmt, Ck->Size, 1, WavFile);
	WavData->fmt      = fmt;
	WavData->CkFlags |= WAVE_CHUNKFLAG_FMT;
	return 0;
}

/**************************************/

//! Begin of RIFF(WAVE)
static int RIFF_WAVE_CbBeg(FILE *WavFile, void *User) {
	struct WAVE_t *WavData = User;
	(void)WavFile;

	//! This is just to ensure we have a valid WAV file
	WavData->CkFlags |= WAVE_CHUNKFLAG_WAVE;
	return 0;
}

/**************************************/
//! WAVE chunk hierarchy (read bottom to top)
/**************************************/

//! RIFF(WAVE) -> LIST(adtl) -> Chunks
static const struct RIFF_CkHdl_t RIFF_WAVE_LIST_adtl_Ck[] = {
	{RIFF_FOURCC('l','a','b','l'),RIFF_WAVE_LIST_adtl_labl},
	{0},
};

//! RIFF(WAVE) -> LIST
static const struct RIFF_CkListHdl_t RIFF_WAVE_LIST[] = {
	{RIFF_FOURCC('a','d','t','l'),RIFF_WAVE_LIST_adtl_Ck,NULL,NULL,NULL},
	{0},
};

//! RIFF(WAVE) -> Chunks
static const struct RIFF_CkHdl_t RIFF_WAVE_Ck[] = {
	{RIFF_FOURCC('f','m','t',' '),RIFF_WAVE_fmt},
	{RIFF_FOURCC('c','u','e',' '),RIFF_WAVE_cue},
	{RIFF_FOURCC('s','m','p','l'),RIFF_WAVE_smpl},
	{RIFF_FOURCC('d','a','t','a'),RIFF_WAVE_data},
#if 0 //! No thank you, life's complicated enough as it is
	{RIFF_FOURCC('w','a','v','l'),RIFF_WAVE_wavl},
	{RIFF_FOURCC('s','l','n','t'),RIFF_WAVE_slnt},
#endif
	{0},
};

//! RIFF(WAVE)
static const struct RIFF_CkListHdl_t RIFF_WAVE[] = {
	{RIFF_FOURCC('W','A','V','E'),RIFF_WAVE_Ck,RIFF_WAVE_LIST,RIFF_WAVE_CbBeg,NULL},
	{0},
};

/**************************************/

//! Read WAV file
int WAVE_ReadFile(struct WAVE_t *WavData, const char *Filename) {
	//! We must clear the entire structure before doing
	//! anything, as we will clean it up if we encounter
	//! any errors during processing
	memset(WavData, 0, sizeof(*WavData));

	//! Parse file via RIFF reader
	FILE *WavFile = fopen(Filename, "rb"); if(!WavFile) return WAVE_ENOFILE;
	int RetVal = RIFF_CkRead(WavFile, WavData, NULL, RIFF_WAVE);
	fclose(WavFile);

	//! Perform sanity checks
	do {
		//! If we encountered errors reading, early exit
		if(RetVal < 0) break;

		//! Ensure we have a valid WAV file
		uint32_t Flags = WavData->CkFlags;
		if((Flags & WAVE_CHUNKFLAG_WAVE) == 0) { RetVal = WAVE_ENOTWAV;  break; }
		if((Flags & WAVE_CHUNKFLAG_FMT)  == 0) { RetVal = WAVE_EINVALID; break; }
		if((Flags & WAVE_CHUNKFLAG_DATA) == 0) { RetVal = WAVE_EINVALID; break; }
	} while(0);

	//! If we have errors, ensure to destroy anything we had before exiting
	if(RetVal < 0) WAVE_Destroy(WavData);
	return RetVal;
}

//! Destroy WAV data
void WAVE_Destroy(struct WAVE_t *WavData) {
	uint32_t n;
	for(n=0;n<WavData->adtl_labl_nLabels;n++) free(WavData->adtl_labl[n]);
	free(WavData->fmt);
	free(WavData->cue);
	free(WavData->adtl_labl);
	free(WavData->smpl);
	free(WavData->data);
	memset(WavData, 0, sizeof(*WavData));
}

/**************************************/
//! EOF
/**************************************/
