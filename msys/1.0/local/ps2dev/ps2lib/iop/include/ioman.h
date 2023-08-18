/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  ioman.h
*/

#ifndef PS2LIB_IOP_IOMAN_H
#define PS2LIB_IOP_IOMAN_H

#define O_RDONLY	0x0001
#define O_WRONLY	0x0002
#define O_RDWR		0x0003
#define O_NBLOCK	0x0010
#define O_APPEND	0x0100
#define O_CREAT		0x0200
#define O_TRUNC		0x0400
#define O_NOWAIT	0x8000

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

int open(const char *name, int flags, ...);
int close( int fd);
int read( int fd, void *ptr, int size);
int write( int fd, void *ptr, int size);
int lseek( int fd, unsigned long pos, int mode);

int ioctl(int fd, unsigned long arg, void *param);
int remove(const char *name);

int mkdir(const char *path);
int rmdir(const char *path);
int dopen(const char *path);
int dclose(int fd);
int dread(int fd, void *buf);

int getstat(const char *name, void *stat);
/* Not sure about the args on this one.  */
int chstat(const char *name, void *stat, unsigned int statmask);

/* This can take take more than one form.  */
int format(const char *dev, ...);

/* Device drivers.  */

/* Device types.  */
#define IOP_DT_CHAR	0x01
#define IOP_DT_CONS	0x02
#define IOP_DT_BLOCK	0x04
#define IOP_DT_RAW	0x08
#define IOP_DT_FS	0x10

/* File objects passed to driver operations.  */
typedef struct _iop_file {
	int	mode;
	int	num;
	struct _iop_device *device;
	void	*privdata;	/* The device driver can use this however it
				   wants.  */
} iop_file_t;

typedef struct _iop_device {
	const char *name;
	unsigned int type;
	unsigned int version;	/* Not so sure about this one.  */
	const char *desc;
	struct _iop_device_ops *ops;
} iop_device_t;

struct _iop_device_ops {
	int	(*init)(void);
	int	(*deinit)(void);
	int	(*format)(iop_file_t *f, ...);
	int	(*open)(iop_file_t *f, const char *name, int mode, ...);
	int	(*close)(iop_file_t *f);
	int	(*read)(iop_file_t *f, void *buf, int size);
	int	(*write)(iop_file_t *f, void *buf, int size);
	int	(*lseek)(iop_file_t *f, unsigned long post, int whence);
	int	(*ioctl)(iop_file_t *f, unsigned long arg, void *param);
	int	(*remove)(iop_file_t *f, const char *name);
	int	(*mkdir)(iop_file_t *f, const char *path);
	int	(*rmdir)(iop_file_t *f, const char *path);
	int	(*dopen)(iop_file_t *f, const char *name);
	int	(*dclose)(iop_file_t *f);
	int	(*dread)(iop_file_t *f, void *buf);
	int	(*getstat)(iop_file_t *f, const char *name, void *stat);
	int	(*chstat)(iop_file_t *f, const char *name, void *stat,
			unsigned int statmask);
} iop_device_ops_t;

int AddDrv(iop_device_t *device);
int DelDrv(const char *name);

/* Compatibility with older fileio.h - This stuff is deprecated, it will be
   removed in the next major release! */

#include <tamtypes.h>

struct fileio_driver
{
   u8				*device;
   u32				xx1;			/*  always 16? */
   u32				version;		/*  1 */
   u8				*description;
   void				**function_list;
};

/* function list index */
enum 
{
   FIO_INITIALIZE,		/* initialize(struct fileio_driver *); */
   FIO_SHUTDOWN,		/* shutdown(struct fileio_driver *) */
   FIO_FORMAT,			/* format( int kernel_fd, char *name); */
   FIO_OPEN,			/* open( int kernel_fd, char *name, int mode); */
   FIO_CLOSE,			/* close( int kernel_fd); */
   FIO_READ,			/* read( int kernel_fd, void *ptr, int size); */
   FIO_WRITE,			/* write( int kernel_fd, void *ptr, int size); */
   FIO_SEEK,			/* seek( int kernel_fd, u32 pos, int mode); */
   FIO_IOCTL,			/* ioctl( int kernel_fd, u32 type, void *value); */
   FIO_REMOVE,			/* remove( int kernel_fd, char *name); */
   FIO_MKDIR,			/* mkdir( int kernel_fd, char *name); */
   FIO_RMDIR,			/* rmdir( int kernel_fd, char *name); */
   FIO_DOPEN,			/* dopen( int kernel_fd, char *name); */
   FIO_DCLOSE,			/* dclose( int kernel_fd); */
   FIO_DREAD,			/* dread( int kernel_fd, void *ptr); */
   FIO_GETSTAT,			/* getstat( int kernel_fd, char *name, void *buf); it is not void*, but a structure */
   FIO_CHSTAT			/* chstat( int kernel_fd, char *name, void *buf, unsigned int mask); */
};

int FILEIO_add( struct fileio_driver *driver);
int FILEIO_del( u8 *device);

#endif /* PS2LIB_IOP_IOMAN_H */
