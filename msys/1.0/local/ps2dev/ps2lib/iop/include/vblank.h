/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, David Ryan (Oobles@hotmail.com)
  ------------------------------------------------------------------------
  vblank.h
			IOP vblank library prototypes

  Looking at the vblank in the bios it starts up and registers two
  of its own vblank handlers that are used in subsequent calls to 
  WaitVblankStart and WaitVblankEnd.

  The First value of register vblank handler is either 0 or 1 depending
  on if you want the callback at the start or end of the vblank. I'm not
  sure which is which at the moment.

*/

#ifndef _VBLANK_H
#define _VBLANK_H

void WaitVblankStart();
void WaitVblankEnd();
void WaitVblank();
void WaitNonVblank();

int RegisterVblankHandler( int startend, int priority, void (*func)(void *), void * funcvalue );
int ReleaseVblankHandler( int a0, int a1 );

#endif

