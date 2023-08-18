#ifndef TYPES_H
#define TYPES_H

// Yes this file has stuff which doesn't belong here :P

#define RGBA32			0
#define RGB24			1
#define RGBA16			2
#define CLUT8_RGBA32	3
#define CLUT8_RGBA16	4
#define CLUT4_RGBA32	5
#define CLUT4_RGBA16	6
#define AUTO			10

typedef unsigned char	uint8;
typedef unsigned short	uint16;			
typedef unsigned int	uint32;

typedef char			int8;
typedef short			int16;			
typedef int				int32;


/*
#ifdef DEBUG
	#define dprintf(format, args...) printf("DEBUG "format, ## args)
#else
	#define dprintf(args...) do { } while(0)
#endif
*/

#endif
