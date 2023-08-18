#ifndef _ITO_FPU_H
#define _ITO_FPU_H

#include <ItoTypes.h>


namespace Ito
{
	/** Float Point Unit (FPU) Math Functions. */
	namespace Fpu
	{
		/** Calculate light */
		u64 CalculateLight(Vector lightDirection, Vector lightColor, Vector normal, Matrix worldMatrix);

		/** Square-root function. 
			\return Square-root value of n
		*/
		float Sqrt(float n);
		/** Consine.
			\param a angle in radian
			\return Cosine value of angle
		*/
		float Cos(float a);
		/** Consine.
			\param a angle in radian
			\return Sine value of angle
		*/
		float Sin(float a);


	}
}

#endif
