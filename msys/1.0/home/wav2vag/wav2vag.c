/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#define _GNU_SOURCE //! strcasecmp, strcasestr
/**************************************/
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**************************************/
#include "MiniRIFF.h"
#include "WAVE.h"
#include "SPUADPCM.h"
#include "WavResample.h"
/**************************************/

//! LE32 -> BE32
#define BIG32(x) ((x)<<24 | ((x)&0xFF00)<<8 | ((x)&0xFF0000)>>8 | (x)>>24)

/**************************************/

char tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        // Se o caractere é uma letra maiúscula, converte para minúscula
        return c + ('a' - 'A');
    } else {
        // Se não for uma letra maiúscula, retorna o mesmo caractere
        return c;
    }
}

char *
strcasestr(s, find)
	const char *s, *find;
{
	char c, sc;
	size_t len;

	if ((c = *find++) != 0) {
		c = tolower((unsigned char)c);
		len = strlen(find);
		do {
			do {
				if ((sc = *s++) == 0)
					return (NULL);
			} while ((char)tolower((unsigned char)sc) != c);
		} while (strncasecmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

//! Parse loop points from WAV file
static inline int ParseLoopPoints_smpl(struct WAVE_t *WavData, int *LoopBeg, int *LoopEnd) {
	uint32_t n;

	//! Ensure we have a sampler chunk
	if((WavData->CkFlags & WAVE_CHUNKFLAG_SMPL) == 0) return 0;

	//! Find the first forward loop and use that
	for(n=0;n<WavData->smpl->cSampleLoops;n++) {
		struct WAVE_smpl_loop_t *Loop = &WavData->smpl->loopPoints[n];
		if(Loop->dwType == 0x00000000) { /* LOOP_FORWARD */
			*LoopBeg = Loop->dwStart, *LoopEnd = Loop->dwEnd;
			return 1;
		}
	}
	return 0;
}
static inline int ParseLoopPoints_labl(struct WAVE_t *WavData, int *LoopBeg, int *LoopEnd) {
	uint32_t n;

	//! Ensure we have cues and labels
	if((WavData->CkFlags & WAVE_CHUNKFLAG_CUE) == 0 || (WavData->CkFlags & WAVE_CHUNKFLAG_ADTL_LABL) == 0) return 0;

	//! Find the loop cues
	uint32_t nLabl = WavData->adtl_labl_nLabels;
	uint32_t LoopBegName = -1, LoopEndName = -1;
	for(n=0;n<nLabl;n++) {
		struct WAVE_adtl_labl_t *labl = WavData->adtl_labl[n];
		const char *Str = labl->data;

		//! Find an instance of "loop" somewhere in the label
		if(!strcasestr(Str, "loop")) continue;

		//! Next, find any variation of "start" or "end"
		if(strcasestr(Str, "beg") || strcasestr(Str, "start")) LoopBegName = labl->dwName;
		if(strcasestr(Str, "end")                            ) LoopEndName = labl->dwName;
	}
	if(LoopBegName == -1u || LoopEndName == -1u) return 0;

	//! Get loop points from cues
	uint32_t nCues = WavData->cue_nCuePoints;
	uint32_t Beg = -1, End = -1;
	for(n=0;n<nCues;n++) {
		struct WAVE_cue_point_t *cue = &WavData->cue[n];
		if(cue->fccChunk != RIFF_FOURCC('d','a','t','a')) continue;
		if(cue->dwName == LoopBegName) Beg = cue->dwSampleOffset;
		if(cue->dwName == LoopEndName) End = cue->dwSampleOffset;
	}
	if(Beg == -1u || End == -1u)  return 0;

	//! Set loop points
	*LoopBeg = Beg;
	*LoopEnd = End;
	return 1;
}
static void ParseLoopPoints(struct WAVE_t *WavData, int *DstLoopBeg, int *DstLoopEnd) {
	//! Try to find loop points in smpl chunk, or labels if not there
	//! NOTE: LoopEnd is /inclusive/, and we're looking for /exclusive/
	//! so we have to advance LoopEnd by 1 sample.
	int LoopBeg, LoopEnd;
	if(!ParseLoopPoints_smpl(WavData, &LoopBeg, &LoopEnd)) {
		if(!ParseLoopPoints_labl(WavData, &LoopBeg, &LoopEnd)) return;
	}
	LoopEnd++;

	//! Sanity check (inverted loop points, 1-sample loops)
	if(LoopEnd-LoopBeg < 2) return;

	//! If we already had loop points, ask to change them
	if(*DstLoopBeg != -1 /* && *DstLoopEnd != -1*/) {
		printf("Found loop in WAV file (%d-%d). Override provided loop and use this? ", LoopBeg, LoopEnd);
		int Ret = getchar();
		if(Ret != 'y' && Ret != 'Y') return;
	}
	*DstLoopBeg = LoopBeg;
	*DstLoopEnd = LoopEnd;
}

/**************************************/

//! Resample so loop size and loop start are multiples of frame size
static int DoEndPadding(struct WAVE_t *WavData, int *nSamples, int nPad, int LoopBeg) {
	int16_t *Data = realloc(WavData->data, (*nSamples + nPad)*sizeof(int16_t));
	if(!Data) return 0;
	WavData->data = Data, WavData->data_Size += nPad * sizeof(int16_t);

	//! Pad data at the end
	const int16_t *Src = (LoopBeg != -1) ? (Data + LoopBeg) : NULL;
	      int16_t *Dst = Data + *nSamples; *nSamples += nPad;
	do *Dst++ = Src ? (*Src++) : 0; while(--nPad);
	return 1;
}
static double DoResampling(struct WAVE_t *WavData, int *LoopBeg, int *LoopEnd, int *nSamples, double ResampleFactor, int ResamplingType) {
	//! Check for loop size alignment
	int InLoopSize = *LoopEnd - *LoopBeg;
	int OutLoopSize; {
		//! Arbitrary tolerance of 0.001 samples for fractional positioning
		double fOutLoopSize = InLoopSize * ResampleFactor;
		OutLoopSize = (int)ceil(fOutLoopSize);
		if(OutLoopSize-fOutLoopSize > 0.001 || OutLoopSize%SPUADPCM_FRAME_LEN) {
			//! Warn user and adjust resampling
			printf("WARNING: Loop size is not a multiple of %d. Resampling will take place.\n", SPUADPCM_FRAME_LEN);
			OutLoopSize = (OutLoopSize/SPUADPCM_FRAME_LEN + 1) * SPUADPCM_FRAME_LEN;
			ResampleFactor = OutLoopSize / (double)InLoopSize;
		}
	}

	//! Get number of samples before and after resampling
	int nInSamples, nOutSamples;
	if(*LoopBeg != -1 /* && *LoopEnd != -1*/) {
		nInSamples  = *LoopEnd;
			*LoopBeg = (int)(*LoopBeg * ResampleFactor);
			*LoopEnd = *LoopBeg + OutLoopSize;
		nOutSamples = *nSamples = *LoopEnd;
	} else {
		nInSamples  = *nSamples;
		nOutSamples = *nSamples = (int)(*nSamples * ResampleFactor);
	}

	//! Perform resampling?
	//! NOTE: Sample rate is also adjusted (no tuning control in VAG)
	if(ResampleFactor != 1.0) {
		int16_t *NewData = WavResample(WavData->data, nInSamples, nOutSamples, ResampleFactor, ResamplingType, InLoopSize);
		if(!NewData) {
			printf("ERROR: Unable to perform resampling (out of memory).\n");
			return 0.0;
		}
		free(WavData->data); WavData->data = NewData, WavData->data_Size = nOutSamples * sizeof(int16_t);
		WavData->fmt->nSamplesPerSec = (uint32_t)lrint(WavData->fmt->nSamplesPerSec * ResampleFactor);
	}

	//! Re-align sample points to frames and return final resampling factor
	int nAlign = nOutSamples % SPUADPCM_FRAME_LEN;
	if(nAlign) {
		nAlign = SPUADPCM_FRAME_LEN - nAlign;
		if(!DoEndPadding(WavData, &nOutSamples, nAlign, *LoopBeg)) {
			printf("ERROR: Unable to perform loop point adjustment (out of memory).\n");
			return 0.0;
		}
		*nSamples = nOutSamples;

		//! Advance loop points
		if(*LoopBeg != -1 /* && *LoopEnd != -1*/) *LoopBeg += nAlign, *LoopEnd += nAlign;
	}
	return ResampleFactor;
}

/**************************************/

int main(int argc, const char *argv[]) {
	//! Check arguments
	if(argc < 3) {
		printf(
			"wav2vag - Aikku93's WAV->VAG Encoding Tool\n"
			"Usage:\n"
			" wav2vag Input.wav Output.vag [Opt]\n"
			"Options:\n"
			" -l:Begin[-End] - Set loop points (eg. \"-l:2800-5628\")\n"
			"                  Loop end is exclusive (ie. end sample is NOT played),\n"
			"                  and if not provided, is assumed to be end-of-sample.\n"
			" -r:1.0         - Set resampling factor\n"
			" -rt:sinc       - Set resampling method (lin, cubic, sinc)\n"
			" -name:[blank]  - Set \"track name\" (VAG header)\n"
			"Loop points will attempt to be fetched from the WAV file, either\n"
			"from a `smpl` chunk or labels that contain some variation of the\n"
			"words 'loop' and 'start/beg/end'.\n"
			"If the loop start point is not aligned to 28 samples, padding will\n"
			"be used at the end of the file and the loop point moved forward.\n"
			"If the loop length is not a multiple of 28 samples, resampling will\n"
			"take place. This may cause loops to click, and is best avoided.\n"
		);
		return 1;
	}

	//! Parse arguments
	int LoopBeg = -1, LoopEnd = -1;
	int ResamplingType = WAVRESAMPLE_SINC;
	double ResampleFactor = 1.0;
	const char *TrackName = NULL;
	{
		int n;
		for(n=3;n<argc;n++) {
			if(!memcmp(argv[n], "-l:", 3)) {
				const char *LoopBegStr = argv[n] + 3; //! Skip "-l:"
				const char *LoopEndStr = strchr(LoopBegStr, '-'); if(LoopEndStr) LoopEndStr++; //! Skip over the hyphen
				LoopBeg = atoi(LoopBegStr);
				if(LoopEndStr) LoopEnd = atoi(LoopEndStr);
			}

			else if(!memcmp(argv[n], "-r:", 3)) {
				double x = atof(argv[n]+3);
				if(x <= 0.0) {
					printf("WARNING: Invalid resampling factor. Ignoring...\n");
					continue;
				}
				ResampleFactor = x;
			}

			else if(!memcmp(argv[n], "-rt:", 4)) {
				const char *Str = argv[n]+4;
				     if(!strcasecmp(Str, "lin"))   ResamplingType = WAVRESAMPLE_LINEAR;
				else if(!strcasecmp(Str, "cubic")) ResamplingType = WAVRESAMPLE_CUBIC;
				else if(!strcasecmp(Str, "sinc"))  ResamplingType = WAVRESAMPLE_SINC;
				else printf("WARNING: Invalid resampling method. Ignoring...\n");
			}

			else if(!memcmp(argv[n], "-name:", 6)) {
				TrackName = argv[n]+6;
			}

			else printf("WARNING: Ignoring unknown argument (%s)\n", argv[n]);
		}
	}

	//! Read source file
	struct WAVE_t WavData; {
		int Ret = WAVE_ReadFile(&WavData, argv[1]);
		if(Ret < 0) {
			printf("ERROR: Unable to open WAV file (error %d)\n", Ret);
			return -1;
		}

		//! Check format
		struct WAVE_fmt_t *fmt = WavData.fmt;
		if(
			fmt->wFormatTag     != 0x0001 || /*WAVE_FORMAT_PCM*/
			fmt->nChannels      != 1      ||
			fmt->wBitsPerSample != 16
		) {
			printf("ERROR: Unsupported format\n");
			WAVE_Destroy(&WavData);
			return -1;
		}
	}

	//! Open target file
	FILE *VagFile = fopen(argv[2], "wb");
	if(!VagFile) {
		printf("ERROR: Unable to open VAG file\n");
		WAVE_Destroy(&WavData);
		return -1;
	}

	//! Try to get loop points from file and then sanitize
	int nSamples = WavData.data_Size / sizeof(int16_t);
	if(LoopBeg != -1 && LoopEnd == -1) {
		//! Fix implied loop end
		LoopEnd = nSamples;
	}
	ParseLoopPoints(&WavData, &LoopBeg, &LoopEnd);
	if(LoopBeg < 0 || LoopEnd-LoopBeg < 2 || LoopBeg >= nSamples || LoopEnd > nSamples) {
		printf("WARNING: Loop points are out of bounds. Ignoring...\n");
		LoopBeg = LoopEnd = -1;
	}
	if(LoopBeg == 0) {
		printf("NOTE: Loop begins at start of file. This may cause artifacts on looping.\n");
	}

	//! Handle resampling and loop adjustments
	ResampleFactor = DoResampling(&WavData, &LoopBeg, &LoopEnd, &nSamples, ResampleFactor, ResamplingType);
	if(ResampleFactor == 0.0) {
		//! Error messages already displayed; just exit
		fclose(VagFile);
		WAVE_Destroy(&WavData);
		return -1;
	}
	if(ResampleFactor != 1.0) {
		double ResampleCents = 0x1.B0CEFAA8EFD82p10*log(ResampleFactor); //! Cents = 12*100*Log2[ResampleFactor] = 12*100*Log2[E] * Log[ResampleFactor]
		printf("NOTE: Resampling took place at a ratio of %.5f (%.2f cents)\n", ResampleFactor, ResampleCents);
	}

	//! Write VAG header
	{
		//! This is a mashup of v2 and v3, which should hopefully
		//! be compatible with both. The idea is to have the sample
		//! data start at 40h (like in v3), by padding the bytes
		//! at 30h..3Fh with 0 (a silent block in v2) and then
		//! setting a larger WavSize to account for this in v2.
		//! NOTE: One-shot samples have an additional silent loop
		//! at the end, so include that in WavSize.
		struct {
			uint32_t Magic;
			uint32_t Version;
			uint32_t SrcOffs;
			uint32_t WavSize;
			uint32_t SampRate;
			uint8_t  r1[12]; //! Normally unused data? Volume, ADSR, etc. Reserved for v3 compatibility
			char     Name[16];
			char     r2[16]; //! Reserved for v3 compatibility
		} VAGp = {
			.Magic    = 0x70474156, //! "VAGp"
			.Version  = BIG32(2),
			.SrcOffs  = BIG32(0),
			.WavSize  = BIG32(16 + nSamples*sizeof(union SPUADPCM_Frame_t)/SPUADPCM_FRAME_LEN + 16*(LoopBeg == -1 /* || LoopEnd == -1*/)),
			.SampRate = BIG32(WavData.fmt->nSamplesPerSec),
		};
		if(TrackName) strncpy(VAGp.Name, TrackName, sizeof(VAGp.Name));
		fwrite(&VAGp, sizeof(VAGp), 1, VagFile);
	}

	//! Begin encoding and append a silent loop frame at the end as needed
	{
		uint32_t Frame, nFrames = nSamples / SPUADPCM_FRAME_LEN;
		union SPUADPCM_Frame_t FrameData;
		int LPCTap[2] = {0,0};
		const int16_t *SrcData = WavData.data;
		for(Frame=0;Frame<nFrames;Frame++) {
			//! NOTE: Flags are ORed in for the case of a one-frame loop.
			FrameData = SPUADPCM_Compress(SrcData, LPCTap);
			if( Frame   *SPUADPCM_FRAME_LEN == (uint32_t)LoopBeg) FrameData.u8[1] |= 0x04; //! LOOP_START
			if((Frame+1)*SPUADPCM_FRAME_LEN == (uint32_t)LoopEnd) FrameData.u8[1] |= 0x03; //! LOOP_END_REPT
			fwrite(&FrameData, sizeof(FrameData), 1, VagFile);
			SrcData += SPUADPCM_FRAME_LEN;
		}
		if(LoopBeg == -1 /* || LoopEnd == -1*/) {
			//! Append silent loop for one-shot samples
			memset(&FrameData, 0, sizeof(FrameData));
			FrameData.u8[1] = 0x05; //! LOOP_START|LOOP_END_MUTE
			fwrite(&FrameData, sizeof(FrameData), 1, VagFile);
		}
	}

	//! All done, close handles and exit
	printf("Ok.\n");
	fclose(VagFile);
	WAVE_Destroy(&WavData);
	return 0;
}

/**************************************/
//! EOF
/**************************************/
