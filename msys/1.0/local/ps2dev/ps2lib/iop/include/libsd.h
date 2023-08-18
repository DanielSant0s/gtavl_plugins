/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, Julian Tyler (lovely@crm114.net)
  ------------------------------------------------------------------------
  libsd.h
			IOP libsd.h
			based on iop model. shared functions
*/

#ifndef _LIBSD_H_
#define _LIBSD_H_

#include <tamtypes.h>

typedef struct {
	u16 func;
	u16 entry;
	u32  value;
} SdBatch;

typedef struct {
     int     core;
     int     mode;
     short   depth_L;
     short   depth_R;
     int     delay;
     int     feedback;
} SdEffectAttr;

extern int     SdInit(int flag);

/*
Please Note that the CallBack function parameters in the two following
functions are placeholders.  If you know the correct parameters for these
functions let me know.
*/

extern void*   SdSetIRQCallback ( void SD_IRQ_CBProc(void *) );
extern void*   SdSetTransCallback (u16 channel, void SD_TRANS_CBProc(void *) );

extern void    SdSetParam(u16 entry, u16 value);
extern u16     SdGetParam(u16 entry);

extern void    SdSetCoreAttr(u16 entry, u16 value );
extern u16     SdGetCoreAttr(u16 entry );
extern int     SdClearEffectWorkArea (int core, int channel, int effect_mode );

extern void    SdSetAddr(u16 entry, u32 value );
extern u32     SdGetAddr(u16 entry );

extern void    SdSetSwitch(u16 entry, u32 value );
extern u32     SdGetSwitch(u16 entry );


extern u16     SdNote2Pitch (u16 center_note, u16 center_fine, u16 note, short fine);
extern u16     SdPitch2Note (u16 center_note, u16 center_fine, u16 pitch);

extern int     SdSetEffectAttr (int core, SdEffectAttr *attr );
extern void    SdGetEffectAttr (int core, SdEffectAttr *attr );

extern int     SdProcBatch(SdBatch* batch, u32 returns[], u32 num  );
extern int     SdProcBatchEx(SdBatch* batch, u32 returns[], u32 num, u32 voice  );

extern int     SdVoiceTrans(short channel, u16 mode, u8 *m_addr, u8 *s_addr, u32 size );
extern int     SdBlockTrans(short channel, u16 mode, u8 *m_addr, u32 size, ... );
extern u32   SdVoiceTransStatus (short channel, short flag);
extern u32   SdBlockTransStatus (short channel, short flag);

#endif 


