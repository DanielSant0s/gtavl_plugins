/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#pragma once
/**************************************/
#include <stdint.h>
/**************************************/

#define WAVE_ENOFILE  (-1)
#define WAVE_ENOMEM   (-2)
#define WAVE_ENOTWAV  (-3)
#define WAVE_EINVALID (-4)

#define WAVE_CHUNKFLAG_WAVE      (1<<0)
#define WAVE_CHUNKFLAG_FMT       (1<<1)
#define WAVE_CHUNKFLAG_CUE       (1<<2)
#define WAVE_CHUNKFLAG_ADTL_LABL (1<<3)
#define WAVE_CHUNKFLAG_SMPL      (1<<4)
#define WAVE_CHUNKFLAG_DATA      (1<<5)

/**************************************/

//! WAVE -> fmt
struct WAVE_fmt_t {
	uint16_t wFormatTag;
	uint16_t nChannels;
	uint32_t nSamplesPerSec;
	uint32_t nAvgBytesPerSec;
	uint16_t nBlockAlign;
	uint16_t wBitsPerSample;
	uint16_t cbSize;
	uint16_t wValidBitsPerSample;
	uint32_t dwChannelMask;
	uint8_t  SubFormat[16];
};

//! WAVE -> cue
//! WAVE -> cue -> cue_point
struct WAVE_cue_point_t {
	uint32_t dwName;
	uint32_t dwPosition;
	uint32_t fccChunk;
	uint32_t dwChunkStart;
	uint32_t dwBlockStart;
	uint32_t dwSampleOffset;
};
struct WAVE_cue_t {
	uint32_t dwCuePoints;
	struct WAVE_cue_point_t cuePoints[];
};

//! WAVE -> adtl -> labl
struct WAVE_adtl_labl_t {
	uint32_t dwName;
	char data[];
};

//! WAVE -> smpl
struct WAVE_smpl_loop_t {
	uint32_t dwIdentifier;
	uint32_t dwType;
	uint32_t dwStart;
	uint32_t dwEnd;
	uint32_t dwFraction;
	uint32_t dwPlayCount;
};
struct WAVE_smpl_t {
	uint32_t dwManufacturer;
	uint32_t dwProduct;
	uint32_t dwSamplePeriod;
	uint32_t dwMIDIUnityNote;
	uint32_t dwMIDIPitchFraction;
	uint32_t dwSMPTEFormat;
	uint32_t dwSMPTEOffset;
	uint32_t cSampleLoops;
	uint32_t cbSamplerData;
	struct WAVE_smpl_loop_t loopPoints[];
};

//! WAVE
struct WAVE_t {
	//! Bookkeeping
	uint32_t CkFlags;
	uint32_t data_Size;
	uint32_t cue_nCuePoints;
	uint32_t adtl_labl_nLabels;

	//! Chunk data
	struct WAVE_fmt_t        *fmt;
	struct WAVE_cue_point_t  *cue;
	struct WAVE_adtl_labl_t **adtl_labl;
	struct WAVE_smpl_t       *smpl;
	void                     *data;
};

/**************************************/

//! Successful opening guarantees fmt and data chunks
int  WAVE_ReadFile(struct WAVE_t *WavData, const char *Filename);
void WAVE_Destroy(struct WAVE_t *WavData);

/**************************************/
//! EOF
/**************************************/
