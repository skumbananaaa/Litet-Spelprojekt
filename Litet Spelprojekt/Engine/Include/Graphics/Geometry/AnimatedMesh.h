#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/Joint.h>
#include <vector>

class API AnimatedMesh
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:

	glm::mat4 AssimpToGLMMat4(aiMatrix4x4 mat);
	glm::mat4 GetJoinTransofrm();
	void AddJointsToArray(Joint headJoint, glm::mat4* jointMatrix);

public: 



private:



private:
	Joint* m_pRootNode;
	uint32 m_VAO;
	uint32 m_Texture;

	uint32 m_NumOfJoints;

};