#include <stdio.h>

int (*printf)(const char *, ...) = (int (*)(const char *, ...))0x54F7D0;
int (*sprintf)(char *str, const char *format, ...) = (int (*)(char *, const char *, ...))0x00550F10;
int (*sscanf)( const char * s, const char * format, ...) = (int (*)( const char * s, const char * format, ...))0x550FA8;

FILE *(*fopen)(const char *filename, const char *mode) = (FILE *(*)(const char *filename, const char *mode))0x54C218;
size_t (*fread)(void *ptr, size_t size, size_t nmemb, FILE *stream) = (size_t (*)(void *ptr, size_t size, size_t nmemb, FILE *stream))0x54C2E8;
size_t (*fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream) = (size_t (*)(const void *ptr, size_t size, size_t nmemb, FILE *stream))0x54D300;
char *(*fgets)(char *str, int n, FILE *stream) = (char *(*)(char *str, int n, FILE *stream))0x54BCA8;
int (*fseek)(FILE *stream, long int offset, int whence) = (int (*)(FILE *stream, long int offset, int whence))0x54C870;
int (*fputs)(const char *str, FILE *stream) = (int (*)(const char *str, FILE *stream))0x54C298;
int (*fflush)(FILE *stream) = (int (*)(FILE *stream))0x54BB90;
long int (*ftell)(FILE *stream) = (long int (*)(FILE *stream))0x54CD80;
int (*feof)(FILE *stream) = (int (*)(FILE *stream))0x54BB80;
int (*fexist)(const char * name) = (int (*)(const char * name))0x3332C8;
int (*fclose)(FILE *stream) = (int (*)(FILE *stream))0x54BA80;
