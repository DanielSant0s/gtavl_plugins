
#ifndef __LOADCORE_H__
#define __LOADCORE_H__

#define LOADCORE_VER	0x101

#define MODULE_RESIDENT_END		0
#define MODULE_NO_RESIDENT_END	1


/*
  The real name is RegisterLibraryEntries so the function
  has been renamed.  The old name is #defined to ensure
  old code doesn't break;
*/

#define ExportFunctions  RegisterLibraryEntries

void *	GetExportTable(void);
void	FlushDcache();	
int	RegisterLibraryEntries(void*);	
int*	QueryBootMode(int code);

#endif//__LOADCORE_H__
