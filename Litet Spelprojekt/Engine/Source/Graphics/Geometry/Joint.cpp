#include <EnginePch.h>
#include <Graphics/Geometry/Joint.h>

Joint::Joint(glm::mat4 bindLocalTransform, uint32 nrOfChildren)
{
	m_LocalBindTransform = bindLocalTransform;
	m_Transform = glm::mat4(1.0f);
	m_NrOfChildren = nrOfChildren;
	m_ppChildren = new Joint*[m_NrOfChildren];

	for (uint32 i = 0; i < m_NrOfChildren; i++)
	{
		m_ppChildren[i] = nullptr;
	}

	m_LocalPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_LocalRot = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

Joint::~Joint()
{
	for (uint32 i = 0; i < m_NrOfChildren; i++)
	{
		DeleteSafe(m_ppChildren[i]);
	}
	DeleteArrSafe(m_ppChildren);
}

const glm::vec4& Joint::GetRotation() const noexcept
{
	return m_LocalRot;
}

const glm::vec3& Joint::GetPosition() const noexcept
{
	return m_LocalPos;
}

const glm::mat4& Joint::GetTransformMat() const noexcept
{
	return m_Transform;
}

const glm::mat4& Joint::GetLocalBindTransform() const noexcept
{
	return m_LocalBindTransform;
}

void Joint::SetRotation(const glm::vec4& rotationVec)
{
	m_LocalRot = rotationVec;
	m_isDirty = true;
}

void Joint::SetPosition(const glm::vec3 & dir)
{
	m_LocalPos = dir;
	m_isDirty = true;
}

void Joint::SetChild(uint32 index, const Joint * child) noexcept
{
	DeleteSafe(m_ppChildren[index]);
	m_ppChildren[index] = CreateFromJoint(child);
}

void Joint::UpdateTransform(const glm::mat4 & parentTransform)
{
	if (m_isDirty)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_LocalPos);
		transform = glm::rotate(transform, m_LocalRot.w, glm::vec3(m_LocalRot));
		m_LocalBindTransform = transform;
		m_Transform = m_LocalBindTransform * parentTransform;
		m_InverseBindTransform = m_Transform;
		m_InverseBindTransform = glm::inverse(m_InverseBindTransform);
		m_isDirty = false;
	}
	else
	{
		m_InverseBindTransform = m_Transform;
		m_InverseBindTransform = glm::inverse(m_InverseBindTransform);
	}
	for (uint32 i = 0; i < m_NrOfChildren; i++)
	{
		m_ppChildren[i]->UpdateTransform(m_Transform);
	}
}

Joint * Joint::CreateFromJoint(const Joint * joint) noexcept
{
	Joint * tmp = new Joint(joint->m_LocalBindTransform, joint->m_NrOfChildren);
	for (uint32 i = 0; i < tmp->m_NrOfChildren; i++)
	{
		tmp->m_ppChildren[i] = CreateFromJoint(joint->m_ppChildren[i]);
	}
	return tmp;
}

const Joint * Joint::GetChild(uint32 index) const
{
	return m_ppChildren[index];
}
