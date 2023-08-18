/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, Florin Sasu (florinsasu@yahoo.com)
  			   (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  modload.h
                           IOP modload library prototypes
*/

#ifndef __MODLOAD_H__
#define __MODLOAD_H__

#define MODLOAD_VER	0x101

void ReBootStart(int *, int);

int LoadStartModule(const char *name, int arglen, const char *arg, int *res);
int LoadModuleBuffer(void *modbuf);	//	returns module id
int StartModule(int modid, char *argv0, int arglen, char *arg, int *modres);


#endif//__MODLOAD_H__
