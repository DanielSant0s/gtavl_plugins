/*      
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  iopcontrol.h
                           IOP reset and status routines.
*/

#ifndef IOPCONTROL_H
#define IOPCONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

int SifIopReset(const char *arg, int mode);

int SifIopIsAlive(void);
int SifIopSync(void);

/* This is retained for compatibility.  */
int SifResetIop();

#ifdef __cplusplus
}
#endif

#endif /* IOPCONTROL_H */
