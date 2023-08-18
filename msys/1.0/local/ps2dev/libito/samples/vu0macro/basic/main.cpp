#include <Ito.h>
#include <stdio.h>
#include <CAnim8or.h>

using namespace Ito;
using namespace Vu;

extern struct Anim8orObject object_sphere;

u32 ScreenWidth;
u32 ScreenHeight;

int main()
{
	GifPipe	Gif;	

	ScreenWidth  = 640;
	ScreenHeight = 448;

	if(Gs::GetVideoMode() == VMODE_PAL)  ScreenHeight += 64;

	Gs::Reset();
	Gif.SetupEnv(RGBA32, ScreenWidth, ScreenHeight, 2, VMODE_AUTO, FIELD, ENABLE, 155, 70, ZBUF32);


	CAnim8or mesh;

	mesh.Load(object_sphere);

	VpuReset();	

	Vector* vecs = new Vector[mesh.GetVertexCount(0)];
	Vector* normals = new Vector[mesh.GetVertexCount(0)];

	mesh.GetVertices(vecs, 0);
	mesh.GetNormals(normals,0);

	VuMatrix matrixVP; // perspective,viewport matrix

	matrixVP.Unit();
	matrixVP.Perspective(45.0f, (float)(ScreenWidth/ScreenHeight), 1.0f, 100.0f);
	matrixVP.Viewport(0, 0, ScreenWidth, ScreenHeight, 0, 0xFFFFFFFF);

	float angle = 0.0f;

	IntVector iv[3];
	VuVector fv[3];

	Gif.PrimAntialias(true);
	Gif.PrimShading(PRIM_GOURAUD);

	u64 color[3];

	VuVector light(0.4f, 0.2f, 0.8f);
	VuVector lcolor(255.0f, 0.0f, 0.0f);
	

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		
		Gif.ZTest(false);
		
		Gif.ClearScreen(RGB(0xFF,0xFF,0xFF));
		
		Gif.ZTest(true, ZTEST_GREATER);

		VuMatrix matrixTransform;
	
		matrixTransform.Unit();

		// Rotate object
		matrixTransform.Rotate(0.0f, angle, angle);
		
		// Move object further away from view
		matrixTransform.Translate(0.0f, 0.0f, 4.0f);
		
		
		// Screen transform matrix with perspective and viewport applied
		VuMatrix matrixScreenTransform = matrixTransform;
		
		matrixScreenTransform.Mult(matrixVP);

		for(u32 i=0; i < (mesh.GetVertexCount(0)/3); i++)
		{
			// Read 3 vertices from mesh = 1 triangle
			color[0] = CalculateLight(light, lcolor, normals[(i*3)+0], matrixTransform);
			color[1] = CalculateLight(light, lcolor, normals[(i*3)+1], matrixTransform);
			color[2] = CalculateLight(light, lcolor, normals[(i*3)+2], matrixTransform);
					  

			fv[0] = vecs[(i*3)+0];
			fv[1] = vecs[(i*3)+1];
			fv[2] = vecs[(i*3)+2];
			
			fv[0].ScreenTransform(matrixScreenTransform, iv[0], 1);
			fv[1].ScreenTransform(matrixScreenTransform, iv[1], 1);
			fv[2].ScreenTransform(matrixScreenTransform, iv[2], 1);

			Gif.Triangle4(color[0], iv[0], color[1], iv[1], color[2], iv[2]);
		}


		angle += (0.01745f*4);
		
		if(angle > 6.28f) angle = -6.28f;

		Gif.Finish();
		
	}


	return 0;
}
