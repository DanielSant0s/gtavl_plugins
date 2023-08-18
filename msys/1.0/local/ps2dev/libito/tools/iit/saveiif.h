#include "types.h"
#include "loadimage.h"

#define IIF_OPTION_SWIZZLE	(1 << 1)
#define IIF_OPTION_NOCLUT	(1 << 2)
#define IIF_OPTION_ONLYCLUT	(1 << 3)

#define IIF_OPTION_FORCE	(1 << 31)

bool SaveIIF(char* filename, Image *image, uint32 psm, uint32 options);
