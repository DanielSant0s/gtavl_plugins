/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, David Ryan (Oobles@hotmail.com)
  ------------------------------------------------------------------------
  thsemap.h
			IOP thsemap library prototypes

*/

#ifndef __THSEMAP_H__
#define __THSEMAP_H__


struct t_sema
{
   unsigned int attr;   // in usbd.irx set to 1
   unsigned int option;         // in usbd.irx set to 0
   int init_count;      // in usbd.irx set to 1
   int max_count;       // in usbd.irx set to 1
};

int CreateSema( struct t_sema * );
int DeleteSema( int h_sem );
int SignalSema( int h_sem );
int iSignalSema( int h_sem );
int WaitSema( int h_sem );
int PollSema( int h_sem );

#endif //__THSEMAP_H__

