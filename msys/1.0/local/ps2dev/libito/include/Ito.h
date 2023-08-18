#ifndef _ITO_H
#define _ITO_H

#include <ItoTypes.h>
#include <LinkedList.h>
#include <DmaDefs.h>
#include <GsDefs.h>
#include <Gs.h>
#include <Vu.h>
#include <Fpu.h>
#include <FpuMath.h>
#include <Vu0Math.h>
#include <GifPacket.h>
#include <GsDriver.h>
#include <GifPipe.h>
#include <TextureManager.h>
#include <Pad.h>
#include <Sema.h>
#include <Thread.h>

/** \mainpage 

- <b>New in ito 1.0.0</b>
 - Completely new framework in C++

*/

/** Ito Main Ito Namespace */
namespace Ito
{	
	extern "C" void EnableScratchpadStack();
	extern "C" void DisableScratchpadStack();
}


#endif

