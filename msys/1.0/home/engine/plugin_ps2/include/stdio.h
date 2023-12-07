#ifndef STDIO_H_
#define STDIO_H_

#include <stdint.h>

extern int (*printf)(const char *, ...);
extern int (*sprintf)(char *str, const char *format, ...);
extern int (*sscanf)( const char * s, const char * format, ...);

typedef uint32_t FILE; // TODO: Replace with the real struct

extern FILE *(*fopen)(const char *filename, const char *mode);
extern size_t (*fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern size_t (*fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
extern char *(*fgets)(char *str, int n, FILE *stream);
extern int (*fseek)(FILE *stream, long int offset, int whence);
extern int (*fputs)(const char *str, FILE *stream);
extern int (*fflush)(FILE *stream);
extern long int (*ftell)(FILE *stream);
extern int (*feof)(FILE *stream);
extern int (*fexist)(const char * name);
extern int (*fclose)(FILE *stream);

#endif



