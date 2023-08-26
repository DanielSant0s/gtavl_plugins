

typedef long unsigned int size_t;

#define O_RDONLY 0x0000

#define NULL ((char *)0)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef int FILE;

struct sce_stat {
        unsigned int    st_mode;       
                                    
        unsigned int    st_attr;       
        unsigned int    st_size;       
        unsigned char   st_ctime[8];   
        unsigned char   st_atime[8];   
        unsigned char   st_mtime[8];   
        unsigned int    st_hisize;     
        unsigned int    st_private[6]; 
};

struct sce_dirent {
        struct sce_stat d_stat; 
        char d_name[256];       
        void    *d_private;     
};

extern void (*FlushCache)(int);

extern FILE* (*rw_open)(const char*, const char*) __attribute__((section(".data")));
extern size_t (*rw_read)(FILE *, void *, size_t) __attribute__((section(".data")));
extern int (*rw_seek)(FILE *, long int, int) __attribute__((section(".data")));
extern int (*rw_close)(FILE *) __attribute__((section(".data")));
extern char* (*loadLine)(FILE *) __attribute__((section(".data")));
extern int (*setDirectory)(const char *) __attribute__((section(".data")));

extern int (*open)(const char *pathname, int flags);
extern int (*close)(int fd);
extern size_t (*lseek)(int fd, size_t offset, int whence);
extern size_t (*read)(int fd, void *buf, size_t count);
extern size_t (*write)(int fd, const void *buf, size_t count);
extern int (*chdir)(const char *name);

extern int (*sceDopen)(const char *dirname);
extern int (*sceDclose)(int fd);
extern int (*sceDread)(int fd, struct sce_dirent *buf);
extern int (*sceRemove)(const char *filename);
extern int (*sceMkdir)(const char *dirname, int flag);
extern int (*sceRmdir)(const char *dirname);



extern int (*printf)(const char *, ...);

extern void* (*malloc)(size_t);
extern void (*free)(void*);
extern void* (*memset)(void *, int, size_t);
extern void* (*memcpy)(void *, const void *, size_t);
extern int (*memcmp)(const void *, const void *, size_t);

extern size_t (*strlen)(const char *);
extern int (*strcmp)(const char *, const char *);
extern char* (*strcpy)(char *, const char *);
extern char* (*strcat)(char *, const char *);

extern char * strdup(const char * s);
