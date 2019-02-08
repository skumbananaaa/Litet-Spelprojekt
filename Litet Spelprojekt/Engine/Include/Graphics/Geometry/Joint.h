#pragma once
#include <EnginePch.h>
#include <GLM/matrix.hpp>
#include<GLM/gtc/matrix_transform.hpp>
class API Joint
{
private:
public:
	Joint(glm::mat4 bindLocalTransform, uint32 nrOfChildren = 0);
	~Joint();
	void Rotate(const glm::vec3& rotationVec, float rot);
private:
	Joint ** m_ppChildren;
	uint32 m_NrOfChildren;

	glm::mat4 m_Transform;
	glm::mat4 m_LocalBindTransform;
	glm::mat4 m_InverseBindTransform;
};