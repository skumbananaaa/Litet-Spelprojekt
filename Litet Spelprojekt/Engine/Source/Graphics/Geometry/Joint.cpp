#include <EnginePch.h>
#include <Graphics/Geometry/Joint.h>

Joint::Joint(glm::mat4 bindLocalTransform, uint32 nrOfChildren)
{
	m_LocalBindTransform = bindLocalTransform;
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
