/*
Sparky writes in the forums:

The Victor Suba code is an extremely fancy tool that supports swizzling from any GS layout to any GS layout.

Since the only swizzling you typically want to do is 8to32, 4to32 and 16to32 I've made much simpler functions to do just this for you. 
They're like memcopy functions made slightly more complicated. 
Very easy to grasp and they don't require any additional memory though the input texels and output texels may not overlap, enjoy.

*/

#include "types.h"
#include "swizzle.h"


///////// swizzlers /////////

void Swizzle8to32(void * pSwizTexels, const void * pInTexels, const int32 width,
      const int32 height)
{
   // this function works for the following resolutions
   // Width:       any multiple of 16 smaller then or equal to 4096
   // Height:      any multiple of 4 smaller then or equal to 4096

   // the texels must be uploaded as a 32bit texture
   // width_32bit = width_8bit / 2
   // height_32bit = height_8bit / 2
   // remember to adjust the mapping coordinates when
   // using a dimension which is not a power of two

   for(int y=0; y<height; y++)
      for(int x=0; x<width; x++)
      {
         const uint8 uPen = ((const uint8 *) pInTexels)[y*width+x];

         const int32 block_location = (y&(~0xf))*width + (x&(~0xf))*2;
         const uint32 swap_selector = (((y+2)>>2)&0x1)*4;
         const int32 posY = (((y&(~3))>>1) + (y&1))&0x7;
         const int32 column_location = posY*width*2 + ((x+swap_selector)&0x7)*4;

         const int32 byte_num = ((y>>1)&1) + ((x>>2)&2);     // 0,1,2,3

         ((uint8 *) pSwizTexels)[block_location + column_location + byte_num] = uPen;
      }
}

void Swizzle4to32(void * pSwizTexels, const void * pInTexels, const int32 width,
      const int32 height)
{
   // this function works for the following resolutions
   // Width:       32, 64, 96, 128, any multiple of 128 smaller then or equal to 4096
   // Height:      16, 32, 48, 64, 80, 96, 112, 128, any multiple of 128 smaller then or equal to 4096

   // the texels must be uploaded as a 32bit texture
   // width_32bit = height_4bit / 2
   // height_32bit = width_4bit / 4
   // remember to adjust the mapping coordinates when
   // using a dimension which is not a power of two

   for(int y=0; y<height; y++)
      for(int x=0; x<width; x++)
      {
         // get the pen
         const int32 index = y*width+x;
         const uint8 uPen = (((const uint8
                     *) pInTexels)[(index>>1)]>>((index&1)*4))&0xf;

         // swizzle
         const int32 pageX = x & (~0x7f);
         const int32 pageY = y & (~0x7f);

         const int32 pages_horz = (width+127)/128;
         const int32 pages_vert = (height+127)/128;

         const int32 page_number = (pageY/128)*pages_horz + (pageX/128);

         const int32 page32Y = (page_number/pages_vert)*32;
         const int32 page32X = (page_number%pages_vert)*64;

         const int32 page_location = page32Y*height*2 + page32X*4;

         const int32 locX = x & 0x7f;
         const int32 locY = y & 0x7f;

         const int32 block_location = ((locX&(~0x1f))>>1)*height + (locY&(~0xf))*2;
         const uint32 swap_selector = (((y+2)>>2)&0x1)*4;
         const int32 posY = (((y&(~3))>>1) + (y&1))&0x7;

         const int32 column_location = posY*height*2 + ((x+swap_selector)&0x7)*4;

         const int32 byte_num = (x>>3)&3;     // 0,1,2,3
         const int32 bits_set = (y>>1)&1;     // 0,1            (lower/upper 4 bits)

         uint8 &setPix = ((uint8 *) pSwizTexels)[page_location + block_location +
            column_location + byte_num];
         setPix = (setPix & (-bits_set)) | (uPen<<(bits_set*4));
      }
}


void Swizzle16to32(void * pSwizTexels, const void * pInTexels, const int32 width,
      const int32 height)
{
   // this function works for the following resolutions
   // Width:       16, 32, 48, 64, any multiple of 64 smaller then or equal to 4096
   // Height:      8, 16, 24, 32, 40, 48, 56, 64, any multiple of 64 smaller then or equal to 4096

   // the texels must be uploaded as a 32bit texture
   // width_32bit = height_16bit
   // height_32bit = width_16bit / 2
   // remember to adjust the mapping coordinates when
   // using a dimension which is not a power of two

   for(int y=0; y<height; y++)
      for(int x=0; x<width; x++)
      {
         const uint16 uCol = ((const uint16 *) pInTexels)[y*width+x];

         const int32 pageX = x & (~0x3f);
         const int32 pageY = y & (~0x3f);

         const int32 pages_horz = (width+63)/64;
         const int32 pages_vert = (height+63)/64;

         const int32 page_number = (pageY/64)*pages_horz + (pageX/64);

         const int32 page32Y = (page_number/pages_vert)*32;
         const int32 page32X = (page_number%pages_vert)*64;

         const int32 page_location = (page32Y*height + page32X)*2;

         const int32 locX = x & 0x3f;
         const int32 locY = y & 0x3f;

         const int32 block_location = (locX&(~0xf))*height + (locY&(~0x7))*2;
         const int32 column_location = ((y&0x7)*height + (x&0x7))*2;

         const int32 short_num = (x>>3)&1;       // 0,1

         ((short *) pSwizTexels)[page_location + block_location + column_location
            + short_num] = uCol;
      }
}

