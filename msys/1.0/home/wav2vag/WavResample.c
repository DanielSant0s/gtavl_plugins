/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
/**************************************/
#include "WavResample.h"
/**************************************/
#define CLIP16(x) (int16_t)(((x) < (-0x8000)) ? (-0x8000) : ((x) > (+0x7FFF)) ? (+0x7FFF) : (x))
#define FETCH(x) (((x) < 0) ? 0 : ((x) < nInSamples) ? Src[x] : LoopLen ? Src[(x)-LoopLen] : 0)
/**************************************/

#define SINC_WINDOW_FXP 15
static void Resample_GenerateSincWindow(int16_t *Dst, int nMin, int nMax, double xOfs, int Order, double Fc) {
	//! This is a Nuttall-windowed sinc window
	int n;
	for(n=nMin;n<=nMax;n++) {
		double Sinc; {
			double x = (n*Fc - xOfs) * M_PI;
			Sinc = x ? (sin(x) / x) : 1.0;
		}
		double Nuttall; {
			double x = n / (double)Order + 0.5;
			Nuttall  = 0.355768;
			Nuttall -= 0.487396*cos(x*2.0*M_PI);
			Nuttall += 0.144232*cos(x*4.0*M_PI);
			Nuttall -= 0.012604*cos(x*6.0*M_PI);
		}
		int v = lrint(Sinc*Fc * Nuttall * (1<<SINC_WINDOW_FXP));
		*Dst++ = CLIP16(v);
	}
}

/**************************************/

static void Resample_PreFilter(const int16_t *Src, int16_t *Dst, int nInSamples, double Ratio, int LoopLen) {
	//! This window is symmetric so we can halve the number of multiplies
	int n, k;
	const int LP_FIR_ORDER = 33, LP_FIR_HALF_ORDER = LP_FIR_ORDER/2; //! LP_FIR_ORDER must be odd
	int16_t LP_FIR[1+LP_FIR_HALF_ORDER];
	Resample_GenerateSincWindow(LP_FIR, 0, LP_FIR_HALF_ORDER, 0.0, LP_FIR_ORDER, Ratio);
	for(n=0;n<nInSamples;n++) {
		int s = LP_FIR[0] * FETCH(n);
		for(k=1;k<=LP_FIR_HALF_ORDER;k++) s += LP_FIR[k] * (FETCH(n-k) + FETCH(n+k));
		s = (s + (1<<SINC_WINDOW_FXP)/2 - (s<0)) >> SINC_WINDOW_FXP;
		Dst[n] = CLIP16(s);
	}
}

/**************************************/

static void Resample_Linear(const int16_t *Src, int nInSamples, int16_t *Dst, int nOutSamples, double Rate, int LoopLen) {
	int n, iSrc = 0;
	double mu = 0.0;
	for(n=0;n<nOutSamples;n++) {
		int x0 = FETCH(iSrc);
		int x1 = FETCH(iSrc+1);
		int v  = lrint(x0 + (x1-x0)*mu);
		*Dst++ = CLIP16(v);
		mu += Rate, iSrc += (int)mu, mu -= (int)mu;
	}
}

/**************************************/

static void Resample_Cubic(const int16_t *Src, int nInSamples, int16_t *Dst, int nOutSamples, double Rate, int LoopLen) {
	int n, iSrc = 0;
	double mu = 0.0;
	for(n=0;n<nOutSamples;n++) {
		//! Catmull-Rom spline seems to sound better than plain cubic spline
		//! NOTE: We can refactor this so that we pre-generate a 4-tap FIR
		//! filter window for a given fractional position accuracy, and this
		//! gives us a simple 4xMUL+3ADD (or 1xMUL+3FMA) version.
		int x0 = FETCH(iSrc-1);
		int x1 = FETCH(iSrc  );
		int x2 = FETCH(iSrc+1);
		int x3 = FETCH(iSrc+2);
		int a0 =         2*x1;
		int a1 = -1*x0        + 1*x2;
		int a2 =  2*x0 - 5*x1 + 4*x2 - x3;
		int a3 = -1*x0 + 3*x1 - 3*x2 + x3;
		int v = lrint((a0 + mu*(a1 + mu*(a2 + mu*(a3)))) * 0.5);
		*Dst++ = CLIP16(v);
		mu += Rate, iSrc += (int)mu, mu -= (int)mu;
	}
}

/**************************************/

static int Resample_Sinc(const int16_t *Src, int nInSamples, int16_t *Dst, int nOutSamples, double Rate, int LoopLen, double Ratio) {
	int n, k;

	//! Generate the sinc sliding window
	//! This window is NOT symmetric so we can't cheat :(
	const int LP_FIR_ORDER = 33, LP_FIR_HALF_ORDER = LP_FIR_ORDER/2; //! LP_FIR_ORDER must be odd
	const int FRAC_BITS = 12, FRAC_SCALE = 1<<FRAC_BITS;
	int16_t *SincWin = malloc(FRAC_SCALE * LP_FIR_ORDER*sizeof(int16_t));
	if(!SincWin) return 0;
	for(n=0;n<FRAC_SCALE;n++) {
		int16_t *Win = SincWin + n*LP_FIR_ORDER;
		Resample_GenerateSincWindow(Win, -LP_FIR_HALF_ORDER, +LP_FIR_HALF_ORDER, n * (1.0/FRAC_SCALE), LP_FIR_ORDER, (Ratio < 1.0) ? Ratio : 1.0);
	}

	//! Filter and then free the sinc window
	int iSrc = 0;
	double mu = 0.0;
	for(n=0;n<nOutSamples;n++) {
		const int16_t *Win = SincWin + (int)(mu*FRAC_SCALE)*LP_FIR_ORDER + LP_FIR_HALF_ORDER;
		int s = 0;
		for(k=-LP_FIR_HALF_ORDER;k<=LP_FIR_HALF_ORDER;k++) {
			s += Win[k] * FETCH(iSrc+k);
		}
		s = (s + (1<<SINC_WINDOW_FXP)/2 - (s<0)) >> SINC_WINDOW_FXP;
		*Dst++ = CLIP16(s);
		mu += Rate, iSrc += (int)mu, mu -= (int)mu;
	}
	free(SincWin);
	return 1;
}

/**************************************/

int16_t *WavResample(int16_t *Src, int nInSamples, int nOutSamples, double ResampleFactor, int ResamplingType, int LoopLen) {
	double Rate = 1.0 / ResampleFactor;
	int16_t *Dst = malloc(nOutSamples*sizeof(int16_t)); if(!Dst) return NULL;

	//! Sinc filtering is "special" because the prefilter is
	//! convolved with the filter window directly.
	if(ResamplingType == WAVRESAMPLE_SINC) {
		//! This one can fail to allocate the window buffer, so check return result
		if(!Resample_Sinc(Src, nInSamples, Dst, nOutSamples, Rate, LoopLen, ResampleFactor)) {
			free(Dst); Dst = NULL;
		}
		return Dst;
	}

	//! Apply pre-filter?
	int16_t *PreFiltBuf = NULL;
	if(ResampleFactor < 1.0) {
		PreFiltBuf = malloc(nInSamples*sizeof(int16_t));
		if(!PreFiltBuf) { free(Dst); return NULL; }
		Resample_PreFilter(Src, PreFiltBuf, nInSamples, ResampleFactor, LoopLen);
		Src = PreFiltBuf;
	}

	//! Do actual resampling
	switch(ResamplingType) {
#if 0 //! This one is already checked for
		case WAVRESAMPLE_SINC: {
		} break;
#endif
		case WAVRESAMPLE_CUBIC: {
			Resample_Cubic(Src, nInSamples, Dst, nOutSamples, Rate, LoopLen);
		} break;

		default:
		case WAVRESAMPLE_LINEAR: {
			Resample_Linear(Src, nInSamples, Dst, nOutSamples, Rate, LoopLen);
		} break;
	}
	free(PreFiltBuf);
	return Dst;
}

/**************************************/
//! EOF
/**************************************/
