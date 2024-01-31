#ifndef STDIO_H_
#define STDIO_H_

#include <stdint.h>

typedef uint32_t FILE; // TODO: Replace with the real struct

int printf(const char *, ...);
int sprintf(char *str, const char *format, ...);
int sscanf(const char *s, const char *format, ...);

FILE *fopen(const char *filename, const char *mode);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
char *fgets(char *str, int n, FILE *stream);
int fseek(FILE *stream, long int offset, int whence);
int fputs(const char *str, FILE *stream);
int fflush(FILE *stream);
long int ftell(FILE *stream);
int feof(FILE *stream);
int fexist(const char *name);
int fclose(FILE *stream);

#endif



