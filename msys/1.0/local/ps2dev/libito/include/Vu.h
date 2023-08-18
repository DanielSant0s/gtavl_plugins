#ifndef _ITO_VU_H
#define _ITO_VU_H


namespace Ito
{
	/** General Vector Unit (VU) and VU0 Macro functions.*/
	namespace Vu
	{
		extern "C" 
		{	/** Reset VU0 and VU1. You must call this function before using any of the VU's. */
			extern void VpuReset();
		}

		/** VU0 Macro Square-root. 
			\param n value to calculate square-root of.
			\return result of square-root.
		*/
		float Sqrt(float n);
		/** VU0 Macro cosine. 
			\param a angle in radian.
			\return cosine value for angle
		*/
		float Cos(float a);
		/** VU0 Macro sine. 
			\param a angle in radian.
			\return sine value for angle
		*/
		float Sin(float a);
	
	
		u64 CalculateLight(Vector lightDirection, Vector lightColor, Vector normal, Matrix worldMatrix);
	}
}

#endif
