#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>

void* memset(void *ptr, int value, uint32_t size);
void* memcpy(void *dest, const void *src, uint32_t size);
int memcmp(const void *ptr1, const void *ptr2, uint32_t size);
uint32_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);
char* strcpy(char *dest, const char *src);
char* strcat(char *dest, const char *src);
char* strncpy(char *dest, const char *src, uint32_t n);
int strncmp(const char *str1, const char *str2, size_t n);
int strcasecmp(const char *str1, const char *str2);
char* strtok(char *str, const char *delim);
char* strchr(const char *str, int c);

char * strdup(const char * s);

#endif
