/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2005, ps2dev - http://www.ps2dev.org
# Licenced under GNU Library General Public License version 2
#
# $Id: upsamplers.c 1136 2005-06-10 08:58:54Z gawd $
# audsrv IOP-side upsamplers
*/

#include <stdio.h>
#include "upsamplers.h"

/**
 * \file upsamplers.c
 * \author gawd (Gil Megidish)
 * \date 04-24-05
 */

/** lut for converting 8000 -> 48000 */
static const short up_8000_lut[512] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x11,
	0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12,
	0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
	0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x15, 0x15,
	0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16,
	0x16, 0x16, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19,
	0x19, 0x19, 0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x1a,
	0x1a, 0x1a, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
	0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1d, 0x1d,
	0x1d, 0x1d, 0x1d, 0x1d, 0x1e, 0x1e, 0x1e, 0x1e,
	0x1e, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x21,
	0x21, 0x21, 0x21, 0x21, 0x22, 0x22, 0x22, 0x22,
	0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23,
	0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x25, 0x25,
	0x25, 0x25, 0x25, 0x25, 0x26, 0x26, 0x26, 0x26,
	0x26, 0x26, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27,
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x29, 0x29,
	0x29, 0x29, 0x29, 0x29, 0x2a, 0x2a, 0x2a, 0x2a,
	0x2a, 0x2a, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b,
	0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2d, 0x2d,
	0x2d, 0x2d, 0x2d, 0x2d, 0x2e, 0x2e, 0x2e, 0x2e,
	0x2e, 0x2e, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31,
	0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32, 0x32,
	0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x35, 0x35,
	0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39,
	0x39, 0x39, 0x39, 0x39, 0x3a, 0x3a, 0x3a, 0x3a,
	0x3a, 0x3a, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b,
	0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3d, 0x3d,
	0x3d, 0x3d, 0x3d, 0x3d, 0x3e, 0x3e, 0x3e, 0x3e,
	0x3e, 0x3e, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
	0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43,
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 0x45,
	0x45, 0x45, 0x45, 0x45, 0x46, 0x46, 0x46, 0x46,
	0x46, 0x46, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47,
	0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x49, 0x49,
	0x49, 0x49, 0x49, 0x49, 0x4a, 0x4a, 0x4a, 0x4a,
	0x4a, 0x4a, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b,
	0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4d, 0x4d,
	0x4d, 0x4d, 0x4d, 0x4d, 0x4e, 0x4e, 0x4e, 0x4e,
	0x4e, 0x4e, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f,
	0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x51, 0x51,
	0x51, 0x51, 0x51, 0x51, 0x52, 0x52, 0x52, 0x52,
	0x52, 0x52, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
	0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x55, 0x55, 
};

/** lut for converting 11025 -> 48000 */
static const short up_11025_lut[512] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
	0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03,
	0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x07,
	0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0d, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x10, 0x10,
	0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x12,
	0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13,
	0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
	0x16, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17,
	0x17, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19,
	0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x1a, 0x1b, 0x1b,
	0x1b, 0x1b, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1d,
	0x1d, 0x1d, 0x1d, 0x1e, 0x1e, 0x1e, 0x1e, 0x1f,
	0x1f, 0x1f, 0x1f, 0x1f, 0x20, 0x20, 0x20, 0x20,
	0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x22, 0x22,
	0x22, 0x23, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24,
	0x24, 0x24, 0x25, 0x25, 0x25, 0x25, 0x26, 0x26,
	0x26, 0x26, 0x27, 0x27, 0x27, 0x27, 0x27, 0x28,
	0x28, 0x28, 0x28, 0x29, 0x29, 0x29, 0x29, 0x2a,
	0x2a, 0x2a, 0x2a, 0x2a, 0x2b, 0x2b, 0x2b, 0x2b,
	0x2c, 0x2c, 0x2c, 0x2c, 0x2d, 0x2d, 0x2d, 0x2d,
	0x2d, 0x2e, 0x2e, 0x2e, 0x2e, 0x2f, 0x2f, 0x2f,
	0x2f, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31,
	0x31, 0x31, 0x32, 0x32, 0x32, 0x32, 0x32, 0x33,
	0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x34, 0x35,
	0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36,
	0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x38,
	0x38, 0x39, 0x39, 0x39, 0x39, 0x3a, 0x3a, 0x3a,
	0x3a, 0x3b, 0x3b, 0x3b, 0x3b, 0x3b, 0x3c, 0x3c,
	0x3c, 0x3c, 0x3d, 0x3d, 0x3d, 0x3d, 0x3e, 0x3e,
	0x3e, 0x3e, 0x3e, 0x3f, 0x3f, 0x3f, 0x3f, 0x40,
	0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x41,
	0x42, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43,
	0x43, 0x44, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45,
	0x45, 0x46, 0x46, 0x46, 0x46, 0x46, 0x47, 0x47,
	0x47, 0x47, 0x48, 0x48, 0x48, 0x48, 0x49, 0x49,
	0x49, 0x49, 0x49, 0x4a, 0x4a, 0x4a, 0x4a, 0x4b,
	0x4b, 0x4b, 0x4b, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c,
	0x4d, 0x4d, 0x4d, 0x4d, 0x4e, 0x4e, 0x4e, 0x4e,
	0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x50, 0x50, 0x50,
	0x50, 0x51, 0x51, 0x51, 0x51, 0x51, 0x52, 0x52,
	0x52, 0x52, 0x53, 0x53, 0x53, 0x53, 0x54, 0x54,
	0x54, 0x54, 0x54, 0x55, 0x55, 0x55, 0x55, 0x56,
	0x56, 0x56, 0x56, 0x57, 0x57, 0x57, 0x57, 0x57,
	0x58, 0x58, 0x58, 0x58, 0x59, 0x59, 0x59, 0x59,
	0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5b, 0x5b, 0x5b,
	0x5b, 0x5c, 0x5c, 0x5c, 0x5c, 0x5d, 0x5d, 0x5d,
	0x5d, 0x5d, 0x5e, 0x5e, 0x5e, 0x5e, 0x5f, 0x5f,
	0x5f, 0x5f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x61,
	0x61, 0x61, 0x61, 0x62, 0x62, 0x62, 0x62, 0x62,
	0x63, 0x63, 0x63, 0x63, 0x64, 0x64, 0x64, 0x64,
	0x65, 0x65, 0x65, 0x65, 0x65, 0x66, 0x66, 0x66,
	0x66, 0x67, 0x67, 0x67, 0x67, 0x68, 0x68, 0x68,
	0x68, 0x68, 0x69, 0x69, 0x69, 0x69, 0x6a, 0x6a,
	0x6a, 0x6a, 0x6b, 0x6b, 0x6b, 0x6b, 0x6b, 0x6c,
	0x6c, 0x6c, 0x6c, 0x6d, 0x6d, 0x6d, 0x6d, 0x6e,
	0x6e, 0x6e, 0x6e, 0x6e, 0x6f, 0x6f, 0x6f, 0x6f,
	0x70, 0x70, 0x70, 0x70, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x72, 0x72, 0x72, 0x72, 0x73, 0x73, 0x73,
	0x73, 0x73, 0x74, 0x74, 0x74, 0x74, 0x75, 0x75, 
};

/** lut for converting 22050 -> 48000 */
static const short up_22050_lut[512] =
{
	0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03,
	0x03, 0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x06,
	0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x0a, 0x0a,
	0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0d, 0x0d, 0x0e,
	0x0e, 0x0f, 0x0f, 0x10, 0x10, 0x10, 0x11, 0x11,
	0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15,
	0x16, 0x16, 0x16, 0x17, 0x17, 0x18, 0x18, 0x19,
	0x19, 0x1a, 0x1a, 0x1b, 0x1b, 0x1c, 0x1c, 0x1c,
	0x1d, 0x1d, 0x1e, 0x1e, 0x1f, 0x1f, 0x20, 0x20,
	0x21, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24,
	0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27, 0x27,
	0x28, 0x28, 0x29, 0x29, 0x2a, 0x2a, 0x2b, 0x2b,
	0x2c, 0x2c, 0x2d, 0x2d, 0x2d, 0x2e, 0x2e, 0x2f,
	0x2f, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x32,
	0x33, 0x33, 0x34, 0x34, 0x35, 0x35, 0x36, 0x36,
	0x37, 0x37, 0x38, 0x38, 0x38, 0x39, 0x39, 0x3a,
	0x3a, 0x3b, 0x3b, 0x3c, 0x3c, 0x3d, 0x3d, 0x3e,
	0x3e, 0x3e, 0x3f, 0x3f, 0x40, 0x40, 0x41, 0x41,
	0x42, 0x42, 0x43, 0x43, 0x43, 0x44, 0x44, 0x45,
	0x45, 0x46, 0x46, 0x47, 0x47, 0x48, 0x48, 0x49,
	0x49, 0x49, 0x4a, 0x4a, 0x4b, 0x4b, 0x4c, 0x4c,
	0x4d, 0x4d, 0x4e, 0x4e, 0x4f, 0x4f, 0x4f, 0x50,
	0x50, 0x51, 0x51, 0x52, 0x52, 0x53, 0x53, 0x54,
	0x54, 0x54, 0x55, 0x55, 0x56, 0x56, 0x57, 0x57,
	0x58, 0x58, 0x59, 0x59, 0x5a, 0x5a, 0x5a, 0x5b,
	0x5b, 0x5c, 0x5c, 0x5d, 0x5d, 0x5e, 0x5e, 0x5f,
	0x5f, 0x60, 0x60, 0x60, 0x61, 0x61, 0x62, 0x62,
	0x63, 0x63, 0x64, 0x64, 0x65, 0x65, 0x65, 0x66,
	0x66, 0x67, 0x67, 0x68, 0x68, 0x69, 0x69, 0x6a,
	0x6a, 0x6b, 0x6b, 0x6b, 0x6c, 0x6c, 0x6d, 0x6d,
	0x6e, 0x6e, 0x6f, 0x6f, 0x70, 0x70, 0x71, 0x71,
	0x71, 0x72, 0x72, 0x73, 0x73, 0x74, 0x74, 0x75,
	0x75, 0x76, 0x76, 0x76, 0x77, 0x77, 0x78, 0x78,
	0x79, 0x79, 0x7a, 0x7a, 0x7b, 0x7b, 0x7c, 0x7c,
	0x7c, 0x7d, 0x7d, 0x7e, 0x7e, 0x7f, 0x7f, 0x80,
	0x80, 0x81, 0x81, 0x82, 0x82, 0x82, 0x83, 0x83,
	0x84, 0x84, 0x85, 0x85, 0x86, 0x86, 0x87, 0x87,
	0x87, 0x88, 0x88, 0x89, 0x89, 0x8a, 0x8a, 0x8b,
	0x8b, 0x8c, 0x8c, 0x8d, 0x8d, 0x8d, 0x8e, 0x8e,
	0x8f, 0x8f, 0x90, 0x90, 0x91, 0x91, 0x92, 0x92,
	0x93, 0x93, 0x93, 0x94, 0x94, 0x95, 0x95, 0x96,
	0x96, 0x97, 0x97, 0x98, 0x98, 0x98, 0x99, 0x99,
	0x9a, 0x9a, 0x9b, 0x9b, 0x9c, 0x9c, 0x9d, 0x9d,
	0x9e, 0x9e, 0x9e, 0x9f, 0x9f, 0xa0, 0xa0, 0xa1,
	0xa1, 0xa2, 0xa2, 0xa3, 0xa3, 0xa3, 0xa4, 0xa4,
	0xa5, 0xa5, 0xa6, 0xa6, 0xa7, 0xa7, 0xa8, 0xa8,
	0xa9, 0xa9, 0xa9, 0xaa, 0xaa, 0xab, 0xab, 0xac,
	0xac, 0xad, 0xad, 0xae, 0xae, 0xaf, 0xaf, 0xaf,
	0xb0, 0xb0, 0xb1, 0xb1, 0xb2, 0xb2, 0xb3, 0xb3,
	0xb4, 0xb4, 0xb4, 0xb5, 0xb5, 0xb6, 0xb6, 0xb7,
	0xb7, 0xb8, 0xb8, 0xb9, 0xb9, 0xba, 0xba, 0xba,
	0xbb, 0xbb, 0xbc, 0xbc, 0xbd, 0xbd, 0xbe, 0xbe,
	0xbf, 0xbf, 0xc0, 0xc0, 0xc0, 0xc1, 0xc1, 0xc2,
	0xc2, 0xc3, 0xc3, 0xc4, 0xc4, 0xc5, 0xc5, 0xc5,
	0xc6, 0xc6, 0xc7, 0xc7, 0xc8, 0xc8, 0xc9, 0xc9,
	0xca, 0xca, 0xcb, 0xcb, 0xcb, 0xcc, 0xcc, 0xcd,
	0xcd, 0xce, 0xce, 0xcf, 0xcf, 0xd0, 0xd0, 0xd1,
	0xd1, 0xd1, 0xd2, 0xd2, 0xd3, 0xd3, 0xd4, 0xd4,
	0xd5, 0xd5, 0xd6, 0xd6, 0xd6, 0xd7, 0xd7, 0xd8,
	0xd8, 0xd9, 0xd9, 0xda, 0xda, 0xdb, 0xdb, 0xdc,
	0xdc, 0xdc, 0xdd, 0xdd, 0xde, 0xde, 0xdf, 0xdf,
	0xe0, 0xe0, 0xe1, 0xe1, 0xe2, 0xe2, 0xe2, 0xe3,
	0xe3, 0xe4, 0xe4, 0xe5, 0xe5, 0xe6, 0xe6, 0xe7,
	0xe7, 0xe7, 0xe8, 0xe8, 0xe9, 0xe9, 0xea, 0xea, 
};

/** lut for converting 44100 -> 48000 */
static const short up_44100_lut[512] =
{
	0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006,
	0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000b, 0x000c, 0x000d,
	0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015,
	0x0016, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x001c,
	0x001d, 0x001e, 0x001f, 0x0020, 0x0021, 0x0021, 0x0022, 0x0023,
	0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x002b,
	0x002c, 0x002d, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031, 0x0032,
	0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0038, 0x0039,
	0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f, 0x0040, 0x0041,
	0x0042, 0x0043, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048,
	0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f, 0x004f,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005a, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e,
	0x005f, 0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0065,
	0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d,
	0x006e, 0x006f, 0x0070, 0x0071, 0x0071, 0x0072, 0x0073, 0x0074,
	0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007a, 0x007b, 0x007c,
	0x007c, 0x007d, 0x007e, 0x007f, 0x0080, 0x0081, 0x0082, 0x0083,
	0x0084, 0x0085, 0x0086, 0x0087, 0x0087, 0x0088, 0x0089, 0x008a,
	0x008b, 0x008c, 0x008d, 0x008e, 0x008f, 0x0090, 0x0091, 0x0092,
	0x0093, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099,
	0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009e, 0x009f, 0x00a0,
	0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8,
	0x00a9, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
	0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b4, 0x00b5, 0x00b6,
	0x00b7, 0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be,
	0x00bf, 0x00c0, 0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5,
	0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cb, 0x00cc,
	0x00cd, 0x00ce, 0x00cf, 0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4,
	0x00d5, 0x00d6, 0x00d6, 0x00d7, 0x00d8, 0x00d9, 0x00da, 0x00db,
	0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e2,
	0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea,
	0x00eb, 0x00ec, 0x00ed, 0x00ed, 0x00ee, 0x00ef, 0x00f0, 0x00f1,
	0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, 0x00f8, 0x00f8,
	0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff, 0x0100,
	0x0101, 0x0102, 0x0103, 0x0104, 0x0104, 0x0105, 0x0106, 0x0107,
	0x0108, 0x0109, 0x010a, 0x010b, 0x010c, 0x010d, 0x010e, 0x010f,
	0x010f, 0x0110, 0x0111, 0x0112, 0x0113, 0x0114, 0x0115, 0x0116,
	0x0117, 0x0118, 0x0119, 0x011a, 0x011a, 0x011b, 0x011c, 0x011d,
	0x011e, 0x011f, 0x0120, 0x0121, 0x0122, 0x0123, 0x0124, 0x0125,
	0x0126, 0x0126, 0x0127, 0x0128, 0x0129, 0x012a, 0x012b, 0x012c,
	0x012d, 0x012e, 0x012f, 0x0130, 0x0131, 0x0131, 0x0132, 0x0133,
	0x0134, 0x0135, 0x0136, 0x0137, 0x0138, 0x0139, 0x013a, 0x013b,
	0x013c, 0x013c, 0x013d, 0x013e, 0x013f, 0x0140, 0x0141, 0x0142,
	0x0143, 0x0144, 0x0145, 0x0146, 0x0147, 0x0147, 0x0148, 0x0149,
	0x014a, 0x014b, 0x014c, 0x014d, 0x014e, 0x014f, 0x0150, 0x0151,
	0x0152, 0x0153, 0x0153, 0x0154, 0x0155, 0x0156, 0x0157, 0x0158,
	0x0159, 0x015a, 0x015b, 0x015c, 0x015d, 0x015e, 0x015e, 0x015f,
	0x0160, 0x0161, 0x0162, 0x0163, 0x0164, 0x0165, 0x0166, 0x0167,
	0x0168, 0x0169, 0x0169, 0x016a, 0x016b, 0x016c, 0x016d, 0x016e,
	0x016f, 0x0170, 0x0171, 0x0172, 0x0173, 0x0174, 0x0175, 0x0175,
	0x0176, 0x0177, 0x0178, 0x0179, 0x017a, 0x017b, 0x017c, 0x017d,
	0x017e, 0x017f, 0x0180, 0x0180, 0x0181, 0x0182, 0x0183, 0x0184,
	0x0185, 0x0186, 0x0187, 0x0188, 0x0189, 0x018a, 0x018b, 0x018b,
	0x018c, 0x018d, 0x018e, 0x018f, 0x0190, 0x0191, 0x0192, 0x0193,
	0x0194, 0x0195, 0x0196, 0x0197, 0x0197, 0x0198, 0x0199, 0x019a,
	0x019b, 0x019c, 0x019d, 0x019e, 0x019f, 0x01a0, 0x01a1, 0x01a2,
	0x01a2, 0x01a3, 0x01a4, 0x01a5, 0x01a6, 0x01a7, 0x01a8, 0x01a9,
	0x01aa, 0x01ab, 0x01ac, 0x01ad, 0x01ad, 0x01ae, 0x01af, 0x01b0,
	0x01b1, 0x01b2, 0x01b3, 0x01b4, 0x01b5, 0x01b6, 0x01b7, 0x01b8,
	0x01b9, 0x01b9, 0x01ba, 0x01bb, 0x01bc, 0x01bd, 0x01be, 0x01bf,
	0x01c0, 0x01c1, 0x01c2, 0x01c3, 0x01c4, 0x01c4, 0x01c5, 0x01c6,
	0x01c7, 0x01c8, 0x01c9, 0x01ca, 0x01cb, 0x01cc, 0x01cd, 0x01ce,
	0x01cf, 0x01cf, 0x01d0, 0x01d1, 0x01d2, 0x01d3, 0x01d4, 0x01d5, 
};

/** Demux, split interleaved audio to left and right
    @param mux   source buffer
    @param left  left channel destination buffer
    @param right right channel destination buffer
    @param len   number of samples to copy
*/
static void demux(short *mux, short *left, short *right, int len)
{
	while (len > 0)
	{
		*left++ = *mux++;
		*right++ = *mux++;
		len--;
	}
}

static int up_12000_16_stereo(struct upsample_t *up)
{
	int p;
	short *left = up->left;
	short *right = up->right;
	short *src = (short *)up->src;

	for (p=0; p<128; p++)
	{
		*left++ = src[0];
		*right++ = src[1];
		*left++ = src[0];
		*right++ = src[1];
		*left++ = src[0];
		*right++ = src[1];
		*left++ = *src++;
		*right++ = *src++;
	}	

	return 512;
}

static int up_24000_16_stereo(struct upsample_t *up)
{
	int p;
	short *left = up->left;
	short *right = up->right;
	short *src = (short *)up->src;

	for (p=0; p<256; p++)
	{
		*left++ = src[0];
		*right++ = src[1];
		*left++ = *src++;
		*right++ = *src++;
	}

	return 1024;
}

static void up_generic_16_stereo(struct upsample_t *up, const unsigned short *lut)
{
	int q, p;
	short *left = up->left;
	short *right = up->right;
	short *src = (short *)up->src;

	for (p=0; p<512; p++)
	{
		q = lut[p] << 1;
		*left++ = src[q + 0];
		*right++ = src[q + 1];
	}	
}

static void up_generic_16_mono(struct upsample_t *up, const unsigned short *lut)
{
	int q, p;
	short *left = up->left;
	short *right = up->right;
	short *src = (short *)up->src;

	for (p=0; p<512; p++)
	{
		q = lut[p];
		*left++ = *right++ = src[q];
	}	
}

static void up_generic_8_mono(struct upsample_t *up, const unsigned short *lut)
{
	int q, p;
	signed char b;
	short *left = up->left;
	short *right = up->right;
	char *src = (char *)up->src;

	for (p=0; p<512; p++)
	{
		q = lut[p];
		b = src[q];
		*left++ = *right++ = (short)((b + (b << 8)) - 32768);
	}	
}

static void up_generic_8_stereo(struct upsample_t *up, const unsigned short *lut)
{
	int q, p;
	signed char b;
	short *left = up->left;
	short *right = up->right;
	char *src = (char *)up->src;

	for (p=0; p<512; p++)
	{
		q = lut[p] << 1;
		b = src[q + 0];
		*left++ = (short)((b + (b << 8)) - 32768);
		b = src[q + 1];
		*right++ = (short)((b + (b << 8)) - 32768);
	}	
}

static int up_11025_8_mono(struct upsample_t *up)
{
	up_generic_8_mono(up, up_11025_lut);
	return 116; /* (11025 / 48000) * 512 */
}

static int up_11025_8_stereo(struct upsample_t *up)
{
	up_generic_8_stereo(up, up_11025_lut);
	return 234; /* (11025 / 48000) * 512 * stereo */
}

static int up_11025_16_mono(struct upsample_t *up)
{
	up_generic_16_mono(up, up_11025_lut);
	return 234; /* (11025 / 48000) * 512 * 16bit */
}

static int up_11025_16_stereo(struct upsample_t *up)
{
	up_generic_16_stereo(up, up_11025_lut);
	return 470; /* (11025 / 48000) * 512 * stereo * 16bit */
}

static int up_22050_8_mono(struct upsample_t *up)
{
	up_generic_8_mono(up, up_22050_lut);
	return 235; /* (22050 / 48000) * 512 */
}

static int up_22050_16_mono(struct upsample_t *up)
{
	up_generic_16_mono(up, up_22050_lut);
	return 470; /* (22050 / 48000) * 512 * 2 */
}

static int up_22050_16_stereo(struct upsample_t *up)
{
	up_generic_16_stereo(up, up_22050_lut);
	return 940; /* (22050 / 48000) * 512 * stereo * 16bit */
}

static int up_44100_8_mono(struct upsample_t *up)
{
	up_generic_8_mono(up, up_44100_lut);
	return 470; /* (44100 / 48000) * 512 */
}

static int up_44100_16_mono(struct upsample_t *up)
{
	up_generic_16_mono(up, up_44100_lut);
	return 940; /* (44100 / 48000) * 512 * 16bit */
}

static int up_44100_16_stereo(struct upsample_t *up)
{
	up_generic_16_stereo(up, up_44100_lut);
	return 1880; /* (44100 / 48000) * 512 * stereo * 16bit */
}

static int up_48000_16_mono(struct upsample_t *up)
{
	int i;
	short *left = (short *)up->left;
	short *right = (short *)up->right;
	short *src = (short *)up->src;

	for (i=0; i<512; i++)
	{
		*left++ = *right++ = *src++;
	}

	return 1024; /* 512 * 16bit */
}

static int up_48000_16_stereo(struct upsample_t *up)
{
	/* simple demux */
	demux((short *)up->src, up->left, up->right, 512);
	return 2048; /* 512 * stereo * 16bit */
}

typedef struct entry_t
{
	int freq;                       ///< source frequency
	int bits;                       ///< source sample bit count
	int channels;                   ///< source number of channels
	upsampler_t func;               ///< upsamplers to convert to native
} entry_t;

/** supported upsamplers */
static entry_t upsamplers[] = 
{
	{11025,  8, 1, up_11025_8_mono},
	{11025,  8, 2, up_11025_8_stereo},
	{11025, 16, 1, up_11025_16_mono},
	{11025, 16, 2, up_11025_16_stereo},
	{12000, 16, 2, up_12000_16_stereo},
	{22050,  8, 1, up_22050_8_mono},
	{22050, 16, 1, up_22050_16_mono},
	{22050, 16, 2, up_22050_16_stereo},
	{24000, 16, 2, up_24000_16_stereo},
	{44100,  8, 1, up_44100_8_mono},
	{44100, 16, 1, up_44100_16_mono},
	{44100, 16, 2, up_44100_16_stereo},
	{48000, 16, 1, up_48000_16_mono},
	{48000, 16, 2, up_48000_16_stereo},
	{0, 0, 0, 0}
};

/** Returns an upsampler from a specified format to SPU2's native
    @param freq      frequency used
    @param bits      bits per sample
    @param channels  number of audio channels 
    @returns a function to be used for upsampling, null if not found

    Will return a function to convert source audio to Playstation's native.
    Note a very limited selection of upsamplers are available, only the most 
    commonly used.
*/
upsampler_t find_upsampler(int freq, int bits, int channels)
{
	struct entry_t *p = upsamplers;
	while (p->func != NULL)
	{
		if (p->freq == freq && p->bits == bits && p->channels == channels)
		{
			/* found us an upsampler */
			return p->func;
		}

		p++;
	}

	/* no more upsamplers */
	return NULL;
}

