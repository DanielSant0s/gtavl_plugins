/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
                           (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  sbrk.c
			EE kernel sbrk function
                        Primary function to malloc - reserved
*/

#include <tamtypes.h>
#include <kernel.h>

extern int _end;

static void * _heap_ptr = (void *)&_end;

void *ps2_sbrk(size_t increment)
{
	int state;
	u32 mp;
	void *ret = (void *)-1;

	/* increment is 0 if the caller just wants the top of the heap.  */
	if (increment == 0)
		return _heap_ptr;

	asm volatile ("mfc0 %0, $12" : "=r" (state));
	state = state & 0x10000;

	if (state)
		DI;

	/* If the area we want to allocated is past the end of our heap, we
	   have a problem.  */
	mp = (u32)_heap_ptr + increment;
	if (mp > (u32)EndOfHeap())
		goto out;

	ret = _heap_ptr;
	_heap_ptr = (void *)mp;

out:
	if (state)
		EI;

	return ret;
}
