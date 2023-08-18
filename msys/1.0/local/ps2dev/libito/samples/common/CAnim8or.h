#ifndef _CANIM8OR_H
#define _CANIM8OR_H

#include <tamtypes.h>
#include <ItoTypes.h>
#include "Anim8orExport.h"

/** CAnim8or class which loads C files exported by CAnim8or.
	Unfortunately these files do not compile out of the box on PS2, you need
	to alter the files by hand to make them compile. In the future there will
	be a better solution for loading 3D models.

*/

class CAnim8or
{
	private:
		Anim8orObject* m_Object;
	public:
		CAnim8or();
		~CAnim8or();
		bool Load(Anim8orObject& object);
		u32 GetMeshCount();
		u32 GetVertexCount(u32 mesh);
		bool GetVertices(Vector* vertices, u32 mesh);
		bool GetNormals(Vector* vertices, u32 mesh);
};


#endif
