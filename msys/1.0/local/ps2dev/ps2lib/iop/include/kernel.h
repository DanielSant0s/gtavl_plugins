

#ifndef IOP_KERNEL_H
#define IOP_KERNEL_H

#include "sifrpc.h"

/*
   Functions from LOADCORE

   Moved to loadcore.h  and include file added here
   to ensure old code doesn't break;
*/
#include <loadcore.h>



#define PUSHDATA( t, x, v, l) \
   *(t *)x = v; l = sizeof( t )

#define POPDATA( t, x, v, l ) \
   v = *(t *)x; l = sizeof( t )

/*
   Functions from THSEMAP

   Moved to thsemap.h and include file added here
   to ensure old code doesn't break. 
*/
#include <thsemap.h>


/* 
   intrman calls 

   Moved to intrman.h  and include file added here
   to ensure old code doesn't break.
*/
#include <intrman.h>


/*
   thbase functions

   Function have been removed from here and added to thbase.h
   Header file included here to make sure nothing breaks.
*/
#include <thbase.h>


/*
   sifcmd functions

   Function have been removed from here and added to sifcmd.h
   Header file included here to make sure nothing breaks.
*/
#include <sifcmd.h>


#endif
