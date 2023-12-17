/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#pragma once
/**************************************/
#include <stdint.h>
/**************************************/

#define WAVRESAMPLE_LINEAR 0
#define WAVRESAMPLE_CUBIC  1
#define WAVRESAMPLE_SINC   2
int16_t *WavResample(int16_t *Src, int nInSamples, int nOutSamples, double ResampleFactor, int ResamplingType, int LoopLen);

/**************************************/
//! EOF
/**************************************/
