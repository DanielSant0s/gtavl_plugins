/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
  sysclib.h
			IOP sysclib.h
			based on iop model. shared functions
*/

#ifndef __SYSCLIB_H__
#define __SYSCLIB_H__

int printf(char *format, ...);

void * memcpy( void * dest, const void * src, unsigned int n );
void * memset( void * p , int c, unsigned int n );
char * strcat( char * s1, const char * s2 );
int    strcmp( const char * s1, const char *s2 );
char * strcpy( char * s1, const char * s2 );
int strlen( const char *s );
int    strncmp( const char *s1, const char *s2, int n );
char strncpy(char *s1, const char *s2, unsigned int n);
char *strstr(const char *haystack, const char *needle);
void bcopy(const void *src, void *dest, int  n);
void bzero(void *s, int n);
long strtol(char *,char**, int);
unsigned long strtoul(char *, char **, int);
char *strtok( char *strToken, const char *strDelimit );
char *strrchr( const char *string, int c );
char *index(const char *s, int c);
char *rindex(const char *s, int c);

#endif //__SYSCLIB_H__

