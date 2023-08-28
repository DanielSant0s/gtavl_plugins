#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>

extern void* (*memset)(void *, int, uint32_t);
extern void* (*memcpy)(void *, const void *, uint32_t);
extern int (*memcmp)(const void *, const void *, uint32_t);
extern uint32_t (*strlen)(const char *);
extern int (*strcmp)(const char *, const char *);
extern char* (*strcpy)(char *, const char *);
extern char* (*strcat)(char *, const char *);
extern char* (*strncpy)(char *, const char *, uint32_t);
extern int (*strncmp)(const char *str1, const char *str2, size_t n);

char * strdup(const char * s);

#endif
