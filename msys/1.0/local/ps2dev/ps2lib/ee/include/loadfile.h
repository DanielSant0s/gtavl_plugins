/*      
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  loadfile.h
                           IOP executable file loader.
*/

#ifndef LOADFILE_H
#define LOADFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tamtypes.h>

#define LF_PATH_MAX	252
#define LF_ARG_MAX	252

enum _lf_val_types {
	LF_VAL_BYTE  = 0,
	LF_VAL_SHORT,
	LF_VAL_LONG
};

typedef struct 
{
	u32 epc;
	u32 gp;
	u32 sp;
	u32 dummy;  
} t_ExecData;

int SifLoadFileInit(void);
void SifLoadFileExit(void);

int SifLoadModule(const char *path, int arg_len, const char *args);
int SifLoadModuleEncrypted(const char *path, int arg_len, const char *args);
int SifLoadModuleBuffer(void *ptr, int arg_len, const char *args);

int SifLoadElfPart(const char *path, const char *secname, t_ExecData *data);
int SifLoadElf(const char *path, t_ExecData *data);
int SifLoadElfEncrypted(const char *path, t_ExecData *data);

int SifIopSetVal(u32 iop_addr, int val, int type);
int SifIopGetVal(u32 iop_addr, void *val, int type);

#ifdef __cplusplus
}
#endif

#endif /* LOADFILE_H */
