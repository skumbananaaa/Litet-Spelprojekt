#include <EnginePch.h>
#include <Graphics/Geometry/Joint.h>

Joint::Joint(glm::mat4 bindLocalTransform, uint32 nrOfChildren)
{
	m_LocalBindTransform = bindLocalTransform;
	m_Transform = glm::mat4(1.0f);
	m_NrOfChildren = nrOfChildren;
	m_ppChildren = new Joint*[m_NrOfChildren];
}

Joint::~Joint()
{
	for (uint32 i = 0; i < m_NrOfChildren; i++)
	{
		DeleteSafe(m_ppChildren[i]);
	}
	DeleteArrSafe(m_ppChildren);
}

void Joint::Rotate(const glm::vec3& rotationVec, float rot)
{
	m_Transform = glm::rotate(m_Transform, rot, rotationVec);
}
