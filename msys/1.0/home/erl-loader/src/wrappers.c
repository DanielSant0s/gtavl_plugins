#include "wrappers.h"

void (*FlushCache)(int) = (void (*)(int))0x0053BA20;

int (*printf)(const char *, ...) = (int (*)(const char *, ...))0x0054F7D0;
int (*sscanf)( const char * s, const char * format, ...) = (int (*)( const char * s, const char * format, ...))0x550FA8;

void* (*sceMalloc)(size_t) = (void* (*)(size_t))0x001005E0;


typedef struct HeapBlockDesc HeapBlockDesc;

struct HeapBlockDesc
{
	unsigned int m_size;
	short m_memId;
	short m_ptrListIndex;
	HeapBlockDesc *m_next;
	HeapBlockDesc *m_prev;
}; 

void* malloc(size_t size) {
  HeapBlockDesc* ptr = sceMalloc(size);
  HeapBlockDesc *blockDesc = ptr - 1;
  *PluginUsedMemory = ((*PluginUsedMemory) + blockDesc->m_size);

  return (void*)ptr;
}

void (*sceFree)(void*) = (void (*)(void*))0x00100780;

void free(void *ptr) {
  HeapBlockDesc *alloc = ptr;
  HeapBlockDesc *blockDesc = alloc - 1;
  *PluginUsedMemory = ((*PluginUsedMemory) - blockDesc->m_size);
  sceFree(ptr);
}

FILE* (*rw_open)(const char*, const char*) = (FILE* (*)(const char*, const char*))0x00233210;
size_t (*rw_read)(FILE *, void *, size_t) = (size_t (*)(FILE *, void *, size_t))0x00233250;
int (*rw_seek)(FILE *, long int, int) = (int (*)(FILE *, long int, int))0x002332B0;
int (*rw_close)(FILE *) = (int (*)(FILE *))0x00233370;
char* (*loadLine)(FILE *) = (char* (*)(FILE *))0x003C70A0;
int (*setDirectory)(const char *) = (int (*)(const char *))0x002330B0;

void* (*memset)(void *, int, size_t) = (void* (*)(void *, int, size_t))0x0054E568;
void* (*memcpy)(void *, const void *, size_t) = (void* (*)(void *, const void *, size_t))0x0054E3B0;
int (*memcmp)(const void *, const void *, size_t) = (int (*)(const void *, const void *, size_t))0x0054E318;

size_t (*strlen)(const char *) = (size_t (*)(const char *))0x005517B8;
int (*strcmp)(const char *, const char *) = (int (*)(const char *, const char *))0x00551558;
char* (*strcpy)(char *, const char *) = (char* (*)(char *, const char *))0x005516A0;
char* (*strcat)(char *, const char *) = (char* (*)(char *, const char *))0x00551298;

unsigned int* PluginUsedMemory = (unsigned int*)0x3940BC;

RwFileFunction* (*RwOsGetFileInterface)(void)  = (RwFileFunction* (*)(void))0x3332B8;

char * strdup(const char * s){
  size_t len = 1+strlen(s);
  char *p = malloc(len);

  return (p ? memcpy(p, s, len) : NULL);
}

