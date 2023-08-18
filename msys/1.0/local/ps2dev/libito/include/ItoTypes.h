#ifndef _ITOTYPES_H
#define _ITOTYPES_H

#include <tamtypes.h>

#define ALIGN(n)			__attribute__ ((aligned(n)))
#define TRUE				1
#define FALSE				0

#define ENABLE				1
#define DISABLE				0

/** Vector
{   x,   y,   z,   w }

( [0], [1], [2], [3]  }

*/

typedef float Vector[4] ALIGN(16);

/** IntVector
{   x,   y,   z,   w }

( [0], [1], [2], [3]  }

*/
typedef s32 IntVector[4] ALIGN(16);

/** 4x4 Matrix

{  x1   ,  y1   ,  z1   ,  w1    }
{  x2   ,  y2   ,  z2   ,  w2    }
{  x3   ,  y3   ,  z3   ,  w3    }
{   0   ,   0   ,   0   ,   1    }

{ [0][0], [1][0], [2][0], [3][0] }
{ [0][1], [1][1], [2][1], [3][1] }
{ [0][2], [1][2], [2][2], [3][2] }
{ [0][3], [1][3], [2][3], [3][3] }
*/
typedef float Matrix[4][4] ALIGN(16);



#endif

