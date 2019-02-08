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
	void SetRotation(const glm::vec4& rotationVec);
	void SetPosition(const glm::vec3& dir);

	void UpdateTransform(const glm::mat4 & transMat = glm::mat4(1.0f));
private:
	Joint ** m_ppChildren;
	uint32 m_NrOfChildren;

	glm::mat4 m_Transform;
	glm::mat4 m_LocalBindTransform;

	glm::vec4 m_LocalRot;
	glm::vec3 m_LocalPos;
	bool m_isDirty;
};