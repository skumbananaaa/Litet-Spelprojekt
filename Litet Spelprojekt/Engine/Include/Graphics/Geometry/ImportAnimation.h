#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/AnimatedMesh.h>

#define MAX_BONES_PER_VERTEX 4

struct BoneVertex
{
	glm::vec3 position;
	glm::vec3 texCoords;
	glm::vec3 normals;
	glm::vec2 boneID0;

};

struct VertexBoneData
{
	uint32 boneID[MAX_BONES_PER_VERTEX];
	float weights[MAX_BONES_PER_VERTEX];
};



class API ImportAnimation
{


public:
	
	AnimatedMesh* CreateAnimatedMeshFromColladaFile(const char* file);

	glm::mat4 AssimpToGLMMat4(aiMatrix4x4 mat);


public:

	glm::mat4 m_GlobalInverseTransform;
};