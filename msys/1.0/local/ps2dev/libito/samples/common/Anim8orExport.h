#ifndef _ANIM8OREXPORT_H
#define _ANIM8OREXPORT_H


typedef float color[4];

typedef struct Anim8orMaterial {
	color ambient;
	color diffuse;
	color specular;
	color emissive;
	float Ka, Kd, Ks, Ke;
	float Roughness, Brilliance;
	char *ambientTexture;
	char *diffuseTexture;
	char *specularTexture;
	char *emissiveTexture;
	char *transparencyTexture;
	char *bumpmapTexture;
} Anim8orMaterial;

typedef struct Anim8orMesh {
    const char *name;
    int nVertices;
    int nIndices;
    int *indices;
    unsigned char *matindices;
    float *coordinates;
    float *normals;
    float *texcoords;
    Anim8orMaterial *materials;
} Anim8orMesh;

typedef struct Anim8orObject {
    const char *name;
    int nMeshes;
    Anim8orMesh *meshes;
} Anim8orObject;

#endif 
