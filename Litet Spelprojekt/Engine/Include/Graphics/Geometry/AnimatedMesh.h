#pragma once
#include <EnginePch.h>
#include <vector>

class API AnimatedMesh
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:

	glm::mat4 AssimpToGLMMat4(aiMatrix4x4 mat);
	glm::mat4 GetJoinTransofrm();

public: 



private:



private:
	uint32 m_VAO;
	uint32 m_Texture;

	uint32 m_NumOfJoints;

};