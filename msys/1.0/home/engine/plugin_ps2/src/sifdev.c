#include <sifdev.h>

int (*sceSifLoadModule)(char*, int, int) = (int (*)(char*, int, int))0x005449A0;

int (*open)(const char *pathname, int flags) = (int (*)(const char *, int))0x00540B28;
int (*close)(int fd) = (int (*)(int))0x00540DB8;
size_t (*lseek)(int fd, size_t offset, int whence) = (size_t (*)(int, size_t, int))0x00540F30;
size_t (*read)(int fd, void *buf, size_t count) = (size_t (*)(int, void *, size_t))0x00541170;
size_t (*write)(int fd, const void *buf, size_t count) = (size_t (*)(int, const void *, size_t))0x005413E0;
int (*chdir)(const char *name) = (int (*)(const char *))0x00542D20;
