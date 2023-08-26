#include <string.h>
#include <stdlib.h>

void* (*memset)(void *, int, uint32_t)  = (void* (*)(void *, int, uint32_t))0x0054E568;
void* (*memcpy)(void *, const void *, uint32_t)  = (void* (*)(void *, const void *, uint32_t))0x0054E3B0;
int (*memcmp)(const void *, const void *, uint32_t)  = (int (*)(const void *, const void *, uint32_t))0x0054E318;
uint32_t (*strlen)(const char *)  = (uint32_t (*)(const char *))0x005517B8;
int (*strcmp)(const char *, const char *)  = (int (*)(const char *, const char *))0x00551558;
char* (*strcpy)(char *, const char *)  = (char* (*)(char *, const char *))0x005516A0;
char* (*strcat)(char *, const char *)  = (char* (*)(char *, const char *))0x00551298;
char* (*strncpy)(char *, const char *, uint32_t) = (char* (*)(char *, const char *, uint32_t))0x00551D80;
int (*strncmp)(const char *str1, const char *str2, size_t n) = (int (*)(const char *str1, const char *str2, size_t n))0x551BC8;

char * strdup(const char * s){
  size_t len = 1+strlen(s);
  char *p = malloc(len);

  return (p ? memcpy(p, s, len) : NULL);
}
