#include <EnginePch.h>
#include <Graphics/Geometry/Joint.h>

Joint::Joint(glm::mat4 bindLocalTransform, uint32 nrOfChildren)
{
	m_LocalBindTransform = bindLocalTransform;
	m_Transform = glm::mat4(1.0f);
	m_NrOfChildren = nrOfChildren;
	m_ppChildren = new Joint*[m_NrOfChildren];
	m_LocalPos = glm::vec3(0.0f, 0.0f, 0.0f);
}

Joint::~Joint()
{
	for (uint32 i = 0; i < m_NrOfChildren; i++)
	{
		DeleteSafe(m_ppChildren[i]);
	}
	DeleteArrSafe(m_ppChildren);
}

void Joint::SetRotation(const glm::vec4& rotationVec)
{
	m_LocalRot = rotationVec;
}

void Joint::SetPosition(const glm::vec3 & dir)
{
	m_LocalPos = dir;
}

void Joint::UpdateTransform(const glm::mat4 & transMat)
{
	if (m_isDirty)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_LocalPos);
		transform = glm::rotate(transform, m_LocalRot.w, glm::vec3(m_LocalRot));
		m_Transform = transform * transMat;
	}
	else
	{
		m_Transform *= transMat;
	}
	for (uint32 i = 0; i < m_NrOfChildren; i++)
	{
		m_ppChildren[i]->UpdateTransform(m_Transform);
	}
}
