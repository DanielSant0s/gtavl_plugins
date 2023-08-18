#include "CAnim8or.h"

CAnim8or::CAnim8or()
{
	m_Object = 0;
}

CAnim8or::~CAnim8or()
{

}

bool CAnim8or::Load(Anim8orObject& object)
{
	m_Object = &object;

	return true;
}
u32 CAnim8or::GetMeshCount()
{
	if(m_Object)
		return m_Object->nMeshes;
	else
		return 0;
}

u32 CAnim8or::GetVertexCount(u32 mesh)
{
	if(m_Object)
		return m_Object->meshes[mesh].nIndices;
	else
		return 0;
}

bool CAnim8or::GetVertices(Vector* vertices, u32 mesh)
{
	if(m_Object)
	{
		for(u32 i=0; i < GetVertexCount(mesh); i++)
		{
			vertices[i][0] = m_Object->meshes[mesh].coordinates[ (m_Object->meshes[mesh].indices[i]*3)+0 ];
			vertices[i][1] = m_Object->meshes[mesh].coordinates[ (m_Object->meshes[mesh].indices[i]*3)+1 ];
			vertices[i][2] = m_Object->meshes[mesh].coordinates[ (m_Object->meshes[mesh].indices[i]*3)+2 ];
			vertices[i][3] = 1.0f;
		}
	
		return true;
	}
	else
	{
		return false;
	}
}

bool CAnim8or::GetNormals(Vector* vertices, u32 mesh)
{
	if(m_Object)
	{
		for(u32 i=0; i < GetVertexCount(mesh); i++)
		{
			vertices[i][0] = m_Object->meshes[mesh].normals[ (m_Object->meshes[mesh].indices[i]*3)+0 ];
			vertices[i][1] = m_Object->meshes[mesh].normals[ (m_Object->meshes[mesh].indices[i]*3)+1 ];
			vertices[i][2] = m_Object->meshes[mesh].normals[ (m_Object->meshes[mesh].indices[i]*3)+2 ];
			vertices[i][3] = 1.0f;
		}
	
		return true;
	}
	else
	{
		return false;
	}
}
