#ifndef _ITO_VU0MATH_H
#define _ITO_VU0MATH_H

#include <ItoTypes.h>

namespace Ito
{
	class VuVector;
	class VuMatrix;
}

/** Floating Point Unit Vector Math. 
	\par About
	FpuVector and VuVector have identical operations, one uses the FPU and the other VU0 Macro Mode.	
	\par Usage
	Please see samples in samples/vu0macro directory for usage.


*/
class Ito::VuVector
{
	public:
		const static u32 FormatFrac0 = 0;
		const static u32 FormatFrac4 = 1;
		/** Vector. */
		Vector m_Vector;
	public:	
		/** Constructor. Sets nothing. */
		VuVector(){}	
		~VuVector(){}

		VuVector(float x, float y, float z, float w = 1.0f);
		VuVector(Vector v);
		
		// Operators
		operator Vector&();

		/** Sets Unit Vector. */
		void Unit();
		/** Add Vector. 
		\param v vector to add.
		*/
		void Add(Vector v);
		/** Substract Vector.
		\param v vector to substract.
		*/
		void Sub(Vector v);
		/** Multiply Vector.
		\param v vector to multiply with.
		*/
		void Mult(Vector v);
		/** Divide x,y,z componets of vector.
		\param v vector to divide x,y,z elements of.
		*/
		void DivXYZ(float f);
		/** Divide by scalar value.
		\param n scalar value to divide by.
		*/
		void Div(float f);
		/** Divide x,y,z componets of the vector with component w. */
		void DivW();
		/** Scale x,y,z componets of vector.
		\param v vector to scale x,y,z with.
		*/
		void ScaleXYZ(float f);
		/** Scale vector.
		\param v vector to scale with.
		*/
		void Scale(float f);
		/** Clamp vector. Clamps the vector in the range of min - max.
		\param min minimum value
		\param max maximum value
		*/
		void Clamp(float min, float max);
		/** Normalize vector. */
		void Normalize();
		/** Dot Product. The dot product is calculated as the dot product between the calling vector and the parameter passed.
		\param v vector to calculate the dot product with
		\return Dot Product (this (dot) v)
		*/
		float DotProduct(Vector v);
		/** Cross Product. 
		Calculates the cross product of the 2 parameters and stores it in the calling vector. this = v1 x v2.
		\param vector 1
		\param vector 2
		*/
		void CrossProduct(Vector v1, Vector v2);
		/** Appy Matrix. 
		Apply a matrix the the calling vector.

		\param m Matrix to apply.
		*/
		void ApplyMatrix(Matrix m);
		
		void ApplyMatrix3x3(Matrix m);
		
		/** Transform vector from camera space to screen space. This function does the following. 1) Applies matrix m 2)
		Divides componets x,y,z 3) Converts to IntVector with fraction part depending on format parameter.
		\param m Matrix to apply
		\param output Output IntVector
		\param format The ouput format. 0 For 0-bit fractional part or 1 for 4-bit fractional part.
		*/
		void ScreenTransform(Matrix m, IntVector& output, u32 format);
		void ScreenTransform(Matrix m, IntVector output[], u32 count, u32 qspacing, u32 format);
		/** Prints the contents of the vector to console. */
		void Print();
};

/** Floating Point Unit Matrix Math.

	\par About
	VuMatrix and FpuMatrix have identical operations, one uses the FPU and the other VU0 Macro Mode.	

	\par Importent
	All matrix operations are multiplied to the calling matrix.
	\par Usage
	Please see samples in samples/fpu directory for usage.
	


*/
class Ito::VuMatrix
{
	public:
		Matrix m_Matrix;
	public:
		
		/** Constructor. */
		VuMatrix(){}
		/** Constructor. Construct VuMatrix from Matrix.
			\param m Matrix to set VuMatrix to.
		*/
		VuMatrix(Matrix m);
		/** Destructor. */
		~VuMatrix(){}
		void operator =(Matrix m);
		operator Matrix&();
		/** Prints the contents of the matrix to console. */
		void Print(char* description = 0);
		/** Sets identity/unit matrix */
		void Unit();
		/** Multiply matrix.
			\param m Matrix to multiply with (result : this = this * m)
		*/
		void Mult(Matrix m);
		/** Rotate Matrix by X axis. 
			\param rx Rotation angle in radian
		*/
		void RotateX(float rx);
		/** Rotate Matrix by X axis. 
			\param ry Rotation angle in radian
		*/
		void RotateY(float ry);
		/** Rotate Matrix by X axis. 
			\param rz Rotation angle in radian
		*/
		void RotateZ(float rz);
		/** Rotate Matrix by X,Y and Z axis.
			\param v Vector to use for rotation, vector x,y and z components are used.
		*/
		void Rotate(Vector v);
		/** Rotate Matrix by X,Y and Z axis.
			\param rx X rotation angle in radian
			\param ry Y rotation angle in radian
			\param rz Z rotation angle in radian
			
		*/
		void Rotate(float rx, float ry, float rz);
		/** Translate Matrix.
			\param tv Vector used to translate, vector x, and z components are used.
		*/
		void Translate(Vector tv);
		/** Translate Matrix.
			\param tx X translation 
			\param tx Y translation 
			\param tx Z translation 
		*/
		void Translate(float tx, float ty, float tz);
		/** Calculate transpose of matrix. */
		void Transpose();
		/** Perspective Project Matrix.
			Same as OpenGL function gluPerspective.

			\param fovy field of view in degrees (common value is 45)
			\param aspect aspect ratio (common value is 4/3 = 1.33)
			\param zNear Z value near
			\param zFar Z Value far
		*/
		void Perspective(float fovy, float aspect, float zNear, float zFar);
		/** LookAt Matrix.
		Same as OpenGL function gluLookAt.
		LookAt creates a viewing matrix derived from an eye point, a reference point indicating the center of the scene, and an UP vector.
			\param eye Eye vector
			\param center Center vector
			\param up Up vector
		*/
		void LookAt(Vector eye, Vector center, Vector up);
		
		/** LookAt Matrix.
		Same as OpenGL function gluLookAt.
		LookAt creates a viewing matrix derived from an eye point, a reference point indicating the center of the scene, and an UP vector.
			\param eyeX, eyeY, eyeZ Eye vector
			\param centerX, centerY, centerZ Center vector
			\param upX, upY, upZ Up vector
		*/		
		void LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
		/** Viewport Matrix.
			Same as OpenGL function glViewport with 2 addiontal parameters.
			Viewport is the rectangle area on screen in which you wish to render.

			\param x X offset of viewport (common value is 0)
			\param y Y offset of viewport (common value is 0)
			\param width Width of viewport (common value is width of screen)
			\param height Height of viewport (common value is height of screen)
			\param zmin Minumum value for zbuffer (common value is 0)
			\param zmax Maximum value for zbuffer (common value is max value for zbuffer)
	
		*/
		void Viewport(u32 x, u32 y, u32 width, u32 height, u32 zmin, u32 zmax);
};




#endif

