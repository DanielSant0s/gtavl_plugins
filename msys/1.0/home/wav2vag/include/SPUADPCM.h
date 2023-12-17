/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#pragma once
/**************************************/
#include <stdint.h>
/**************************************/

//! 28 samples in every 16byte frame
#define SPUADPCM_FRAME_LEN 28u
union SPUADPCM_Frame_t {
	uint8_t  u8[16];
	uint16_t u16[8];
	uint32_t u32[4];
};

/**************************************/

static inline union SPUADPCM_Frame_t SPUADPCM_Compress(const int16_t *Src, int LPCTap[2]) {
	uint32_t n, Filter, Shift;
	int      BestTap[2] = {0,0};
	uint64_t BestError = ~0ULL;
	union SPUADPCM_Frame_t BestFrame = {0};
	for(Filter=0;Filter<5;Filter++) for(Shift=0;Shift<=12;Shift++) {
		int Tap[2] = {LPCTap[0],LPCTap[1]};
		uint64_t ThisError = 0;
		union SPUADPCM_Frame_t ThisFrame;
		ThisFrame.u32[0] = (12-Shift) | Filter<<4, ThisFrame.u32[1] = ThisFrame.u32[2] = ThisFrame.u32[3] = 0;
		for(n=0;n<SPUADPCM_FRAME_LEN;n++) {
			//! LPC filters provided by SPU (.6fxp)
			static const int LPC[5][2] = {
				{  0,  0},
				{ 60,  0},
				{115,-52},
				{ 98,-55},
				{122,-60},
			};

			//! Get prediction, form residue, quantize, clip, sum error, swap LPC taps
			//! NOTE: Prediction formula is based off a common-sense approach to the
			//! formula provided in the no$psx specification. The formula there uses
			//! division by 64, but this is likely to be a bit-shift right instead, as
			//! then the rounding term actually makes sense, as its the most common
			//! approach towards rounding (even if it's off by 1 for negative values).
			int x = Src[n];
			int p = (Tap[0]*LPC[Filter][0] + Tap[1]*LPC[Filter][1] + 32) >> 6;
			int r = x - p;
			int q = (r + (((1<<Shift) - (r < 0)) >> 1)) >> Shift; //! Round[r / 2^Shift]. Looks janky but it's correct
			    q = (q < -8) ? (-8) : (q > +7) ? (+7) : q;
			int y = p + (q << Shift); y = (y < -0x8000) ? (-0x8000) : (y > +0x7FFF) ? (+0x7FFF) : y;
			int e = y - x;
			ThisFrame.u32[(n+4)/8] |= (q&0xF) << (((n+4)%8)*4);
			ThisError += (uint64_t)e*e;
			Tap[1] = Tap[0], Tap[0] = y;
		}
		if(ThisError < BestError) {
			BestTap[0] = Tap[0], BestTap[1] = Tap[1];
			BestError = ThisError;
			BestFrame = ThisFrame;
		}
	}
	LPCTap[0] = BestTap[0], LPCTap[1] = BestTap[1];
	return BestFrame;
}

/**************************************/
//! EOF
/**************************************/
