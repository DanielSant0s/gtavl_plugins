#ifndef _ITO_GIFPIPE_H
#define _ITO_GIFPIPE_H

#include <ItoTypes.h>
#include <GsDriver.h>
#include <GifPacket.h>
#include <TextureManager.h>

namespace Ito
{
	class GifPipe;
}

/** GS InterFace Pipeline (PATH3).

	\par About
	GifPipe is the most highlevel graphics part of libito, it usage is recommend for 2D graphics, but can also be used for 3D
	like shown in the vu0macro samples be usage for 3D. Graphics rendering on PS2 is a packet based, which means
	you build a buffer of data which you send to the Graphics Synthesizer (GS) which renders the result. GifPipe
	builds this packet in realtime based on the functions called and is therefor now recommend for high performance 3D graphics.
	
	GifPipe builds on top of the GsDriver, which handles video init and framebuffer switching among its major
	tasks. GifPipe can render primitives, set render states and upload textures.

	GifPipe installs a Vertical Retrace handler (VRHandler) which is a interrupt handler which gets called once a frame
	has been "drawn" on you TV. The VRHandler checks if the next framebuffer is fully rendered and if so it will switch to the next
	framebuffer (GsDriver::ShowNextFramebuffer) and free up the one just shown. Once it switches to the next framebuffer it also calls 
	TextureManager::Tick() which increases the logical time for the TextureManager, which is usage to determin for how long a texture 
	has been in VRAM.

	GifPipe also automaticly uploads textures which aren't in VRAM when calling SetTexture, so its not necessary to load the textures, 
	its all handled by GifPipe.


	\par Usage
	Please see samples in samples/gif directory for usage.

*/
class Ito::GifPipe: public GsDriver
{
	private:
		GifPacket*	m_GifPackets[2];
		GifPacket*	m_CurPacket;
		u32			m_PrimMode;
	public:
		u64 m_RegisterTEST;
		u64 m_RegisterTEX0;
		static const u32 AlphaBlendNormal = 0;
		static const u32 AlphaBlendAdditive = 1;
		static const u32 AlphaBlendSubtractive = 2;
		static const u32 AlphaBlendLighten = 3;
		static const u32 AlphaBlendDarken = 4;
		
		static const u32 TextureModulate = 0;
		static const u32 TextureDecal = 1;
		static const u32 TextureHighlight = 2;
		static const u32 TextureHighlight2 = 3;
		
		static const u32 TextureRGB = 0;
		static const u32 TextureRGBA = 1;
	private:
		void CheckPacket();
	public:
		GifPipe();
		GifPipe(u32 size);
		~GifPipe();
		/** Send packet to GS, note it does not wait for the transfer to end, use finish() instead.*/
		void Send();
		/** Same as Send() but waits for the the transfer to end. */
		void Finish();
		/** Set primitive shading.
			\param shading <tt>PRIM_SHADE_FLAT</tt>, <tt>PRIM_SHADE_FLAT</tt> */
		void PrimShading(u32 shading);
		/** Enable/disable primitive alpablending 
			\param enable true to enable, false to disable.	*/
		void PrimAlphaBlend(bool enable);
		/** Enable/disable primitive antialiasing. 
			\param enable true to enable, false to disable. */
		void PrimAntialias(bool enable);
		/** Clear the screen. 
		\param color (optional) is black RGB(0,0,0) by default. 
		*/
		void ClearScreen(u64 color = 0);
		/** Point/dot with 4 bit fraction part for x,y.
		\param color RGBA32 color
		\param x, y x,y coords
		\param z z value 
		*/
		void Point4(u64 color, s32 x, s32 y, s32 z = 0);
		/** Point/dot.
		\param color RGBA32 color
		\param v use x,y,z components of IntVector v as coords.
		*/
		inline void Point(u64 color, IntVector v)
		{
			Point(color, v[0], v[1], v[2]);
		}
		/** Point/dot.
		\param color RGBA32 color
		\param x, y x,y coords
		\param z z value
		*/
		inline void Point(u64 color, s32 x, s32 y, s32 z = 0)
		{
			Point4(color, (x) << 4, (y) << 4, z);
		}
		/** Line with 4 bit fraction part for x,y.
		\param color1 RGBA32 color for vertex 1
		\param x1, y1 x,y coords for vertex 1
		\param z1 z value for vertex 1
		\param color2 RGBA32 color for vertex 2
		\param x2, y2 x,y coords for vertex 2
		\param z2 z value for vertex 2
		*/
		void Line4(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2);
		/** Line strip with 4 bit fraction part for x,y.
			Use AddVertex4 to add addional vertices and EndStrip to end the strip.
		\param color1 RGBA32 color for vertex 1
		\param x1, y1 x,y coords for vertex 1
		\param z1 z value for vertex 1
		\param color2 RGBA32 color for vertex 2
		\param x2, y2 x,y coords for vertex 2
		\param z2 z value for vertex 2
		*/
		void LineStrip4(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2);
		/** Line strip.
			Use AddVertex to add addional vertices and EndStrip to end the strip.
		\param color1 RGBA32 color for vertex 1
		\param x1, y1 x,y coords for vertex 1
		\param z1 z value for vertex 1
		\param color2 RGBA32 color for vertex 2
		\param x2, y2 x,y coords for vertex 2
		\param z2 z value for vertex 2
		*/
		inline void LineStrip(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2)
		{	
			LineStrip4(color1, x1 << 4, y1 << 4, z1, color2, x2 << 4, y2 << 4, z2);
		}

		/** Line Strip. 
			Use AddVertex to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		*/
		inline void LineStrip(u64 color1, IntVector vector1, u64 color2, IntVector vector2)
		{
			LineStrip(color1, vector1[0], vector1[1], vector1[2], color2, vector2[0], vector2[1], vector2[2]);
		}
		
		/** Line.
		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		*/
		inline void Line(u64 color1, IntVector vector1, u64 color2, IntVector vector2)
		{
			Line(color1, vector1[0], vector1[1], vector1[2], color2, vector2[0], vector2[1], vector2[2]);
		}
		/** Line.
		\param color1 RGBA32 color for vertex 1
		\param x1, y1 x,y coords for vertex 1
		\param z1 z value for vertex 1
		\param color2 RGBA32 color for vertex 2
		\param x2, y2 x,y coords for vertex 2
		\param z2 z value for vertex 2
		*/
		inline void Line(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2)
		{	
			Line4(color1, x1 << 4, y1 << 4, z1, color2, x2 << 4, y2 << 4, z2);
		}
		/** Textured Line with 4 bit fraction part for x,y and u,v. Use SetTexture to specify the texture to use.
	
		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/	
		inline void TextureLine4(	u64 color1, IntVector vector1, u32 u1, u32 v1,
									u64 color2, IntVector vector2, u32 u2, u32 v2)
		{
			TextureLine4(	color1, vector1[0], vector1[1], vector1[2], u1, v1,
							color2, vector2[0], vector2[1], vector2[2], u2, v2);
		}
		/** Textured Line. Use SetTexture to specify the texture to use.
	
		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/		
		inline void TextureLine(	u64 color1, IntVector vector1, u32 u1, u32 v1,
									u64 color2, IntVector vector2, u32 u2, u32 v2)
		{
			TextureLine4(	color1, vector1[0], vector1[1], vector1[2], u1 << 4, v1 << 4,
							color2, vector2[0], vector2[1], vector2[2], u2 << 4, v2 << 4);
		}
		/** Textured Line. Use SetTexture to specify the texture to use.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/
		inline void TextureLine(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
									u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2)
		{
			TextureLine4(	color1, x1 << 4, y1 << 4, z1, u1 << 4, v1 << 4,
							color2, x2 << 4, y2 << 4, z2, u2 << 4, v2 << 4);
		}
		/** Textured Line with 4 bit fraction part for x,y,z and u,v params. Use SetTexture to specify the texture to use.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/
		inline void TextureLine4(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
									u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2);


	
		/** Textured Line Strip with 4 bit fraction part for x,y and u,v. Use SetTexture to specify the texture to use.
			Use AddTextureVertex4 to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/
		inline void TextureLineStrip4(	u64 color1, IntVector vector1, u32 u1, u32 v1,
									u64 color2, IntVector vector2, u32 u2, u32 v2)
		{
			TextureLineStrip4(	color1, vector1[0], vector1[1], vector1[2], u1, v1,
							color2, vector2[0], vector2[1], vector2[2], u2, v2);
		}
		/** Textured Line Strip. Use SetTexture to specify the texture to use.
			Use AddTextureVertex to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/
		inline void TextureLineStrip(	u64 color1, IntVector vector1, u32 u1, u32 v1,
									u64 color2, IntVector vector2, u32 u2, u32 v2)
		{
			TextureLineStrip(	color1, vector1[0], vector1[1], vector1[2], u1, v1,
								color2, vector2[0], vector2[1], vector2[2], u2, v2);
		}
		/** Textured Line Strip. Use SetTexture to specify the texture to use.
			Use AddTextureVertex to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		*/
		inline void TextureLineStrip(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
									u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2)
		{
			TextureLineStrip4(	color1, x1 << 4, y1 << 4, z1, u1 << 4, v1 << 4,
							color2, x2 << 4, y2 << 4, z2, u2 << 4, v2 << 4);
		}
		/** Textured Line Strip with 4 bit fraction part for x,y and u,v. Use SetTexture to specify the texture to use.
			Use AddTextureVertex4 to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2 */
		inline void TextureLineStrip4(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
										u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2);
		/** Sprite/Rectangle with 4 bit fraction part for x,y.

		\param color RGBA32 color
		\param x1, y1 x,y coords for vertex 1
		\param x2, y2 x,y coords for vertex 2
		\param z z value for all vertices.
		*/
		void Sprite4(u64 color, s32 x1, s32 y1, s32 x2, s32 y2, u32 z = 0);
		/** Sprite/Rectangle.

		\param color RGBA32 color
		\param x1, y1 x,y coords for vertex 1
		\param x2, y2 x,y coords for vertex 2
		\param z z value for all vertices.
		*/
		inline void Sprite(u64 color, s32 x1, s32 y1, s32 x2, s32 y2, u32 z = 0)
		{
			Sprite4(color, (x1) << 4, (y1) << 4, (x2) << 4, (y2) << 4, z);
		}
		/** Textured Sprite/Rectangle with 4 bit fraction part for x,y and u,v params. Use SetTexture to specify the texture to use.

		\param color RGBA32 color
		\param x1, y1 x,y coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		\param x2, y2 x,y coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		\param z z value for all vertices.
		*/
		void TextureSprite4(u64 color, s32 x1, s32 y1, u32 u1, u32 v1, s32 x2, s32 y2, u32 u2, u32 v2, u32 z);
		/** Textured Sprite/Rectangle. Use SetTexture to specify the texture to use.

		\param color RGBA32 color
		\param x1, y1 x,y coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		\param x2, y2 x,y coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		\param z z value for all vertices.
		*/
		inline void TextureSprite(u64 color, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 z)
		{
			TextureSprite4(color, x1 << 4, y1 << 4, u1 << 4, v1 << 4, x2 << 4, y2 << 4, u2 << 4, v2 << 4, z);
		}
		/** Textured Sprite/Rectangle with 4 bit fraction part for x,y and u,v params. If you simple wish to display
		    a texture you can use this function in which you specify the texture to use and position on screen.
			
			\param texture texture to use
			\param color RGBA32 color
			\param x1, y1 x,y coords for vertex 1 (position)
			\param x2, y2 x,y coords for vertex 2 (optional)
			\param z z value for all vertices. (optional)
		*/
		void TextureSprite4(Texture& texture, u64 color, u32 x1, u32 y1, u32 x2 = 0xFFFF0, u32 y2 = 0xFFFF0, u32 z = 0);
		/** Textured Sprite/Rectangle. If you simple wish to display a texture you can use this function in which you specify 
			the texture to use and position on screen.
			
			\param texture texture to use
			\param color RGBA32 color
			\param x1, y1 x,y coords for vertex 1 (position)
			\param x2, y2 x,y coords for vertex 2 (optional)
			\param z z value for all vertices. (optional)
		*/
		inline void TextureSprite(Texture& texture, u64 color, u32 x1, u32 y1, u32 x2 = 0xFFFF, u32 y2 = 0xFFFF, u32 z = 0)
		{
			TextureSprite4(texture, color, x1 << 4, y1 << 4, x2 << 4, y2 << 4, z);
		}
		/** Triangle with 4 bit fraction part for x,y

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
	
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		*/
		void Triangle4(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2, u64 color3, s32 x3, s32 y3, u32 z3);
		/** Triangle.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
	
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		*/
		inline void Triangle(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2, u64 color3, s32 x3, s32 y3, u32 z3)
		{
			Triangle4(color1, x1 << 4, y1 << 4, z1, color2, x2 << 4, y2 << 4, z2, color3, x3 << 4, y3 << 4, z3);
		}
		/** Triangle with 4 bit fraction part for x,y

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
	
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param vector3 use IntVector x,y,z components as coords for vertex 3
		*/
		inline void Triangle4(u64 color1, IntVector vector1, u64 color2, IntVector vector2, u64 color3, IntVector vector3)
		{
			Triangle4(	color1, vector1[0], vector1[1], vector1[2],
						color2, vector2[0], vector2[1], vector2[2],
						color3, vector3[0], vector3[1], vector3[2]);
						
		}
		/** Triangle.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
	
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param vector3 use IntVector x,y,z components as coords for vertex 3
		*/
		inline void Triangle(u64 color1, IntVector vector1, u64 color2, IntVector vector2, u64 color3, IntVector vector3)
		{
			Triangle(	color1, vector1[0], vector1[1], vector1[2],
						color2, vector2[0], vector2[1], vector2[2],
						color3, vector3[0], vector3[1], vector3[2]);
		}
		/** Textured Triangle with 4 bit fraction part for IntVector x,y and u,v params. Use SetTexture to specify the texture to use.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param vector3 use IntVector x,y,z components as coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/
		inline void TextureTriangle4(	u64 color1, IntVector vector1, u32 u1, u32 v1,
										u64 color2, IntVector vector2, u32 u2, u32 v2, 
										u64 color3, IntVector vector3, u32 u3, u32 v3)
		{
			TextureTriangle4(		color1, vector1[0], vector1[1], vector1[2], u1, v1,
									color2, vector2[0], vector2[1], vector2[2], u2, v2, 
									color3, vector3[0], vector3[1], vector3[2], u3, v3);

		}
		/** Textured Triangle. Use SetTexture to specify the texture to use.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param vector3 use IntVector x,y,z components as coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/
		inline void TextureTriangle(	u64 color1, IntVector vector1, u32 u1, u32 v1,
										u64 color2, IntVector vector2, u32 u2, u32 v2, 
										u64 color3, IntVector vector3, u32 u3, u32 v3)
		{
			TextureTriangle(	color1, vector1[0], vector1[1], vector1[2], u1 << 4, v1 << 4,
									color2, vector2[0], vector2[1], vector2[2], u2 << 4, v2 << 4, 
									color3, vector3[0], vector3[1], vector3[2], u3 << 4, v3 << 4);

		}
		/** Textured Triangle. Use SetTexture to specify the texture to use.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3	
		*/
		inline void TextureTriangle(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
										u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2, 
										u64 color3, s32 x3, s32 y3, u32 z3, u32 u3, u32 v3)
		{
			TextureTriangle4(	color1, x1 << 4, y1 << 4, z1, u1 << 4, v1 << 4,
									color2, x2 << 4, y2 << 4, z2, u2 << 4, v2 << 4, 
									color3, x3 << 4, y3 << 4, z3, u3 << 4, v3 << 4);
	
		}
		/** Textured Triangle with 4 bit fraction part for x,y and u,v params. Use SetTexture to specify the texture to use.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/	
		void TextureTriangle4(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
								u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2, 
								u64 color3, s32 x3, s32 y3, u32 z3, u32 u3, u32 v3);

		/** Textured Triangle Strip with 4 bit fraction part for x,y and u,v params. Use SetTexture to specify the texture to use.
			Use AddTextureVertex4 to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/		

		void TextureTriangleStrip4(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
									u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2, 
									u64 color3, s32 x3, s32 y3, u32 z3, u32 u3, u32 v3);

		/** Textured Triangle Strip. Use SetTexture to specify the texture to use.
			Use AddTextureVertex to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/		
		inline void TextureTriangleStrip(	u64 color1, s32 x1, s32 y1, u32 z1, u32 u1, u32 v1,
											u64 color2, s32 x2, s32 y2, u32 z2, u32 u2, u32 v2, 
											u64 color3, s32 x3, s32 y3, u32 z3, u32 u3, u32 v3)
		{
			TextureTriangleStrip4(		color1, x1 << 4, y1 << 4, z1, u1 << 4, v1 << 4,
										color2, x2 << 4, y2 << 4, z2, u2 << 4, v2 << 4, 
										color3, x3 << 4, y3 << 4, z3, u3 << 4, v3 << 4);
	
		}	
		/** Textured Triangle Strip with 4 bit fraction part for x,y and u,v params. Use SetTexture to specify the texture to use.
			Use AddTextureVertex4 to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param vector3 use IntVector x,y,z components as coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/		
		inline void TextureTriangleStrip4(	u64 color1, IntVector vector1, u32 u1, u32 v1,
											u64 color2, IntVector vector2, u32 u2, u32 v2, 
											u64 color3, IntVector vector3, u32 u3, u32 v3)
		{
			TextureTriangleStrip4(	color1, vector1[0], vector1[1], vector1[2], u1, v1,
									color2, vector2[0], vector2[1], vector2[2], u2, v2, 
									color3, vector3[0], vector3[1], vector3[2], u3, v3);

		}

		/** Textured Triangle Strip. Use SetTexture to specify the texture to use.
			Use AddTextureVertex to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param vector1 use IntVector x,y,z components as coords for vertex 1
		\param u1, v1 u,v texture mapping coords for vertex 1
		
		\param color2 RGBA32 color for vertex 2
		\param vector2 use IntVector x,y,z components as coords for vertex 2
		\param u2, v2 u,v texture mapping coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param vector3 use IntVector x,y,z components as coords for vertex 3
		\param u3, v3 u,v texture mapping coords for vertex 3
		*/		
		inline void TextureTriangleStrip(	u64 color1, IntVector vector1, u32 u1, u32 v1,
											u64 color2, IntVector vector2, u32 u2, u32 v2, 
											u64 color3, IntVector vector3, u32 u3, u32 v3)
		{
			TextureTriangleStrip(	color1, vector1[0], vector1[1], vector1[2], u1 << 4, v1 << 4,
									color2, vector2[0], vector2[1], vector2[2], u2 << 4, v2 << 4, 
									color3, vector3[0], vector3[1], vector3[2], u3 << 4, v3 << 4);

		}
		/** Triangle with 4 bit fraction part for x,y params. Use AddVertex4 to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
	
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		*/
		void TriangleStrip4(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2, u64 color3, s32 x3, s32 y3, u32 z3);
		
		/** Triangle. Use AddVertex to add addional vertices and EndStrip to end the strip.

		\param color1 RGBA32 color for vertex 1
		\param x1, y1, z1 x,y,z coords for vertex 1
	
		\param color2 RGBA32 color for vertex 2
		\param x2, y2, z2 x,y,z coords for vertex 2
		
		\param color3 RGBA32 color for vertex 3
		\param x3, y3, z3 x,y,z coords for vertex 3
		*/
		inline void TriangleStrip(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2, u64 color3, s32 x3, s32 y3, u32 z3)
		{
			TriangleStrip4(color1, x1 << 4, y1 << 4, z1, color2, x2 << 4, y2 << 4, z2, color3, x3 << 4, y3 << 4, z3);
		}
		/** Add vertex to TriangleStrip/Fan, LineStrip with 4 bit fraction part for x,y params.
		\param color RGBA32 color for the vertex
		\param x,y,z x,y,z coords for the vertex
		*/
		void AddVertex4(u64 color, s32 x, s32 y, u32 z);
		/** Add vertex to TriangleStrip/Fan, LineStrip.
		\param color RGBA32 color
		\param x,y,z x,y,z coords
		*/
		void AddVertex(u64 color, s32 x, s32 y, u32 z)
		{
			AddVertex4(color, x << 4, y << 4, z);
		}	
		/** Add vertex to TextureTriangleStrip/Fan, TextureLineStrip with 4 bit fraction part for x,y and u,v params.
		\param color RGBA32 color
		\param x,y,z x,y,z coords
		\param u,v u,v texture mapping coords
		*/
		void AddTextureVertex4(u64 color, s32 x, s32 y, u32 z, u32 u, u32 v);
		/** Add vertex to TextureTriangleStrip/Fan, TextureLineStrip.
		\param color RGBA32 color
		\param x,y,z x,y,z coords
		\param u,v u,v texture mapping coords
		*/
		void AddTextureVertex(u64 color, s32 x, s32 y, u32 z, u32 u, u32 v)
		{
			AddTextureVertex4(color, x << 4, y << 4, z, u << 4, v << 4);
		}
		/** End a strip/fan, must be called when ending any type of strip/fan. */
		void EndStrip();
		
		/** This function calls EndStrip. */
		inline void EndFan() { EndStrip(); }
		/** SetTexture. Set a texture to use with the Texture<prim> functions.
		
			\return returns false if it was unable to set the texture otherwise true.
		*/
		bool SetTexture(Texture& texture);
		/** Load and upload Texture to VRAM. 
			You do not need to call this function, since SetTexture already does this for you.		
		*/
		bool UploadTexture(Texture& texture);
		/** Load and upload Image from EE buffer.
			\param texture texture object you want to assign the texture to.
			\param buffer memory address at which the image resides.
		*/
		bool LoadImageBuffer(Texture& texture, u8* buffer);
		/** LoadImageFile. 
			\param texture texture object you want to assign the texture to.
			\param filename filename of the image you wish to upload.
		*/
		bool LoadImageFile(Texture& texture, char* filename);
		/** Set a GPR GS register. */
		void SetRegister(u64 address, u64 data);
		/** Set alpha blending mode.
		\par Detailed description of the modes
		<tt>
		CS = Color (RGB) Source\n
		CD = Color (RGB) Destination (framebuffer)\n
		AS = Alpha Source\n \n
		</tt>

		\par GifPipe::AlphaBlendNormal
			<tt>Result = (CS*AS) + (CD*(0x80-AS))</tt>\n
		\par GifPipe::AlphaBlendAdditive
			<tt>Result = CS+CD</tt>\n
		\par GifPipe::AlphaBlendSubtractive
			<tt>Result = CD-CS</tt>\n
		\par GifPipe::AlphaBlendLighten
			<tt>Result = CD*(0x80+AS)</tt> \n
		\par GifPipe::AlphaBlendDarken
			<tt>Result = CD*(0x80-AS)</tt> \n
		*/
		bool AlphaBlending(u32 mode);
		/** Set alpha blending formula. 
			Please see the GS Manual for further details.
		*/
		void AlphaBlending(u32 a, u32 b, u32 c, u32 d, u8 fix);
		/** Enable alpha blending per pixel. 
			It is possible to control which pixels the alpha blending
			formula is applied by setting the MSB (bit 7, (1 << 7) = 0x80) of the alpha to 1 and
			enabling per pixel alpha blending with this function. If MSB is set to 0, alpha blending is not applied.
			
			To enable alpha blending for a pixel, set the the alpha value (Av, range 0-128) to the following:
			 Av = Av | (1 << 7);
			To enable alpha blending for a pixel, do not alter the alpha value (Av, range 0-128) 
			 Av = Av;

			\param enable true or false, whether to enable alpha per pixel.
		*/
		bool AlphaPerPixel(bool enable);
		/* Sets the MSB bit (bit 7) for alpha value for RGBA32 and alpha value for RGBA16.
		   Pleas see GS Manual "Registers"->"General Purpose Registers"->"FBA_1/FBA_2" for further details.
		
			\param val value to set, must be 0 or 1.
		*/
		bool AlphaCorrection(u32 val);
		/** Enable/disable AlphaTest.
			 Please see GS Manual "Registers"->"General Purpose Registers"->"TEST_1/TEST_2".
			 This function only sets the alpha test parts of the TEST_1/TEST_2 register, for
			 Z test, please see the ZTest function.
			
			\param enable 
			\param atst
			\param aref
			\param afail
			\param date
			\param datm
		*/
		void AlphaTest(bool enable, u32 atst, u8 aref, u32 afail, u32 date, u32 datm);
		/** Enable/disable ZTest.
			Please see GS Manual <tt>"Registers"->"General Purpose Registers"->"TEST_1/TEST_2"</tt>.
			 This function only sets the Z test parts of the TEST_1/TEST_2 register, for
			 alpha test, please see the AlphaTest function.

			 \param enable this value does not set the ZTE bit of the TEST register, since that
			        is not allowed. But it sets the ZMSK bit in ZBUF register. So you can enable
					and disable Z test with function without worrying about doing something which
					is not alllowd.
			 \param ztst
		*/
		void ZTest(u32 enable, u32 ztst = ZTEST_ALWAYS);
		/** Sets the "Texture Function"
		Please see the GS Manaul "Drawing Function"->"Texture Mapping"->"Texture Function" for
		further details. 
	
		\param function Can be set to one of the following <tt>GifPipe::TextureModulate, GifPipe::TextureDecal,
			GifPipe::TextureHighlight or GifPipe::TextureHighlight2</tt>
		*/
		bool TextureFunction(u32 function);

		/** Sets the "Texture Color Componet.
		Please see the GS Manaul "Drawing Function"->"Texture Mapping"->"Texture Function" for
		further details. 
	
		\param component Can be set to one of the following <tt>GifPipe::RGB, GifPipe::RGBA</tt> 
		*/
		bool TextureColorComponent(u32 component);	
		/** SetNextFramebuffer. If there is no avaliable framebuffer, this function will block until there is one. 
			This function does not, unlike GsDriver::SetNextDrawbuffer() wait until the transfers end, the buffer
			switch data is just pushed onto the GifPipe's double bufferd GifPacket.
		*/
		void SetNextDrawbuffer();
};

#endif
