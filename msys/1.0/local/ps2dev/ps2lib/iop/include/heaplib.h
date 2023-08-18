/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, David Ryan (Oobles@hotmail.com)
  ------------------------------------------------------------------------
  heaplib.h
			IOP Heap library prototypes
*/

#ifndef _HEAPLIB_H
#define _HEAPLIB_H

#define heap_ptr void *

heap_ptr CreateHeap( int chunkSize,  int memoryType );
int DestroyHeap( heap_ptr );

void * HeapMalloc( heap_ptr, int size );
int HeapFree( heap_ptr, void * mem );
int HeapSize( heap_ptr );

#endif
