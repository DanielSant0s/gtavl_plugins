#include <ItoTypes.h>
#include <Vu0Math.h>
#include <Vu.h>
#include <kernel.h>
#include <stdio.h>


using namespace Ito;
using namespace Vu;

// TODO: Change this to pure inline asm
#ifdef F_CalculateLight
u64 Vu::CalculateLight(Vector lightDirection, Vector lightColor, Vector normal, Matrix worldMatrix)
{
	
	VuVector v = normal;
	Vector ld;

	// Apply rotations to normal
	v.ApplyMatrix3x3(worldMatrix);

	for(u32 i=0; i < 3; i++)
		ld[i] = 0-lightDirection[i];
	
	// Get light influence from direction vector
	float li = v.DotProduct(ld);
	
	

	// Multiply light influence with color
	v = lightColor;
	v.ScaleXYZ(li);

	//printf("vu  = %f, %f, %f\n", v.m_Vector[0], v.m_Vector[1], v.m_Vector[2] );

	// Clamp in 0-255 range
	v.Clamp(0, 255.0f);

	///printf("vu  = %f, %f, %f\n", v.m_Vector[0], v.m_Vector[1], v.m_Vector[2] );

	//printf("x = %g\n", v.m_Vector[0]);

	u64 r = (u32)(v.m_Vector[0]);
	u64 g = (u32)(v.m_Vector[1]);
	u64 b = (u32)(v.m_Vector[2]);
	return ((b << 16) | (g << 8) | r);
}
#endif


// ----------------------------------------------------------------------------
// Vector
// ----------------------------------------------------------------------------

#ifdef F_V_Unit
void VuVector::Unit()
{
	asm volatile("sqc2 $vf0, 0(%0)" : : "r"(m_Vector));
}
#endif

#ifdef F_V_Constructor
VuVector::VuVector(float x, float y, float z, float w)
{
	m_Vector[0] = x;
	m_Vector[1] = y;
	m_Vector[2] = z;
	m_Vector[3] = w;
}
#endif

#ifdef F_V_Constructor2
VuVector::VuVector(Vector v)
{
	asm volatile("lq $8, 0(%0)" : : "r"(v));
	asm volatile("sq $8, 0(%0)" : : "r"(m_Vector));
}
#endif

#ifdef F_V_Operator1
VuVector::operator Vector&()
{
	return m_Vector;
}
#endif

#ifdef F_V_Add
void VuVector::Add(Vector v)
{
	asm volatile("lqc2		$vf4, 0(%0)" : : "r"(m_Vector));
	asm volatile("lqc2		$vf5, 0(%0)" : : "r"(v));
	asm volatile("vadd.xyz	$vf4, $vf4, $vf5");
	asm volatile("sqc2		$vf4, 0(%0)" : : "r"(m_Vector));
}
#endif


#ifdef F_V_ApplyMatrix
void VuVector::ApplyMatrix(Matrix m)
{
	asm volatile("lqc2			$vf4,  0(%0)" : : "r"(m)); 
	asm volatile("lqc2			$vf5, 16(%0)" : : "r"(m));
	asm volatile("lqc2			$vf6, 32(%0)" : : "r"(m));
	asm volatile("lqc2			$vf7, 48(%0)" : : "r"(m));	
	asm volatile("lqc2			$vf8,  0(%0)" : : "r"(m_Vector)); 
	asm volatile("vmulax.xyzw	ACC, $vf4, $vf8x");
	asm volatile("vmadday.xyzw	ACC, $vf5, $vf8y");
	asm volatile("vmaddaz.xyzw	ACC, $vf6, $vf8z");
	asm volatile("vmaddw.xyzw	$vf9, $vf7, $vf8w"); // +ACC also included
	asm volatile("sqc2			$vf9,  0(%0)" : : "r"(m_Vector)); // save result

}
#endif

#ifdef F_V_ApplyMatrix3x3
void VuVector::ApplyMatrix3x3(Matrix m)
{
	asm volatile("lqc2			$vf4,  0(%0)" : : "r"(m)); 
	asm volatile("lqc2			$vf5, 16(%0)" : : "r"(m));
	asm volatile("lqc2			$vf6, 32(%0)" : : "r"(m));
//	asm volatile("lqc2			$vf7, 48(%0)" : : "r"(m));	
	asm volatile("lqc2			$vf8,  0(%0)" : : "r"(m_Vector)); 
	asm volatile("vmulax.xyzw	ACC, $vf4, $vf8x");
	asm volatile("vmadday.xyzw	ACC, $vf5, $vf8y");
	asm volatile("vmaddz.xyzw	$vf9, $vf6, $vf8z");
//	asm volatile("vmaddw.xyzw	$vf9, $vf7, $vf8w"); // +ACC also included
	asm volatile("sqc2			$vf9,  0(%0)" : : "r"(m_Vector)); // save result

}
#endif


#ifdef F_V_Clamp
void VuVector::Clamp(float min, float max)
{
	asm volatile("mfc1			$14, $f12" ::: "$14");
	asm volatile("mfc1			$15, $f13" ::: "$15");
	asm volatile("qmtc2			$14, vf4" ::: "$14");	
	asm volatile("qmtc2			$15, vf5" ::: "$15");
	asm volatile("lqc2			$vf6,  0(%0)" : : "r"(m_Vector));
	asm volatile("vmaxx.xyzw	vf6, vf6, vf4x");
	asm volatile("vminix.xyzw	vf6, vf6, vf5x");
	asm volatile("sqc2			$vf6,  0(%0)" : : "r"(m_Vector));
}
#endif

#ifdef F_V_ScaleXYZ
void VuVector::ScaleXYZ(float f)
{
	asm volatile("mfc1			$14, $f12" ::: "$14");
	asm volatile("qmtc2			$14, vf4" ::: "$14");	
	asm volatile("lqc2			$vf5,  0(%0)" : : "r"(m_Vector));
	asm volatile("vmulx.xyz		$vf5, $vf5, $vf4x");
	asm volatile("sqc2			$vf5,  0(%0)" : : "r"(m_Vector));
}
#endif

#ifdef F_V_Scale
void VuVector::Scale(float f)
{
	asm volatile("mfc1			$4, $f12");
	asm volatile("qmtc2			$4, vf4");	
	asm volatile("lqc2			$vf5,  0(%0)" : : "r"(m_Vector));
	asm volatile("vmulx.xyzw	$vf5, $vf5, $vf4x");
	asm volatile("sqc2			$vf5,  0(%0)" : : "r"(m_Vector));
}
#endif

#ifdef F_V_DotProduct
float VuVector::DotProduct(Vector v)
{
	register float ret = 0.0f;

	asm volatile("lqc2		$vf4,  0(%0)" : : "r"(m_Vector));
	asm volatile("lqc2		$vf5,  0(%0)" : : "r"(v));
	asm volatile("vmul.xyz	$vf4, $vf4, $vf5"); // The power of vu, x,y,z mult in 1 instruction
	asm volatile("vaddy.x	$vf4, $vf4, $vf4y"); // x += y
	asm volatile("vaddz.x	$vf4, $vf4, $vf4z"); // x += z
	asm volatile("qmfc2		$2, $vf4" ::: "$2");
	asm volatile("mtc1		$2, $f0"); // Do not remove this line, it will break the code.
	asm volatile("move		%0, $2" :: "r"(ret));

	return ret;
}
#endif


#ifdef F_V_ScreenTransform
void VuVector::ScreenTransform(Matrix m, IntVector& output, u32 format)
{
	// Apply matrix m to vector
	asm volatile("lqc2			$vf4,  0(%0)" :: "r"(m)); // m
	asm volatile("lqc2			$vf5, 16(%0)" :: "r"(m));
	asm volatile("lqc2			$vf6, 32(%0)" :: "r"(m));
	asm volatile("lqc2			$vf7, 48(%0)" :: "r"(m));	
	asm volatile("lqc2			$vf8,  0(%0)" :: "r"(m_Vector));
	// Mult Matrix
	asm volatile("vmulax.xyzw	ACC, $vf4, $vf8x");
	asm volatile("vmadday.xyzw	ACC, $vf5, $vf8y");
	asm volatile("vmaddaz.xyzw	ACC, $vf6, $vf8z");
	asm volatile("vmaddw.xyzw	$vf9, $vf7, $vf8w"); // +ACC also included
	
	// Normalized Device to Screen Coordinates
	asm volatile("vdiv			Q, $vf0w, $vf9w"); // q = 1.0f/ v.w
	asm volatile("vwaitq");
	asm volatile("vmulq.xyz		$vf9, $vf9xyz, Q");
	asm volatile("sqc2			$vf9,  0(%0)" :: "r"(m_Vector));
	asm volatile("vftoi0.xyz	$vf7, $vf9");
	asm volatile("beq			%0, $0, Trans4" :: "r"(format));
	asm volatile("vftoi4.xy		$vf7, $vf9");
	asm volatile("Trans4:");
	asm volatile("sqc2			$vf7, 0(%0)" :: "r"(output));
}
#endif

// ----------------------------------------------------------------------------
// Matrix
// ----------------------------------------------------------------------------

#ifdef F_M_Operator
VuMatrix::operator Matrix&()
{
	return m_Matrix;
}
#endif

#ifdef F_M_Unit
void VuMatrix::Unit()
{
	asm volatile("vmove.xyzw	$vf4, $vf0");
	asm volatile("vmr32.xyzw	$vf5, $vf4");
	asm volatile("vmr32.xyzw	$vf6, $vf5");
	asm volatile("vmr32.xyzw	$vf7, $vf6");
	asm volatile("sqc2			$vf4, 48(%0)" : : "r"(m_Matrix));
	asm volatile("sqc2			$vf5, 32(%0)" : : "r"(m_Matrix));
	asm volatile("sqc2			$vf6, 16(%0)" : : "r"(m_Matrix));
	asm volatile("sqc2			$vf7,  0(%0)" : : "r"(m_Matrix));

}
#endif

#ifdef F_M_Equal
void VuMatrix::operator =(Matrix m)
{	
	asm volatile("lqc2			$vf4,  0(%0)" : : "r"(m));
	asm volatile("lqc2			$vf5, 16(%0)" : : "r"(m));
	asm volatile("lqc2			$vf6, 32(%0)" : : "r"(m));
	asm volatile("lqc2			$vf7, 48(%0)" : : "r"(m));
	asm volatile("sqc2			$vf4,  0(%0)" : : "r"(m_Matrix));
	asm volatile("sqc2			$vf5, 16(%0)" : : "r"(m_Matrix));
	asm volatile("sqc2			$vf6, 32(%0)" : : "r"(m_Matrix));
	asm volatile("sqc2			$vf7, 48(%0)" : : "r"(m_Matrix));
}

#endif

#ifdef F_M_Mult
void VuMatrix::Mult(Matrix m)
{
	asm volatile("lqc2			$vf6,  0(%0)" : : "r"(m));
	asm volatile("lqc2			$vf7, 16(%0)" : : "r"(m));
	asm volatile("lqc2			$vf8, 32(%0)" : : "r"(m));
	asm volatile("lqc2			$vf9, 48(%0)" : : "r"(m));
	asm volatile("daddu			$14, $31, $0"	::: "$14");
	asm volatile("la			$4, 0(%0)" :: "r"(m_Matrix));
	asm volatile("jal			MacroMatrixMult");			
	asm volatile("nop");
	asm volatile("daddu			$31, $14, $0"	::: "$14");
}
#endif

#ifdef F_M_RotateX
void VuMatrix::RotateX(float rx)
{
	// X Rotation matrix
	//  $vf6   $vf7   $vf8   $vf9
	//
	// [  1  ][  0  ][  0  ][  0  ]
	// [  0  ][ cosx][-sinx][  0  ]
	// [  0  ][ sinx][ cosx][  0  ]
	// [  0  ][  0  ][  0  ][  1  ]

	asm volatile("mfc1			$14, $f12"		::: "$14");
	asm volatile("qmtc2			$14, $vf19"		::: "$14");
	asm volatile("daddu			$14, $31, $0"	::: "$14");
	asm volatile("jal			MacroSinCos");			// output vf19 x=sinx,y=cosx
	asm volatile("vsub.xyzw		$vf6, $vf0, $vf0");		//  vf6 = 0
	asm volatile("vsub.xyzw		$vf7, $vf0, $vf0");		// vf7 = 0
	asm volatile("vsub.xyzw		$vf8, $vf0, $vf0");		// vf8 = 0
	asm volatile("vmove.xyzw	$vf9, $vf0");			// vf9 = 0,0,0,1
	asm volatile("vaddw.x		$vf6, $vf5, $vf0w");	// [0][0] = 1
	asm volatile("vaddy.y		$vf7, $vf0, $vf19y");	// [1][1] = cosx
	asm volatile("vaddx.z		$vf7, $vf0, $vf19x");	// [1][2] = sinx
	asm volatile("vsubx.y		$vf8, $vf0, $vf19x");	// [2][1] = -sinx
	asm volatile("vaddy.z		$vf8, $vf0, $vf19y");	// [2][2] = cosx
	asm volatile("la			$4, 0(%0)" : : "r"(m_Matrix));
	asm volatile("jal			MacroMatrixMult");			
	asm volatile("daddu			$31, $14, $0"	::: "$14");
}
#endif

#ifdef F_M_RotateY
void VuMatrix::RotateY(float ry)
{
	// Y Rotation matrix
	//  $vf6   $vf7   $vf8   $vf9
	// [ cosy][  0  ][ siny][  0  ]
	// [  0  ][  1  ][  0  ][  0  ]
	// [-siny][  0  ][ cosy][  0  ]
	// [  0  ][  0  ][  0  ][  1  ]
	
	asm volatile("mfc1			$14, $f12"		::: "$14");
	asm volatile("qmtc2			$14, $vf19"		::: "$14");
	asm volatile("daddu			$14, $31, $0"	::: "$14");
	asm volatile("jal			MacroSinCos");			// output vf19 x=sinx,y=cosx
	asm volatile("vsub.xyzw		$vf6, $vf0, $vf0");		// vf6 = 0
	asm volatile("vsub.xyzw		$vf7, $vf0, $vf0");		// vf7 = 0
	asm volatile("vsub.xyzw		$vf8, $vf0, $vf0");		// vf8 = 0
	asm volatile("vmove.xyzw	$vf9, $vf0");			// vf9 = 0,0,0,1		
	asm volatile("vaddw.y		$vf7, $vf0, $vf0w");	// vf7 = 0,1,0,0
	asm volatile("vaddy.x		$vf6, $vf0, $vf19y");	// [0][0] = cosy
	asm volatile("vsubx.z		$vf6, $vf0, $vf19x");	// [0][2] = -siny	
	asm volatile("vaddx.x		$vf8, $vf0, $vf19x");	// [2][0] = siny			
	asm volatile("vaddy.z		$vf8, $vf0, $vf19y");	// [2][2] = cosy
	asm volatile("la			$4, 0(%0)" :: "r"(m_Matrix));
	asm volatile("jal			MacroMatrixMult");			
	asm volatile("daddu			$31, $14, $0"	::: "$14");
}
#endif

#ifdef F_M_RotateZ
void VuMatrix::RotateZ(float rz)
{
	// Z Rotation matrix
	//  $vf6   $vf7   $vf8   $vf9
	// [ cosz][-sinz][  0  ][  0  ]
	// [ sinz][ cosz][  0  ][  0  ]
	// [  0  ][  0  ][  1  ][  0  ]
	// [  0  ][  0  ][  0  ][  1  ]

	asm volatile("mfc1			$14, $f12"		::: "$14");
	asm volatile("qmtc2			$14, $vf19"		::: "$14");
	asm volatile("daddu			$14, $31, $0"	::: "$14");
	asm volatile("jal			MacroSinCos");			// output vf19 x=sinx,y=cosx
	asm volatile("vsub.xyzw		$vf6, $vf0, $vf0");		// vf5 = 0
	asm volatile("vsub.xyzw		$vf7, $vf0, $vf0");		// vf5 = 0
	asm volatile("vmove.xyzw	$vf9, $vf0");			// vf9 = 0,0,0,1		
	asm volatile("vmr32.xyzw	$vf8, $vf9");			// vf8 = 0,0,1,0
	asm volatile("vaddx.y		$vf6, $vf0, $vf19x");	// [0][1] = sinz			
	asm volatile("vaddy.x		$vf6, $vf0, $vf19y");	// [0][0] = cosz
	asm volatile("vsubx.x		$vf7, $vf0, $vf19x");	// [1][0] = -sinz	
	asm volatile("vaddy.y		$vf7, $vf0, $vf19y");	// [1][1] = cosz
	asm volatile("la			$4, 0(%0)" :: "r"(m_Matrix));
	asm volatile("jal			MacroMatrixMult");			
	asm volatile("daddu			$31, $14, $0"	::: "$14");
}
#endif

#ifdef F_M_Rotate
void VuMatrix::Rotate(float rx, float ry, float rz)
{
	RotateX(rx);
	RotateY(ry);
	RotateZ(rz);
}
#endif

/*
#ifdef F_m_translate2
void VuMatrix::translate(float tx, float ty, float tz)
{
	VuVector v;
	asm volatile("la			$13, 0(%0)"		:: "r"(v.vector): "$13");
	asm volatile("swc1			$f12, 0($13)");
	asm volatile("swc1			$f13, 4($13)");
	asm volatile("swc1			$f14, 8($13)");

	translate(v);
}
#endif
*/

#ifdef F_M_Translate2
void VuMatrix::Translate(float tx, float ty, float tz)
{
	Vector v;
	v[0] = tx;
	v[1] = ty;
	v[2] = tz;
	
	Translate(v);

}
#endif

#ifdef F_M_Translate
void VuMatrix::Translate(Vector v)
{
	// [  0  ][  0  ][  0  ][  +x  ]
	// [  0  ][  1  ][  0  ][  +y  ]
	// [  0  ][  0  ][  1  ][  +z  ]
	// [  0  ][  0  ][  0  ][   1  ]

	asm volatile("lqc2			$vf4, 48(%0)" :: "r"(m_Matrix));
	asm volatile("lqc2			$vf5,  0(%0)" :: "r"(v));
	asm volatile("vadd.xyz		$vf4, $vf4, $vf5"); // Just add the values instead of mult.
	asm volatile("sqc2			$vf4, 48(%0)" :: "r"(m_Matrix));

}
#endif

#ifdef F_M_Viewport
void VuMatrix::Viewport(u32 x, u32 y, u32 width, u32 height, u32 zmin, u32 zmax)
{
	// Viewport Matrix
	// Note: you still need to divide XYZ by W to get actual screen coords.
	//     $vf6       $vf7           $vf8               $vf9
	// [ width/2 ][    0     ][        0      ][     x+(width/2)      ]
	// [    0    ][ height/2 ][        0      ][     y+(height/2)     ]
	// [    0    ][    0     ][ (zmax-zmin)/2 ][ zmin+((zmax-zmin)/2) ]
	// [    0    ][    0     ][        0      ][          1           ]

	asm volatile("srl			%0, %0, 1"  :: "r"(width));				// width = width/2
	asm volatile("srl			%0, %0, 1"  :: "r"(height));			// height = height/2
	asm volatile("addu			%0, %0, %1" :: "r"(x), "r"(width));		// x = x+(width/2)
	asm volatile("addu			%0, %0, %1" :: "r"(y), "r"(height));	// y= y+(height/2)
	asm volatile("subu			%0, %0, %1" :: "r"(zmax), "r"(zmin));	// zmax = zmax-zmin
	asm volatile("srl			%0, %0, 1"	:: "r"(zmax));				// zmax = (zmax-zmin)/2
	asm volatile("addu			%0, %0, %1"	:: "r"(zmin), "r"(zmax));	//  zmin = zmin+((zmax-zmin)/2)
	asm volatile("qmtc2			%0, $vf6"	:: "r"(width));				// [0][0] = width/2
	asm volatile("vitof0.x		$vf6, $vf6");							// convert to float
	asm volatile("qmtc2			%0, $vf7"	:: "r"(height));			// vf7x = $7
	asm volatile("vitof0.x		$vf7, $vf7");							// convert to float
	asm volatile("vaddx.y		$vf7y, $vf0y, $vf7x");					// [1][1] = height/2
	asm volatile("vsubx.x		$vf7x, $vf7x, $vf7x");					// [1][0] = 0 
	asm volatile("qmtc2			%0, $vf8"	:: "r"(zmax));				// vf8x = zmax
	asm volatile("vitof0.x		$vf8, $vf8");							// convert to float
	asm volatile("vaddx.z		$vf8z, $vf0z, $vf8x");					// [2][2] = (zmax-zmin)/2
	asm volatile("vsubx.x		$vf8x, $vf8x, $vf8x");					// [2][0] = 0
	asm volatile("vmove.xyzw	$vf9, $vf0");							// vf9 = 0,0,0,1
	asm volatile("qmtc2			%0, $vf5"	:: "r"(x));					// temp load
	asm volatile("vitof0.x		$vf5, $vf5");							// conver to float
	asm volatile("vaddx.x		$vf9x, $vf9, $vf5");					// [3][0] = x+(width/2)
	asm volatile("qmtc2			%0, $vf5"	:: "r"(y));					// temp load
	asm volatile("vitof0.x		$vf5, $vf5");							// conver to float
	asm volatile("vaddx.y		$vf9y, $vf9, $vf5");					// [3][1] = y+(height/2)
	asm volatile("qmtc2			%0, $vf5"	:: "r"(zmin));				// temp load
	asm volatile("vitof0.x		$vf5, $vf5");							// conver to float
	asm volatile("vaddx.z		$vf9z, $vf9, $vf5");					// [3][2] = zmin+((zmax-zmin)/2)
	asm volatile("daddu			$14, $31, $0"	::: "$14");
	asm volatile("la			$4, 0(%0)" :: "r"(m_Matrix));
	asm volatile("jal			MacroMatrixMult");			
	asm volatile("nop");
	asm volatile("daddu			$31, $14, $0"	::: "$14");


}
#endif

#ifdef F_M_Perspective
void VuMatrix::Perspective(float fovy, float aspect, float zNear, float zFar)
{

	// Perspective Matrix
	// [ f/aspect ][ 0 ][              0            ][               0              ]
	// [    0     ][ f ][              0            ][               0              ]
	// [    0     ][ 0 ][ (zFar+zNear)/(zNear-zFar) ][ (-2*zFar*zNear)/(zNear-zFar) ]
	// [    0     ][ 0 ][             -1            ][               0              ]
	// f = cot(fovy/2) 
	// cot(x) = cos(x)/sin(x)

	asm volatile("li			$14, 0x3c0efa35":::"$14"); // load ((PI)/360.0f)
	asm volatile("mtc1			$14, $f0":::"$14");
	asm volatile("mul.s			$f12, $f12, $f0");		// fovy/2 and convert to radian
	asm volatile("mfc1			$14, $f12"  ::: "$14");
	asm volatile("qmtc2			$14, $vf19" ::: "$14");
	asm volatile("daddu			$14, $31, $0" ::: "$14");
	asm volatile("jal			MacroSinCos");			// output vf19 x=sinx,y=cosx
	asm volatile("nop");
	asm volatile("vdiv			Q, $vf19y, $vf19x");	// Q = cos(x)/sin(x)
	asm volatile("vwaitq");
	asm volatile("vaddq.x		$vf4, $vf0, Q");		// vf4.x = f (cot(x))
	asm volatile("vsub.xyzw		$vf5, $vf0, $vf0");
	
	asm volatile("mfc1			$6, $f15" :::"$6");		// zFar
	
	asm volatile("qmtc2			$6, $vf10" ::: "$6");	// qmtc2 overwrites the entire regiter, so we have to load it into vf10 first.
	asm volatile("vaddx.y		$vf5y, $vf5y, $vf10x");	//vf5.y = zNear
	
	asm volatile("mfc1			$6, $f14" :::"$6");		// zNear
	
	asm volatile("qmtc2			$6, $vf10" ::: "$6");
	asm volatile("vaddx.z		$vf5z, $vf5z, $vf10x");	// vf5.z = zNear
	asm volatile("mfc1			$6, $f13" :::"$6");
	asm volatile("qmtc2			$6, $vf10" ::: "$6");	  
	asm volatile("vaddx.x		$vf5x, $vf5x, $vf10x");	// vf5.x = aspect
	asm volatile("vsub.xyzw		$vf6, $vf0, $vf0");		// vf6 = 0
	asm volatile("vsub.xyzw		$vf7, $vf0, $vf0");		// vf7 = 0
	asm volatile("vsub.xyzw		$vf8, $vf7, $vf0");		// vf8 = 0,0,0,-1
	asm volatile("vsub.xyzw		$vf9, $vf0, $vf0");		// vf9 = 0
	asm volatile("vaddx.y		$vf7, $vf0, $vf4x");	// [1][1] = f
	asm volatile("vdiv			Q, $vf4x, $vf5x");
	asm volatile("vwaitq");
	asm volatile("vaddq.x		$vf6, $vf0, Q");		// [0][0] = f/aspect
	asm volatile("vsubz.y		$vf4y, $vf5y, $vf5z");	// vf4y = zFar-zNear
	asm volatile("vaddy.z		$vf4z, $vf5z, $vf5y");	// vf4z = zNear+zFar
	asm volatile("vdiv			Q, $vf4z, $vf4y");
	asm volatile("vwaitq");
	asm volatile("vaddq.x		$vf4, $vf0, Q");		// vf4x = (zFar+zNear)/(zFar-zNear)
	asm volatile("vaddx.z		$vf8z, $vf8z, $vf4x");	// [2][2] = $vf4x
	asm volatile("vmuly.z		$vf4z, $vf5z, $vf5y");	// vf4z = zNear*zFar 
	asm volatile("vaddw.w		$vf4w, $vf0w, $vf0w");	// vf4w = 2.0
	asm volatile("vsubw.w		$vf4w, $vf9w, $vf4w");  // vf4w = -2.0
	asm volatile("vmulw.z		$vf4z, $vf4z, $vf4w");	// vf4z = 2*zNear*zFar
	asm volatile("vdiv			Q, $vf4z, $vf4y");
	asm volatile("vwaitq");
	asm volatile("vaddq.x		$vf4x, $vf0, Q");		// vf4x = (-2*zFar*zNear)/(zNear-zFar)
	asm volatile("vaddx.z		$vf9z, $vf9z, $vf4x");	// [3][2] = vf4x
	asm volatile("la			$4, 0(%0)" :: "r"(m_Matrix));
	asm volatile("jal			MacroMatrixMult");			
	asm volatile("nop");
	asm volatile("daddu			$31, $14, $0"	::: "$14");


}
#endif

#ifdef F_M_Print
void VuMatrix::Print(char* description)
{
	if(description == 0)
		printf("Matrix [0x%08X]\n", (u32)m_Matrix);
	else
		printf("%s\n", description);
	
	for(u32 i=0; i < 4; i++)
		printf("[ %f ] [ %f ] [ %f ] [ %f ]\n", m_Matrix[0][i], m_Matrix[1][i], m_Matrix[2][i], m_Matrix[3][i]);

}
#endif
