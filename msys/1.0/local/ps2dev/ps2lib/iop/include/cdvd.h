/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, Nick Van Veen (nickvv@xtra.co.nz)
  ------------------------------------------------------------------------
  cdvd.h		IOP CDVD library prototypes

*/

#ifndef _CDVD_H
#define _CDVD_H

#include <tamtypes.h>

#define CdSecS2048		0	
#define CdSecS2328		1	
#define CdSecS2340		2	

#define CdSpinMax		0
#define CdSpinNom		1
#define CdSpinStm		0

#define CdMmodeCd		1
#define CdMmodeDvd		2

typedef struct {
	u8 stat;  			
	u8 second; 			
	u8 minute; 			
	u8 hour; 			
	u8 week; 			
	u8 day; 			
	u8 month; 			
	u8 year; 			
} CdCLOCK;

typedef struct {
	u32 lsn; 			
	u32 size; 			
	char name[16]; 		
	u8 date[8]; 		
} CdlFILE;

typedef struct {
	u8 minute; 			
	u8 second; 			
	u8 sector; 			
	u8 track; 			
} CdlLOCCD;

typedef struct {
	u8 trycount; 		
	u8 spindlctrl; 		
	u8 datapattern; 	
	u8 pad; 			
} CdRMode;


typedef struct
{
	u32	fileLBA;
	u32 fileSize;
	u16	fileProperties;
	u8	filename[128];
} TocEntry;


int CdBreak(void);
int CdCallback( void (*func)() );
int CdDiskReady(int mode);
int CdGetDiskType(void);
int CdGetError(void);
u32 CdGetReadPos(void);
int CdGetToc(u8 *toc);
int CdInit(int init_mode);
CdlLOCCD *CdIntToPos(int i, CdlLOCCD *p);
int CdPause(void);
int CdPosToInt(CdlLOCCD *p);
int CdRead(u32 lsn, u32 sectors, void *buf, CdRMode *mode);
int CdReadClock(CdCLOCK *rtc);
int CdSearchFile (CdlFILE *fp, const char *name);
int CdSeek(u32 lsn);
int CdStandby(void);
int CdStatus(void);
int CdStop(void);
int CdSync(int mode);
int CdTrayReq(int mode, u32 *traycnt);
int CdMmode(int mode);

#endif // _CDVD_H
