/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
                           (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  fileio.c
                        EE FILE IO handling
                        These are my findings based on debug-info elf files.
*/
#ifndef _FILEIO_H
#define _FILEIO_H

#define O_RDONLY	0x0001
#define O_WRONLY	0x0002
#define O_RDWR		0x0003
#define O_NBLOCK	0x0010
#define O_APPEND	0x0100
#define O_CREAT		0x0200
#define O_TRUNC		0x0400
#define O_EXCL		0x0800
#define O_NOWAIT	0x8000

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define FIO_PATH_MAX	256

#define FIO_WAIT		0
#define FIO_NOWAIT		1

#define FIO_COMPLETE	1
#define FIO_INCOMPLETE	0

#ifdef __cplusplus
extern "C" {
#endif

int fioInit(void);
void fioExit(void);
int fioOpen(const char *fname, int mode);
int fioClose( int fd);
int fioRead( int fd, void *buff, int buff_size);
int fioWrite( int fd, void *buff, int buff_size);
int fioLseek( int fd, int offset, int whence);
int fioMkdir(const char* dirname);
int fioPutc(int fd,int c);
int fioGetc(int fd);
void fioSetBlockMode(int blocking);
int fioSync(int mode, int *retVal);

int nprintf(const char *format, ...);
int printf(const char *format, ...);
int sprintf(char*, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // _FILEIO_H
