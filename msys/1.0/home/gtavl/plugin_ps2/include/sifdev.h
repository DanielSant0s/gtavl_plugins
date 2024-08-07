#ifndef __SIFDEV_H__
#define __SIFDEV_H__

#include <stdint.h>

/* Flag for sceOpen() */
#define SCE_RDONLY      0x0001
#define SCE_WRONLY      0x0002
#define SCE_RDWR        0x0003
#define SCE_NBLOCK      0x0010  /* Non-Blocking I/O */
#define SCE_APPEND      0x0100  /* append (writes guaranteed at the end) */
#define SCE_CREAT       0x0200  /* open with file create */
#define SCE_TRUNC       0x0400  /* open with truncation */
#define SCE_EXCL        0x0800  /* exclusive create */
#define SCE_NOBUF       0x4000  /* no device buffer and console interrupt */
#define SCE_NOWAIT      0x8000  /* asyncronous i/o */

struct sce_stat {
        unsigned int    st_mode;        /* �ե�����μ���(file/dir) */
                                        /* �ȥ⡼��(R/W/X) */
        unsigned int    st_attr;        /* �ǥХ�����¸��°�� */
        unsigned int    st_size;        /* �ե����륵���� ���� 32 bit */
        unsigned char   st_ctime[8];    /* �������� */
        unsigned char   st_atime[8];    /* �ǽ����Ȼ��� */
        unsigned char   st_mtime[8];    /* �ǽ��ѹ����� */
        unsigned int    st_hisize;      /* �ե����륵���� ��� 32bit */
        unsigned int    st_private[6];  /* ����¾ */
};

int sceSifLoadModule(char* pathname, int flags, int);
int sceSifSearchModuleByName(const char *modulename);


int sceOpen(const char *pathname, int flags);
int sceClose(int fd);
size_t sceLseek(int fd, size_t offset, int whence);
size_t sceRead(int fd, void *buf, size_t count);
size_t sceWrite(int fd, const void *buf, size_t count);
int chdir(const char *name);

int sceRemove ( const char *filename);
int sceSync( const char *path, int flag);	

int  sceGetstat ( const char *name, struct sce_stat *buf);

#endif
