#include <string.h>
#include <stdint.h>
#include <stdlib.h>

char * strdup(const char * s){
  size_t len = 1+strlen(s);
  char *p = malloc(len);

  return (p ? memcpy(p, s, len) : NULL);
}
