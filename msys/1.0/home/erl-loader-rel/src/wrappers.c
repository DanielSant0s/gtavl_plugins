#include "wrappers.h"

void (*FlushCache)(int) = (void (*)(int))0x0053BA20;

int (*printf)(const char *, ...) = (int (*)(const char *, ...))0x0054F7D0;

void* (*malloc)(size_t) = (void* (*)(size_t))0x001005E0;
void (*free)(void*) = (void (*)(void*))0x00100780;

FILE* (*rw_open)(const char*, const char*) = (FILE* (*)(const char*, const char*))0x00233210;
size_t (*rw_read)(FILE *, void *, size_t) = (size_t (*)(FILE *, void *, size_t))0x00233250;
int (*rw_seek)(FILE *, long int, int) = (int (*)(FILE *, long int, int))0x002332B0;
int (*rw_close)(FILE *) = (int (*)(FILE *))0x00233370;
char* (*loadLine)(FILE *) = (char* (*)(FILE *))0x003C70A0;
int (*setDirectory)(const char *) = (int (*)(const char *))0x002330B0;

int (*open)(const char *pathname, int flags) = (int (*)(const char *, int))0x00540B28;
int (*close)(int fd) = (int (*)(int))0x00540DB8;
size_t (*lseek)(int fd, size_t offset, int whence) = (size_t (*)(int, size_t, int))0x00540F30;
size_t (*read)(int fd, void *buf, size_t count) = (size_t (*)(int, void *, size_t))0x00541170;
size_t (*write)(int fd, const void *buf, size_t count) = (size_t (*)(int, const void *, size_t))0x005413E0;
int (*chdir)(const char *name) = (int (*)(const char *))0x00542D20;

int (*sceDopen)(const char *dirname) = (int (*)(const char *))0x00542388;
int (*sceDclose)(int fd) = (int (*)(int))0x00542450;
int (*sceDread)(int fd, struct sce_dirent *buf) = (int (*)(int, struct sce_dirent *))0x005425B8;
int (*sceRemove)(const char *filename) = (int (*)(const char *))0x00542B18;
int (*sceMkdir)(const char *dirname, int flag) = (int (*)(const char *, int))0x00541DE0;
int (*sceRmdir)(const char *dirname) = (int (*)(const char *))0x00541F98;


void* (*memset)(void *, int, size_t) = (void* (*)(void *, int, size_t))0x0054E568;
void* (*memcpy)(void *, const void *, size_t) = (void* (*)(void *, const void *, size_t))0x0054E3B0;
int (*memcmp)(const void *, const void *, size_t) = (int (*)(const void *, const void *, size_t))0x0054E318;

size_t (*strlen)(const char *) = (size_t (*)(const char *))0x005517B8;
int (*strcmp)(const char *, const char *) = (int (*)(const char *, const char *))0x00551558;
char* (*strcpy)(char *, const char *) = (char* (*)(char *, const char *))0x005516A0;
char* (*strcat)(char *, const char *) = (char* (*)(char *, const char *))0x00551298;

char * strdup(const char * s){
  size_t len = 1+strlen(s);
  char *p = malloc(len);

  return (p ? memcpy(p, s, len) : NULL);
}

