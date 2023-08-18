#include <FpuMath.h>
#include <Fpu.h>
#include <kernel.h>
#include <stdio.h>

using namespace Ito;
using namespace Fpu;

// cos & sin function are from PbDemo lib by emoon and others, see http://cvs.ps2dev.org

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

#define SIN_ITERATOR 20

#ifdef F_Cos
float Fpu::Cos( float v )
{
	float res,w;
	int t;
	float fac;
	int i = (int)((v)/(2.0f*M_PI));
	
	v-=i*2.0f*M_PI;

	fac=1.0f;
	res=0.0f;
	w=1.0f;
	for(t=0;t<SIN_ITERATOR;)
	{
		res+=fac*w;
		w*=v*v;
		t++;
		fac/=t;
		t++;
		fac/=t;
		
		res-=fac*w;
		w*=v*v;
		t++;
		fac/=t;
		t++;
		fac/=t;
	}
	return res;
}
#endif

#ifdef F_Sin
float Fpu::Sin( float v )
{
	float res,w;
	int t;
	float fac;
	int i=(int)((v)/(2.0f*M_PI));
	v-=i*2.0f*M_PI;

	fac=1.0f;
	res=0.0f;
	w=v;
	for(t=1;t<SIN_ITERATOR;)
	{
		res+=fac*w;
		w*=v*v;
		t++;
		fac/=t;
		t++;
		fac/=t;
		
		res-=fac*w;
		w*=v*v;
		t++;
		fac/=t;
		t++;
		fac/=t;
	}
	return res;
}
#endif

#ifdef F_Sqrt
float Fpu::Sqrt(float n)
{
	register float res;
	
	asm volatile("sqrt.s %0,%1\n" : "=f" (res) : "f" (n));
	
	return res;
}
#endif

#ifdef F_CalculateLight
u64 Fpu::CalculateLight(Vector lightDirection, Vector lightColor, Vector normal, Matrix worldMatrix)
{
	FpuVector v = normal;
	Vector ld;

	// Apply rotations to normal
	v.ApplyMatrix3x3(worldMatrix);

	// 
	for(u32 i=0; i < 3; i++)
		ld[i] = 0-lightDirection[i];
	
	// Get light influence from direction vector
	float li = v.DotProduct(ld);
	
	// Multiply light influence with color
	v = lightColor;
	v.ScaleXYZ(li);
	
	//printf("fpu  = %f, %f, %f\n", v.m_Vector[0], v.m_Vector[1], v.m_Vector[2] );

	// Clamp in 0-255 range
	v.Clamp(0, 255.0f);
	
//	printf("fpu  = %f, %f, %f\n", v.m_Vector[0], v.m_Vector[1], v.m_Vector[2] );

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

#ifdef F_V_Constructor
FpuVector::FpuVector(float x, float y, float z)
{
	m_Vector[0] = x;
	m_Vector[1] = y;
	m_Vector[2] = z;
	m_Vector[3] = 1.0f;
}
#endif

#ifdef F_V_Constructor2
FpuVector::FpuVector(Vector v)
{
	u128* src = (u128*)v;
	u128* dest = (u128*)m_Vector;

	*dest = *src;
}
#endif

#ifdef F_V_Operator1
FpuVector::operator Vector&()
{
	return m_Vector;
}
#endif

#ifdef F_V_Unit
void FpuVector::Unit()
{
	m_Vector[0] = m_Vector[1] = m_Vector[2] = 0.0f;
	m_Vector[3] = 1.0f;
}
#endif

#ifdef F_V_Add
void FpuVector::Add(Vector v)
{
	for(u32 i=0; i < 4; i++)
		m_Vector[i] += v[i];
}
#endif

#ifdef F_V_Sub
void FpuVector::Sub(Vector v)
{
	for(u32 i=0; i < 4; i++)
		m_Vector[i] -= v[i];
}
#endif

#ifdef F_V_Mult
void FpuVector::Mult(Vector v)
{
	for(u32 i=0; i < 4; i++)
		m_Vector[i] *= v[i];
}
#endif

#ifdef F_V_DivXYZ
void FpuVector::DivXYZ(float f)
{
	float s = 1/f;

	for(u32 i=0; i < 3; i++)
		m_Vector[i] *= s;
}
#endif

#ifdef F_V_DivW
void FpuVector::DivW()
{
	float s = 1/m_Vector[3];

	for(u32 i=0; i < 3; i++)
		m_Vector[i] *= s;
}
#endif

#ifdef F_V_Div
void FpuVector::Div(float f)
{
	float s = 1/f;

	for(u32 i=0; i < 4; i++)
		m_Vector[i] *= s;
}
#endif

#ifdef F_V_ScaleXYZ
void FpuVector::ScaleXYZ(float f)
{
	for(u32 i=0; i < 3; i++)
		m_Vector[i] *= f;
}
#endif

#ifdef F_V_Scale
void FpuVector::Scale(float f)
{
	for(u32 i=0; i < 4; i++)
		m_Vector[i] *= f;
}
#endif

#ifdef F_V_Clamp
void FpuVector::Clamp(float min, float max)
{
	for(u32 i=0; i < 4; i++)
	{
		if(m_Vector[i] < min) m_Vector[i] = min;
		if(m_Vector[i] > max) m_Vector[i] = max;
	}

}
#endif

#ifdef F_V_Normalize
void FpuVector::Normalize()
{
	float s = 1/Fpu::Sqrt((m_Vector[0]*m_Vector[0])+(m_Vector[1]*m_Vector[1])+(m_Vector[2]*m_Vector[2]));
	
	m_Vector[0] *= s;
	m_Vector[1] *= s;
	m_Vector[2] *= s;

}
#endif

#ifdef F_V_DotProduct
float FpuVector::DotProduct(Vector v)
{
	float res = 0.0f;

	for(u32 i=0; i < 3; i++)
		res += (m_Vector[i]*v[i]);

	return res;
}
#endif

#ifdef F_V_CrossProduct
void FpuVector::CrossProduct(Vector v1, Vector v2)
{
	//	( A.x , A.y , A.z ) X ( B.x , B.y , B.z ) = ( A.y * B.z - A.z * B.y , A.z * B.x - A.x * B.z , A.x * B.y - A.y * B.x)

	m_Vector[0] = (v1[1]*v2[2])-(v1[2]*v2[1]);
	m_Vector[1] = (v1[2]*v2[0])-(v1[0]*v2[2]);
	m_Vector[2] = (v1[0]*v2[1])-(v1[1]*v2[0]);

}
#endif

#ifdef F_V_ApplyMatrix
void FpuVector::ApplyMatrix(Matrix m)
{
	register float vx,vy,vz,vw;

	vx = (m_Vector[0]*m[0][0])+(m_Vector[1]*m[1][0])+(m_Vector[2]*m[2][0])+(m_Vector[3]*m[3][0]);
	vy = (m_Vector[0]*m[0][1])+(m_Vector[1]*m[1][1])+(m_Vector[2]*m[2][1])+(m_Vector[3]*m[3][1]);
	vz = (m_Vector[0]*m[0][2])+(m_Vector[1]*m[1][2])+(m_Vector[2]*m[2][2])+(m_Vector[3]*m[3][2]);
	vw = (m_Vector[0]*m[0][3])+(m_Vector[1]*m[1][3])+(m_Vector[2]*m[2][3])+(m_Vector[3]*m[3][3]);
	
	m_Vector[0] = vx;
	m_Vector[1] = vy;
	m_Vector[2] = vz;
	m_Vector[3] = vw;

}
#endif

#ifdef F_V_ApplyMatrix3x3
void FpuVector::ApplyMatrix3x3(Matrix m)
{
	register float vx,vy,vz;

	vx = (m_Vector[0]*m[0][0])+(m_Vector[1]*m[1][0])+(m_Vector[2]*m[2][0]);
	vy = (m_Vector[0]*m[0][1])+(m_Vector[1]*m[1][1])+(m_Vector[2]*m[2][1]);
	vz = (m_Vector[0]*m[0][2])+(m_Vector[1]*m[1][2])+(m_Vector[2]*m[2][2]);
	
	m_Vector[0] = vx;
	m_Vector[1] = vy;
	m_Vector[2] = vz;

}
#endif

#ifdef F_V_ScreenTransform
void FpuVector::ScreenTransform(Matrix m, IntVector& output, u32 format)
{
	ApplyMatrix(m);
	
	float s = 1/m_Vector[3];

	if(format == 1)
	{
		m_Vector[0] *= 16.0f;
		m_Vector[1] *= 16.0f;
	}

	output[0] = (u32)(m_Vector[0]*s);
	output[1] = (u32)(m_Vector[1]*s);
	output[2] = (u32)(m_Vector[2]*s);

//	m_Vector[2] = m_Vector[2]*s;

//	printf("z = %g\n", m_Vector[2]);

}
#endif

// ----------------------------------------------------------------------------
// Matrix
// ----------------------------------------------------------------------------

#ifdef F_M_Operator
FpuMatrix::operator Matrix&()
{
	return m_Matrix;
}
#endif

#ifdef F_M_Equal
void FpuMatrix::operator =(Matrix m)
{
	u128* src = (u128*)m;
	u128* dst = (u128*)m_Matrix;

	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = src[3];

}
#endif

#ifdef F_M_Unit
void FpuMatrix::Unit()
{
	for(u32 x=0; x < 4; x++)
	{
		for(u32 y=0; y < 4; y++)
		{	
			if(x == y)
				m_Matrix[x][y] = 1.0f;
			else
				m_Matrix[x][y] = 0.0f;

		}
	}
}
#endif

#ifdef F_M_Mult
void FpuMatrix::Mult(Matrix m)
{
	Matrix res;
	
	for(u32 x=0; x < 4; x++)
	{
		for(u32 y=0; y < 4; y++)
		{
			res[x][y]  = (m[0][y] * m_Matrix[x][0]);
			res[x][y] += (m[1][y] * m_Matrix[x][1]);
			res[x][y] += (m[2][y] * m_Matrix[x][2]);
			res[x][y] += (m[3][y] * m_Matrix[x][3]);
		}
	}

	u128* src = (u128*)res;
	u128* dst = (u128*)m_Matrix;

	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = src[3];
}
#endif

#ifdef F_M_Print
void FpuMatrix::Print(char* description)
{
	if(description == 0)
		printf("Matrix [0x%08X]\n", (u32)m_Matrix);
	else
		printf("%s\n", description);
	
	for(u32 i=0; i < 4; i++)
		printf("[ %f ] [ %f ] [ %f ] [ %f ]\n", m_Matrix[0][i], m_Matrix[1][i], m_Matrix[2][i], m_Matrix[3][i]);

}
#endif

#ifdef F_M_RotateX
void FpuMatrix::RotateX(float rx)
{
	float cosx = Cos(rx);
	float sinx = Sin(rx);
	
	FpuMatrix r;

	r.Unit();

	r.m_Matrix[1][1] = cosx;
	r.m_Matrix[1][2] = sinx;
	r.m_Matrix[2][1] = -sinx;
	r.m_Matrix[2][2] = cosx;

	Mult(r);
}
#endif

#ifdef F_M_RotateY
void FpuMatrix::RotateY(float ry)
{
	float cosy = Cos(ry);
	float siny = Sin(ry);
	
	FpuMatrix r;

	r.Unit();
	
	r.m_Matrix[0][0] = cosy;
	r.m_Matrix[0][2] = -siny;
	r.m_Matrix[2][0] = siny;
	r.m_Matrix[2][2] = cosy;

	Mult(r);
}
#endif

#ifdef F_M_RotateZ
void FpuMatrix::RotateZ(float rz)
{
	float cosz = Cos(rz);
	float sinz = Sin(rz);
	
	FpuMatrix r;

	r.Unit();

	r.m_Matrix[0][0] = cosz;
	r.m_Matrix[0][1] = sinz;
	r.m_Matrix[1][0] = -sinz;
	r.m_Matrix[1][1] = cosz;

	Mult(r);
}
#endif

#ifdef F_M_Rotate
void FpuMatrix::Rotate(Vector v)
{
	Rotate(v[0], v[1], v[2]);	
}
#endif

#ifdef F_M_Rotate2
void FpuMatrix::Rotate(float rx, float ry, float rz)
{
	RotateX(rx);
	RotateY(ry);
	RotateZ(rz);
}
#endif

#ifdef F_M_Translate2
void FpuMatrix::Translate(float tx, float ty, float tz)
{
	Vector v;
	v[0] = tx;
	v[1] = ty;
	v[2] = tz;
	
	Translate(v);

}
#endif

#ifdef F_M_Translate
void FpuMatrix::Translate(Vector tv)
{
	m_Matrix[3][0] += tv[0];
	m_Matrix[3][1] += tv[1];
	m_Matrix[3][2] += tv[2];
}
#endif

#ifdef F_M_Transpose
void FpuMatrix::Transpose()
{
	Matrix t;

	for(u32 x=0; x < 4; x++)
		for(u32 y=0; y < 4; y++)
			t[y][x] = m_Matrix[x][y];

}
#endif

#ifdef F_M_Perspective
void FpuMatrix::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	// Perspective Matrix
	// [ f/aspect ][ 0 ][              0            ][               0              ]
	// [    0     ][ f ][              0            ][               0              ]
	// [    0     ][ 0 ][ (zFar+zNear)/(zNear-zFar) ][ (-2*zFar*zNear)/(zNear-zFar) ]
	// [    0     ][ 0 ][             -1            ][               0              ]
	// f = cot(fovy/2) 
	// cot(x) = cos(x)/sin(x)
	
	fovy = (fovy*0.008727f); // convert to radian, fovy/2
	
	float f = Cos(fovy)/Sin(fovy);
	FpuMatrix p;


	p.Unit();

	p.m_Matrix[0][0] = f/aspect;
	p.m_Matrix[1][1] = f;
	
	p.m_Matrix[2][2] = (zFar+zNear)/(zFar-zNear);
	p.m_Matrix[2][3] = -1.0f;
	p.m_Matrix[3][2] = (-2*zFar*zNear)/(zFar-zNear);
	p.m_Matrix[3][3] = 0.0f;

	Mult(p);
}
#endif

// TODO move code to LookAt2
#ifdef F_M_LookAt2
void FpuMatrix::LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	LookAt(FpuVector(eyeX, eyeY, eyeZ), FpuVector(centerX, centerY, centerZ), FpuVector(upX, upY, upZ));
}
#endif

#ifdef F_M_LookAt
void FpuMatrix::LookAt(Vector eye, Vector center, Vector up)
{
	// LookAt Matrix
	// [  s[0] ][  s[1] ][  s[2] ][ 0 ]
	// [  u[0] ][  u[1] ][  u[2] ][ 0 ]
	// [ -f[0] ][ -f[1] ][ -f[2] ][ 0 ]
	// [   0   ][  0    ][    0  ][ 1 ]
	
	// f = center - eye;
	// f normalize
	// Up = up normalized
	// s = f x UP
	// u = s X f
	
	FpuVector f = center;
	FpuVector Up = up;
	FpuVector s,u;
	FpuMatrix look;

	f.Sub(eye);
	f.Normalize();
	Up.Normalize();
	s.CrossProduct(f, Up);
	u.CrossProduct(s, f);
	


	look.Unit();
	look.m_Matrix[0][0] = s.m_Vector[0];
	look.m_Matrix[1][0] = s.m_Vector[0];
	look.m_Matrix[2][0] = s.m_Vector[0];

	look.m_Matrix[0][1] = u.m_Vector[0];
	look.m_Matrix[1][1] = u.m_Vector[1];
	look.m_Matrix[2][1] = u.m_Vector[2];

	look.m_Matrix[0][2] = -f.m_Vector[0];
	look.m_Matrix[1][2] = -f.m_Vector[1];
	look.m_Matrix[2][2] = -f.m_Vector[2];

	Mult(look);
	Translate(-eye[0], -eye[1], -eye[2]);
}
#endif

#ifdef F_M_Viewport
void FpuMatrix::Viewport(u32 x, u32 y, u32 width, u32 height, u32 zmin, u32 zmax)
{
	// Viewport Matrix
	// [ width/2 ][    0     ][        0      ][     x+(width/2)      ]
	// [    0    ][ height/2 ][        0      ][     y+(height/2)     ]
	// [    0    ][    0     ][ (zmax-zmin)/2 ][ zmin+((zmax-zmin)/2) ]
	// [    0    ][    0     ][        0      ][          1           ]

	FpuMatrix v;
	
	v.Unit();
	
	v.m_Matrix[0][0] = (float)(width/2);
	v.m_Matrix[1][1] = (float)(height/2);
	
	v.m_Matrix[2][2] = (float)((zmax-zmin)/2);

	v.m_Matrix[3][0] = (float)(x+(width/2));
	v.m_Matrix[3][1] = (float)(y+(height/2));

	v.m_Matrix[3][2] = (float)(zmin+((zmax-zmin)/2));
	
	Mult(v);
}
#endif

