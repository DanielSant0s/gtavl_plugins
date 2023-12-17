/**************************************/
//! Aikku93's wav2vag: WAV to VAG Conversion Tool
//! Copyright (C) 2021, Ruben Nunez (Aikku; aik AT aol DOT com DOT au)
//! Refer to the project README file for license terms.
/**************************************/
#pragma once
/**************************************/
#include <stdio.h>
#include <stdint.h>
/**************************************/

//! FourCC from string
#define RIFF_FOURCC(x0,x1,x2,x3) ((x0) | (x1)<<8 | (x2)<<16 | (x3)<<24)

//! RIFF chunk header structure
struct RIFF_CkHeader_t {
	uint32_t Type;
	uint32_t Size;
};

/**************************************/

//! RIFF chunk handler structure
//! Finish a list of these by appending (struct RIFF_CkHdl_t){.Type=0}
struct RIFF_CkHdl_t {
	uint32_t Type;
	int (*Func)(FILE *File, void *User, const struct RIFF_CkHeader_t *Ck);
};

//! RIFF 'LIST' chunk handler structure
//! Finish a list of these by appending (struct RIFF_CkListHdl_t){.Type=0}
struct RIFF_CkListHdl_t {
	uint32_t Type;
	const struct RIFF_CkHdl_t     *CkHdl;
	const struct RIFF_CkListHdl_t *ListHdl;
	int (*ListCbBeg)(FILE *File, void *User);
	int (*ListCbEnd)(FILE *File, void *User);
};

/**************************************/

//! RIFF_CkRead(File, User, CkHdl, ListHdl)
//! Description: Read RIFF chunk.
//! Arguments:
//!   File:    Source file.
//!   User:    Userdata to pass to handlers.
//!   CkHdl:   List of chunk handlers.
//!   ListHdl: List of RIFF/LIST handlers.
//! Returns:
//!   On no handler found, returns 0.
//!   Otherwise, returns the value of the last-handled chunk.
//!   When encounternig a RIFF/LIST chunk, execution stops when
//!   an associated handler returns a value less than 0.
int RIFF_CkRead(FILE *File, void *User, const struct RIFF_CkHdl_t *CkHdl, const struct RIFF_CkListHdl_t *ListHdl);

/**************************************/
//! EOF
/**************************************/
