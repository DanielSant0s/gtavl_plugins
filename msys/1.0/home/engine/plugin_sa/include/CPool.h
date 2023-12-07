#ifndef CPOOL_H_
#define CPOOL_H_

typedef struct 
{
  void *objects;
  signed char *flags;
  int size;
  int top;
  char initialized;
  char field_11;
  short _pad;
} CPool;

#endif
